#include"xiong.h"

//读取一年内的比赛列表，过滤出Div1，2，3，4的比赛
cJSON* filter_contests(cJSON* root) {

	//获取数组保存原始信息
	cJSON* origin_contests_array = cJSON_GetObjectItemCaseSensitive(root, "result");
	if (!origin_contests_array || !cJSON_IsArray(origin_contests_array)) {
		fprintf(stderr, "Missing or invalid 'result' array\n");
		return NULL;
	}


	//创建新数组保存筛选结果
	cJSON* filtered_contests_array = cJSON_CreateArray();
	if (!filtered_contests_array) {
		printf("Failed to create filtered contests array\n");
		return NULL;
	}


	time_t now = time(NULL);
	time_t one_year_ago = now - (long long)(365 * 24) * 3600;

	// 筛选一年内 div比赛
	cJSON* contest = NULL;
	cJSON_ArrayForEach(contest, origin_contests_array) {
		cJSON* name = cJSON_GetObjectItemCaseSensitive(contest, "name");
		cJSON* phase = cJSON_GetObjectItemCaseSensitive(contest, "phase");
		cJSON* startTime = cJSON_GetObjectItemCaseSensitive(contest, "startTimeSeconds");

		if (!cJSON_IsString(phase) || strcmp(phase->valuestring, "FINISHED") != 0) {
			continue; // 跳过未结束的比赛
		}

		// 2. 检查 startTime 是否在一年内
		if (!cJSON_IsNumber(startTime) || startTime->valueint < one_year_ago) {
			continue; // 跳过一年前的比赛
		}

		// 3. 检查 name 是否包含 "Div."
		if (!cJSON_IsString(name) || strstr(name->valuestring, "Div.") == NULL) {
			continue; // 跳过非 Div 比赛
		}

		cJSON_AddItemToArray(filtered_contests_array, cJSON_Duplicate(contest, 1));

	}


	// 创建一个包含筛选结果的完整响应对象
	cJSON* response = cJSON_CreateObject();
	if (!response) {
		fprintf(stderr, "Failed to create output JSON\n");
		return NULL;
	}
	cJSON_AddItemToObject(response, "status", cJSON_CreateString("OK"));
	cJSON_AddItemToObject(response, "result", filtered_contests_array);
	return response;
}

/*
	读取用户 rating变化(含参赛记录)
	 获取用户所有参赛记录（含排名和Rating变化）
 {
  "contestId": "比赛ID",
  "contestName": "比赛名称",
  "rank": "用户排名",
  "oldRating": "参赛前Rating",
  "newRating": "参赛后Rating",
  "ratingUpdateTimeSeconds": "Rating更新时间戳"
}
*/
//读取用户参加比赛的列表，过滤出Div1，2，3，4的比赛
cJSON* filter_user_contests(cJSON* root) {
	//获取比赛数组保存原始信息
	cJSON* origin_contests_array = cJSON_GetObjectItemCaseSensitive(root, "result");
	if (!origin_contests_array || !cJSON_IsArray(origin_contests_array)) {
		fprintf(stderr, "Missing or invalid 'result' array\n");
		return NULL;
	}

	// 筛选一年内 div比赛
	cJSON* filtered_contests_array = json_filter_array(origin_contests_array, "contestName", "Div.");

	// 组装包含筛选结果的完整响应对象
	cJSON* response = assemble_cJSON_array_to_object(filtered_contests_array);

	return response;
}

//统计和展示用户参加各级比赛的次数，参赛次数的月度分布，出勤率
struct user_attend_contest_information user_attend_contest_information_calculate(cJSON* user_attend_contest, cJSON* all_contest) {
	struct user_attend_contest_information information = {
		.month = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		.attendance_rate = 0.0,
		.user_attend_div1 = 0,
		.user_attend_div2 = 0,
		.user_attend_div3 = 0,
		.user_attend_div4 = 0,
		.eligible_contest = 0,
		.attend_contest = 0,
		.maxRating = 0,
		.sum_div1 = 0,
		.sum_div2 = 0,
		.sum_div3 = 0,
		.sum_div4 = 0,
	};

	calculate_user_attend_information(&information, user_attend_contest);
	calculate_all_contest_information(&information, all_contest);
	eligible_cont(&information);
	information.attendance_rate = (double)information.attend_contest / information.eligible_contest * 100;
	/*printf("information.attend_contest / information.eligible_contest=%d %d",
		information.attend_contest, information.eligible_contest
	);
	printf("%d\n", information.attendance_rate);*/
	return information;
}

