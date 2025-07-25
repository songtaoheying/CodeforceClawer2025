#include"xiong.h"





// 修复后的函数，支持重试机制
char* fetch_json_str(const char* url) {
	int max_retries = 3;
	CURL* curl = NULL;
	struct MemoryStruct chunk;
	chunk.memory = malloc(1); // 初始分配1字节
	chunk.size = 0;
	chunk.total_received = 0;

	if (!chunk.memory) {
		fprintf(stderr, "内存分配失败\n");
		return NULL;
	}

	int retry_count = 0;
	while (retry_count <= max_retries) {
		curl = curl_easy_init();
		if (!curl) {
			fprintf(stderr, "无法初始化cURL\n");
			free(chunk.memory);
			return NULL;
		}

		printf("正在读取 Web 响应... (尝试 %d/%d)\n", retry_count + 1, max_retries + 1);

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);

		CURLcode res = curl_easy_perform(curl);

		// 换行避免进度信息被覆盖
		printf("\n");

		if (res == CURLE_OK) {
			printf("读取完成! 总接收字节数: %zu(%s)\n", chunk.total_received,
				format_size(chunk.total_received));
			curl_easy_cleanup(curl);
			return chunk.memory; // 成功返回数据
		}
		else {
			fprintf(stderr, "请求失败: %s\n", curl_easy_strerror(res));
			retry_count++;
			if (retry_count > max_retries) {
				fprintf(stderr, "达到最大重试次数 (%d)，放弃请求。\n", max_retries);
				break;
			}
			printf("等待1秒后重试...\n");
			Sleep(1); // 等待1秒后重试
		}

		curl_easy_cleanup(curl);
	}

	// 清理内存并返回NULL
	free(chunk.memory);
	return chunk.memory;
}






















const char* format_size(size_t bytes) {
	static char buffer[50];
	const char* units[] = { "B", "KB", "MB", "GB", "TB" };
	int unit = 0;
	double size = bytes;

	while (size >= 1024 && unit < 4) {
		size /= 1024;
		unit++;
	}

	snprintf(buffer, sizeof(buffer), "%.2f %s", size, units[unit]);
	return buffer;
}



static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)userp;

	// 更新总接收字节数
	mem->total_received += realsize;

	// 实时显示进度
	printf("\r正在读取响应流... (读取的字节数: %zu)", mem->total_received);
	fflush(stdout);

	// 内存分配和存储逻辑
	char* ptr = realloc(mem->memory, mem->size + realsize + 1);
	if (!ptr) {
		fprintf(stderr, "内存不足!\n");
		return 0;
	}

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

//char* fetch_json_str(const char* url)
//{
//	CURL* curl = curl_easy_init();
//	if (!curl) {
//		fprintf(stderr, "无法初始化cURL\n");
//		return;
//	}
//
//	struct MemoryStruct chunk = { 0 };
//	chunk.memory = malloc(1); // 初始分配1字节
//	chunk.size = 0;
//	chunk.total_received = 0;
//
//	printf("正在读取 Web 响应...\n");
//
//	curl_easy_setopt(curl, CURLOPT_URL, url);
//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
//	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
//
//	CURLcode res = curl_easy_perform(curl);
//
//	// 换行避免进度信息被覆盖
//	printf("\n");
//
//	if (res != CURLE_OK) {
//		fprintf(stderr, "请求失败: %s\n", curl_easy_strerror(res));
//	}
//	else {
//		printf("读取完成! 总接收字节数: %zu(%s)\n", chunk.total_received,
//			format_size(chunk.total_received));
//	}
//
//	
//	curl_easy_cleanup(curl);
//	return chunk.memory;
//}

// 静态变量记录上次调用时间
//static time_t last_call_time = 0;
//
//void function_with_delay(double sleep_time) {
//	time_t current_time = time(NULL);
//
//	// 如果是第一次调用，直接记录时间并继续
//	if (last_call_time == 0) {
//		last_call_time = current_time;
//		return;
//	}
//
//	// 计算距离上次调用的时间差
//	double seconds_since_last = difftime(current_time, last_call_time);
//
//	// 如果不足1秒，则等待剩余时间
//	if (seconds_since_last < sleep_time) {
//		double remaining = sleep_time - seconds_since_last;
//		Sleep((unsigned int)remaining); // 转换为整数秒
//	}
//
//	// 更新最后调用时间
//	last_call_time = time(NULL);
//}