//计算当前 rating能参加的比赛数量
void eligible_cont(struct user_attend_contest_information* information) {
	if (information->maxRating <= 1400) {
		information->eligible_contest = information->sum_div4 + information->sum_div3 + information->sum_div2;
	}
	else if (information->maxRating <= 1600) {
		information->eligible_contest = information->sum_div3 + information->sum_div2;
	}
	else if (information->maxRating < 1900) {
		information->eligible_contest = information->sum_div2;
	}
	else {
		information->eligible_contest = information->sum_div1 + information->sum_div2;
	}
	//printf("%d", information->eligible_contest);
}

//计算用户各级参赛数量
void calculate_user_attend_information(struct user_attend_contest_information* information, cJSON* user_attend_contest) {

	//获取用户比赛数组保存原始信息
	cJSON* user_contests_array = cJSON_GetObjectItemCaseSensitive(user_attend_contest, "result");
	if (!user_contests_array || !cJSON_IsArray(user_contests_array)) {
		printf("Missing or invalid 'user_contests_array' array\n");
	}

	cJSON* contest = NULL;
	cJSON_ArrayForEach(contest, user_contests_array) {
		cJSON* name = cJSON_GetObjectItemCaseSensitive(contest, "contestName");
		cJSON* rating = cJSON_GetObjectItemCaseSensitive(contest, "newRating");
		cJSON* ratingUpdateTimeSeconds = cJSON_GetObjectItemCaseSensitive(contest, "ratingUpdateTimeSeconds");

		if (!cJSON_IsString(name) || strstr(name->valuestring, "Div.") == NULL) {
			continue; // 跳过非 Div 比赛
		}
		if (!cJSON_IsNumber(ratingUpdateTimeSeconds) || !is_the_past_12_month(ratingUpdateTimeSeconds->valueint)) {
			continue; // 跳过一年前的比赛
		}
		if (!cJSON_IsNumber(rating) || rating->valueint > information->maxRating) {
			information->maxRating = rating->valueint;// 记录最大Rating
		}

		int month = (ratingUpdateTimeSeconds->valueint / (30 * 24 * 3600)) % 12;
		information->month[month]++;// 记录月度分布

		if (strstr(name->valuestring, "Div. 1")) {
			information->user_attend_div1++;
		}
		if (strstr(name->valuestring, "Div. 2")) {
			information->user_attend_div2++;
		}
		if (strstr(name->valuestring, "Div. 3")) {
			information->user_attend_div3++;
		}
		if (strstr(name->valuestring, "Div. 4")) {
			information->user_attend_div4++;
		}

		information->attend_contest = information->user_attend_div1 + information->user_attend_div2 +
			information->user_attend_div3 + information->user_attend_div4;
	}

}

//计算各级比赛总数量
void calculate_all_contest_information(struct user_attend_contest_information* information, cJSON* all_contest) {

	//获取所有比赛数组保存原始信息
	cJSON* all_contests_array = cJSON_GetObjectItemCaseSensitive(all_contest, "result");
	if (!all_contests_array || !cJSON_IsArray(all_contests_array)) {
		printf("Missing or invalid 'all_contest_array' array\n");
	}

	cJSON* contest = NULL;
	cJSON_ArrayForEach(contest, all_contests_array) {
		cJSON* name = cJSON_GetObjectItemCaseSensitive(contest, "name");
		cJSON* startTime = cJSON_GetObjectItemCaseSensitive(contest, "startTimeSeconds");

		if (!cJSON_IsString(name) || strstr(name->valuestring, "Div.") == NULL) {
			continue; // 跳过非 Div 比赛
		}
		if (!is_the_past_12_month(startTime->valueint)) {
			continue; // 跳过一年前的比赛
		}

		if (strstr(name->valuestring, "Div. 1")) {
			information->sum_div1++;
		}
		if (strstr(name->valuestring, "Div. 2")) {
			information->sum_div2++;
		}
		if (strstr(name->valuestring, "Div. 3")) {
			information->sum_div3++;
		}
		if (strstr(name->valuestring, "Div. 4")) {
			information->sum_div4++;
		}
	}
}

//获取所有参赛的比赛的级别，名称，时间，用户排名
cJSON* get_user_contest_detail(char* user_name, cJSON* user_contest_list) {
	printf("正在获取 %s 所有比赛详细信息:\n", user_name);
	//获取所有比赛数组保存原始信息
	cJSON* all_contests_array = cJSON_GetObjectItemCaseSensitive(user_contest_list, "result");
	if (!all_contests_array || !cJSON_IsArray(all_contests_array)) {
		printf("Missing or invalid 'all_contest_array' array\n");
	}

	cJSON* all_contests_information_array = cJSON_CreateArray();
	if (!all_contests_information_array || !cJSON_IsArray(all_contests_array)) {
		printf("Missing or invalid 'all_contest_information_array' array\n");
	}

	cJSON* contest = NULL;
	cJSON_ArrayForEach(contest, all_contests_array) {
		
		cJSON* contestName = cJSON_GetObjectItemCaseSensitive(contest, "contestName");//比赛名
		cJSON* ratingUpdateTimeSeconds = cJSON_GetObjectItemCaseSensitive(contest, "ratingUpdateTimeSeconds");//用于提取一年内数据
		cJSON* contestId = cJSON_GetObjectItemCaseSensitive(contest, "contestId");//id用于 api参数

		if (!cJSON_IsNumber(ratingUpdateTimeSeconds) || !is_the_past_12_month(ratingUpdateTimeSeconds->valueint)) {
			continue; // 跳过一年前的比赛
		}

		cJSON* every_contest = fetch_contest_standings(contestId->valueint, user_name);
		cJSON* every_contests_array = cJSON_GetObjectItemCaseSensitive(every_contest, "result");
		if (!all_contests_array || !cJSON_IsArray(all_contests_array)) {
			printf("Missing or invalid 'all_contest_array' array\n");
		}

		cJSON_AddItemToArray(all_contests_information_array, every_contests_array);
		
		
	}

	// 创建一个包含筛选结果的完整响应对象
	cJSON* response = assemble_cJSON_array_to_object(all_contests_information_array);
	printf("所有比赛信息获取完成\n");
	return response;
}

//创建一个包含筛选结果的完整响应对象
cJSON* assemble_cJSON_array_to_object(cJSON* cJSON_array) {
	cJSON* response = cJSON_CreateObject();
	if (!response) {
		fprintf(stderr, "Failed to create output JSON\n");
		return NULL;
	}
	cJSON_AddItemToObject(response, "status", cJSON_CreateString("OK"));
	cJSON_AddItemToObject(response, "result", cJSON_array);
	return response;
}

//	输入参数,需要过滤的 json数组  过滤标签名   过滤字符串
cJSON* json_filter_array(cJSON* origin_array, char* labelName, char* filterStr) {
	cJSON* filtered_array = cJSON_CreateArray();
	if (!filtered_array) {
		printf("Failed to create filtered_array\n");
		return NULL;
	}
	cJSON* unit = NULL;
	cJSON_ArrayForEach(unit, origin_array) {
		cJSON* label_content = cJSON_GetObjectItemCaseSensitive(unit, labelName);
		if (cJSON_IsString(label_content) && strstr(label_content->valuestring, filterStr) != NULL) {
			cJSON_AddItemToArray(filtered_array, cJSON_Duplicate(unit, 1));
		}
	}
	cJSON_Delete(origin_array);
	return filtered_array;
}

//判断是否为一年内
int is_the_past_12_month(int TimeSeconds) {
	time_t now = time(NULL);
	time_t one_year_ago = now - (long long)(365 * 24) * 3600;
	return TimeSeconds > one_year_ago ? 1 : 0;
}

//统计全年各级比赛各题的通过准确性，题目分值的分布（比如最低，最高，均值，方差）等信息，并使用合适的图例展示
cJSON* get_all_contest_statistics(cJSON* contest_list){
	printf("正在获取所有比赛详细信息:\n");
	//获取所有比赛数组保存原始信息

	
	cJSON* all_contests_array = cJSON_GetObjectItemCaseSensitive(contest_list, "result");

	//save_cJSON_to_file("all_contests_array.txt", all_contests_array);

	if (!all_contests_array || !cJSON_IsArray(all_contests_array)) {
		printf("Missing or invalid 'all_contest_array' array\n");
	}

	cJSON* all_contests_information_array = cJSON_CreateArray();
	if (!all_contests_information_array || !cJSON_IsArray(all_contests_array)) {
		printf("Missing or invalid 'all_contest_information_array' array\n");
	}

	cJSON* contest = NULL;
	cJSON_ArrayForEach(contest, all_contests_array) {

		cJSON* ratingUpdateTimeSeconds = cJSON_GetObjectItemCaseSensitive(contest, "startTimeSeconds");//用于提取一年内数据
		cJSON* contestId = cJSON_GetObjectItemCaseSensitive(contest, "id");//id用于 api参数

		if (!cJSON_IsNumber(ratingUpdateTimeSeconds) || !is_the_past_12_month(ratingUpdateTimeSeconds->valueint)) {
			continue; // 跳过一年前的比赛
		}

		cJSON* every_contest = fetch_contest_standings(contestId->valueint, NULL);
		cJSON* every_contests_array = cJSON_GetObjectItemCaseSensitive(every_contest, "result");
		if (!all_contests_array || !cJSON_IsArray(all_contests_array)) {
			printf("Missing or invalid 'all_contest_array' array\n");
		}
		
		cJSON_AddItemToArray(all_contests_information_array, every_contests_array);
		
		
	}

	// 创建一个包含筛选结果的完整响应对象
	cJSON* response = assemble_cJSON_array_to_object(all_contests_information_array);
	printf("所有比赛信息获取完成\n");
	return response;
}