/*
	回调函数,将数据加载到内存中
	void* contents,
	size_t size,
	size_t nmemb, 数据块数量（实际数据大小=size*nmemb）
	void* userdata
*/
//static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userdata) {
//	// 计算本次接收的数据总大小
//	size_t contents_size = size * nmemb;
//	struct MemoryStruct* mem = (struct MemoryStruct*)userdata;
//	/*
//	 * 重新分配内存缓冲区：
//	 * 1. 原有大小：mem->size
//	 * 2. 新增大小：contentsSize
//	 * 3. +1 是为了字符串终止符'\0'
//	 */
//	char* ptr = realloc(mem->memory, mem->size + contents_size + 1);
//	if (!ptr) {
//		/* 内存不足时返回0会中止传输 */
//		fprintf(stderr, "Not enough memory (realloc returned NULL)\n");
//		return 0;
//	}
//
//	// 更新内存指针和大小
//	mem->memory = ptr;
//	// 将新数据追加到缓冲区末尾
//	memcpy(&(mem->memory[mem->size]), contents, contents_size);
//	mem->size += contents_size;
//	// 添加字符串终止符（因为 Code forces API返回JSON文本）
//	mem->memory[mem->size] = '\0';
//
//	return contents_size;  // 必须返回实际处理的大小
//}


/*
	api访问并获取 json字符串
	输入:需要访问的 url字符串的指针
	返回值:访问 api得到 json字符串
	提示:需要解析为 json类型
*/

//char* fetch_json_str(char* api_url) {
//	if (!api_url) {
//		fprintf(stderr, "错误: 空URL指针\n");
//		return NULL;
//	}
//
//	CURL* curl = NULL;
//	struct MemoryStruct chunk = { 0 };
//	char* result = NULL;
//	int retry_count = 0;
//
//	do {
//		function_with_delay(2.0);
//		curl = curl_easy_init();
//		if (!curl) {
//			fprintf(stderr, "错误: curl初始化失败\n");
//			break;
//		}
//
//		chunk.memory = malloc(1);
//		if (!chunk.memory) {
//			fprintf(stderr, "错误: 初始内存分配失败\n");
//			break;
//		}
//		chunk.memory[0] = '\0';
//		chunk.size = 0;
//
//		// 设置 curl选项
//		curl_easy_setopt(curl, CURLOPT_URL, api_url);
//		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
//		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
//		curl_easy_setopt(curl, CURLOPT_USERAGENT, "CF-Stats-Tool/1.0.0");
//		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1500L); // 15秒超时
//		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1000L); // 10秒连接超时
//
//		CURLcode res = curl_easy_perform(curl);
//		if (res != CURLE_OK) {
//			fprintf(stderr, "curl请求失败(%d/%d): %s\n",
//				retry_count + 1, MAX_RETRIES, curl_easy_strerror(res));
//
//			free(chunk.memory);
//			curl_easy_cleanup(curl);
//
//			if (retry_count < MAX_RETRIES - 1) {
//				Sleep(RETRY_DELAY); // 等待后重试
//			}
//			continue;
//		}
//
//		long http_code = 0;
//		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
//		if (http_code != 200) {
//			fprintf(stderr, "HTTP错误(%d/%d): %ld\n",
//				retry_count + 1, MAX_RETRIES, http_code);
//
//			free(chunk.memory);
//			curl_easy_cleanup(curl);
//
//			if (http_code == 429) { // 太多请求
//				Sleep(10); // 遇到限制时等待更长时间
//			}
//			else if (retry_count < MAX_RETRIES - 1) {
//				Sleep(RETRY_DELAY);
//			}
//			continue;
//		}
//
//		// 请求成功
//		if (chunk.size > 0 && chunk.memory) {
//			result = chunk.memory;
//		}
//		else {
//			fprintf(stderr, "警告: 获取到空响应\n");
//		}
//		break;
//
//	} while (++retry_count < MAX_RETRIES);
//
//	if (!result && curl) {
//		curl_easy_cleanup(curl);
//	}
//
//	return result;
//}


/*
char* fetch_json_str(char* api_url) {
	if (!api_url) {
		fprintf(stderr, "错误: 空URL指针\n");
		return NULL;
	}

	CURL* curl = curl_easy_init();
	if (!curl) {
		fprintf(stderr, "错误: curl初始化失败\n");
		return NULL;
	}

	struct MemoryStruct chunk = {0};
	chunk.memory = malloc(1);
	if (!chunk.memory) {
		fprintf(stderr, "错误: 初始内存分配失败\n");
		curl_easy_cleanup(curl);
		return NULL;
	}
	chunk.memory[0] = '\0';
	chunk.size = 0;

	// 设置 curl选项
	curl_easy_setopt(curl, CURLOPT_URL, api_url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&chunk);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "CF-Stats-Tool/1.0.0");
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15L); // 15秒超时
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L); // 10秒连接超时

	CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
		fprintf(stderr, "curl请求失败: %s\n", curl_easy_strerror(res));
		free(chunk.memory);
		curl_easy_cleanup(curl);
		return NULL;
	}

	long http_code = 0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
	if (http_code != 200) {
		fprintf(stderr, "HTTP错误: %ld\n", http_code);
		free(chunk.memory);
		curl_easy_cleanup(curl);
		return NULL;
	}

	curl_easy_cleanup(curl);

	// 检查是否获取到有效数据
	if (chunk.size == 0 || !chunk.memory) {
		fprintf(stderr, "错误: 未获取到有效数据\n");
		free(chunk.memory);
		return NULL;
	}

	return chunk.memory;
}

*/