cJSON* convert_struct_to_json(struct user_attend_contest_information information, const char* user_name) {
	// 创建根对象
	cJSON* root = cJSON_CreateObject();
	if (root == NULL) {
		fprintf(stderr, "无法创建 JSON 对象\n");
		return NULL;
	}

	// 添加字段
	cJSON_AddStringToObject(root, "user_name", user_name);
	cJSON_AddNumberToObject(root, "attendance_rate", information.attendance_rate);
	cJSON_AddNumberToObject(root, "user_attend_div1", information.user_attend_div1);
	cJSON_AddNumberToObject(root, "user_attend_div2", information.user_attend_div2);
	cJSON_AddNumberToObject(root, "user_attend_div3", information.user_attend_div3);
	cJSON_AddNumberToObject(root, "user_attend_div4", information.user_attend_div4);
	cJSON_AddNumberToObject(root, "eligible_contest", information.eligible_contest);
	cJSON_AddNumberToObject(root, "attend_contest", information.attend_contest);
	cJSON_AddNumberToObject(root, "maxRating", information.maxRating);

	// 添加月份数组
	cJSON* month_array = cJSON_CreateIntArray(information.month, 12);
	cJSON_AddItemToObject(root, "month", month_array);

	// 添加其他字段
	cJSON_AddNumberToObject(root, "sum_div1", information.sum_div1);
	cJSON_AddNumberToObject(root, "sum_div2", information.sum_div2);
	cJSON_AddNumberToObject(root, "sum_div3", information.sum_div3);
	cJSON_AddNumberToObject(root, "sum_div4", information.sum_div4);
	return root;
}

// 字符串保存函数
void saveStringToFile(const char* filename, const char* text) {
	char path[MAX_PATH];
	snprintf(path, sizeof(path), "./out/%s", filename);
	FILE* file = fopen(path, "w");  // 打开文件用于写入
	if (file != NULL) {                  // 检查文件是否成功打开
		fprintf(file, "%s", text);       // 写入字符串
		fclose(file);                    // 关闭文件
	}
}

//将cJSON数据保存到./out 中
void save_cJSON_to_file(const char* filename, cJSON* root) {
	char* json_str = cJSON_Print(root);
	saveStringToFile(filename, json_str);
	free(json_str);
}


void save_user_contest_detail(struct user_attend_contest_information information, const char* user_name) {
	//统计和展示用户参加各级比赛的次数，参赛次数的月度分布，出勤率

	char path[MAX_PATH];
	snprintf(path, sizeof(path), "./out/%s", "user_detail.txt");
	FILE* file = fopen(path, "w");  // 打开文件用于写入
	if (file != NULL) {                  // 检查文件是否成功打开

		fprintf(file, "用户%s参加的比赛有：\n", user_name);
		fprintf(file, "参加了Div1的比赛有 % d场\n", information.user_attend_div1);
		fprintf(file, "参加了Div2的比赛有%d场\n", information.user_attend_div2);
		fprintf(file, "参加了Div3的比赛有%d场\n", information.user_attend_div3);
		fprintf(file, "参加了Div4的比赛有%d场\n", information.user_attend_div4);
		fprintf(file, "用户出勤率为%.2f%%\n", information.attendance_rate);

		for (int i = 0; i < 12; i++) {
			fprintf(file, "用户在%d月份参加了%d场比赛\n", i + 1, information.month[i]);
		}

		fclose(file);                    // 关闭文件
	}
	else {
		printf("文件打开失败\n");
		return;
	}
}