/*
	输入 json字符串并将其转换为 json类型,同时检查数据状态是否正常
	注意  会将原始数据空间释放
*/
cJSON* Convert_json_str_to_JSON(char* json_str) {

	// 1. 解析JSON
	cJSON* json = cJSON_Parse(json_str);
	if (json == NULL) {
		printf("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
		return NULL;
	}

	// 2. 检查API状态
	cJSON* status = cJSON_GetObjectItemCaseSensitive(json, "status");
	if (cJSON_IsString(status) == 0) {
		printf("无效的 status字段\n");
		cJSON_Delete(json);
	}


	// 3. 处理错误状态
	if (strcmp(status->valuestring, "FAILED") == 0) {
		cJSON* comment = cJSON_GetObjectItemCaseSensitive(json, "comment");
		printf("请求失败: %s\n", cJSON_IsString(comment) ? comment->valuestring : "无错误详情");
		cJSON_Delete(json);
	}

	// 4. 验证 result结构
	cJSON* result = cJSON_GetObjectItemCaseSensitive(json, "result");
	if (result == NULL) {
		printf("缺失 result字段\n");
		cJSON_Delete(json);
	}
	free(json_str);//转换为 json后将原始数据空间释放
	return json;
}





/*
	获取指定用户基本信息
	{
  "rating": "当前Rating",
  "maxRating": "历史最高Rating",
  "rank": "当前头衔（如Candidate Master）",
  "maxRank": "最高头衔",
  "registrationTimeSeconds": "注册时间戳"
}
*/
cJSON* fetch_user_info(const char* Handle) {
	printf("正在获取 %s 基本信息...\n", Handle);
	//构造请求 api字符串
	char url[URL_MAX_LENGTH];
	snprintf(url, sizeof(url),
		"https://codeforces.com/api/user.info?handles=%s", Handle);
	

	char* json_str = fetch_json_str(url);
	
	cJSON* json = Convert_json_str_to_JSON(json_str);

	//printf("获取成功\n");
	return json;
}


/*
	读取用户 rating变化(含参赛记录)
	含比赛id,名称,用户排名,用户 raging变化
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
cJSON* fetch_user_rating(const char* Handle) {
	printf("正在获取 %s 比赛数据...\n", Handle);
	//构造请求 api字符串
	char url[URL_MAX_LENGTH];
	snprintf(url, sizeof(url),
		"https://codeforces.com/api/user.rating?handle=%s", Handle);

	char* json_str = fetch_json_str(url);
	cJSON* json = Convert_json_str_to_JSON(json_str);

	//printf("获取成功\n");
	return json;
}


/*
	获取比赛信息:比赛类型,题目列表,参赛者排名
{
  "status": "OK",
  "result": {
	"contest": {...},       // 比赛元信息
	"problems": [...],     // 题目列表
	"rows": [...]          // 排名数据
  }
}
}
(1) 比赛信息 (contest)
{
  "id": 1917,
  "name": "Codeforces Round 920 (Div. 3)",
  "type": "CF",           // 比赛类型（CF/ICPC）
  "phase": "FINISHED",    // 比赛状态
  "frozen": false,        // 是否封榜
  "durationSeconds": 8100 // 比赛时长（秒）
}
(2) 题目列表 (problems)
{
  "contestId": 1917,
  "index": "A",          // 题目编号
  "name": "Problem A",
  "type": "PROGRAMMING", // 题目类型
  "points": 500,         // 分值（动态分数赛制）
  "rating": 800,         // 题目难度
  "tags": ["implementation"]
}

(3) 排名数据 (rows)
{
  "party": {             // 参赛者信息
	"handle": "tourist",
	"members": [...]
  },
  "rank": 1,             // 最终排名
  "points": 2500,        // 总分
  "penalty": 120,        // 罚时（分钟）
  "problemResults": [    // 每题结果
	{
	  "points": 500,     // 本题得分
	  "rejectedAttemptCount": 2, // 错误提交次数
	  "bestSubmissionTimeSeconds": 1234 // 首次AC时间
	}
  ]
}
	*/
cJSON* fetch_contest_standings(int contest_id, const char* Handle) {
	
	
	char url[URL_MAX_LENGTH];
	if (Handle == NULL) {
		printf("正在获取id为 %d 的比赛数据...\n", contest_id);
		snprintf(url, sizeof(url),
			"https://codeforces.com/api/contest.standings?contestId=%d&showUnofficial=true&from=1&count=1",
			contest_id);
	}
	else {
		printf("正在获取 %s 在id为 %d 的比赛数据...\n", Handle, contest_id);
		snprintf(url, sizeof(url),
			"https://codeforces.com/api/contest.standings?contestId=%d&handles=%s&showUnofficial=true",
			contest_id, Handle);
	}
	
	char* json_str = fetch_json_str(url);
	
	
	cJSON* json = Convert_json_str_to_JSON(json_str);
	if (json == NULL) {
		printf("获取失败\n");
		return NULL;
	}
	//printf("获取成功\n");
	return json;
}


/*
	获取用户所有提交记录 每题得分
	{
		"status": "OK",
		"result": [
			{
				"id": 256790001,
				"contestId": 1917,
				"problem": {
				"contestId": 1917,
				"index": "A",
				"name": "Problem A",
				"points": 500,           // 题目分值（如有）
				"rating": 800            // 题目难度（如有）
				},
				"programmingLanguage": "GNU C++20",
				"verdict": "OK",           // 判题结果（AC/WA等）
				"testset": "TESTS",
				"passedTestCount": 12,
				"timeConsumedMillis": 123,
				"memoryConsumedBytes": 204800,
				"creationTimeSeconds": 1704984000
			}
		]
	}

*/
cJSON* fetch_contest_status(int contest_id, const char* Handle) {
	printf("正在获取 %s 在id为 %d 的比赛数据...\n", Handle, contest_id);
	//构造请求 api字符串
	char url[URL_MAX_LENGTH];
	snprintf(url, sizeof(url),
		"https://codeforces.com/api/contest.status?contestId=%d&handle=%s&showUnofficial=false",
		contest_id, Handle);

	char* json_str = fetch_json_str(url);
	cJSON* json = Convert_json_str_to_JSON(json_str);

	//printf("获取成功\n");
	return json;
}



/*

	获取所有比赛列表
 {
  "status": "OK",
  "result": [
	{
	  "id": 1917,
	  "name": "Codeforces Round 920 (Div. 3)",
	  "type": "CF",               // 比赛类型：CF（常规赛）/ICPC
	  "phase": "FINISHED",        // 比赛状态
	  "frozen": false,            // 是否封榜
	  "durationSeconds": 8100,   // 比赛时长（秒）
	  "startTimeSeconds": 1704983700, // 开始时间（Unix时间戳）
	  "relativeTimeSeconds": -12345678, // 当前时间与比赛时间的相对差值
	  "preparedBy": "MikeMirzayanov" // 比赛组织者（可选）
	},
	// 更多比赛...
  ]
}

*/
cJSON* fetch_contest_list() {
	printf("正在获取比赛列表...\n");
	//构造请求 api字符串
	char url[URL_MAX_LENGTH];
	snprintf(url, sizeof(url),
		"https://codeforces.com/api/contest.list?gym=false");

	char* json_str = fetch_json_str(url);
	cJSON* json = Convert_json_str_to_JSON(json_str);

	//printf("获取成功\n");
	return json;
}


/*
	获取题目标签（知识点）
	{
	  "status": "OK",
	  "result": {
		"problems": [
		  {
			"contestId": 1917,
			"index": "A",
			"name": "Problem A",
			"type": "PROGRAMMING",
			"points": 500,          // 题目分值（可能不存在）
			"rating": 800,          // 题目难度（可能不存在）
			"tags": ["implementation", "brute force"]
		  },
		  // 更多题目...
		],
		"problemStatistics": [
		  {
			"contestId": 1917,
			"index": "A",
			"solvedCount": 2563     // 通过人数
		  }
		]
	  }
}
*/
cJSON* fetch_problemset_problems() {
	printf("正在获取题目标签...\n");
	//构造请求 api字符串
	char url[URL_MAX_LENGTH];
	snprintf(url, sizeof(url),
		"https://codeforces.com/api/problemset.problems?tags=implementation");

	char* json_str = fetch_json_str(url);
	cJSON* json = Convert_json_str_to_JSON(json_str);

	//printf("获取成功\n");
	return json;
}