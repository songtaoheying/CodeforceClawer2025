#include"xiong.h"

int main() {
	char handle[50];
	printf("请输入 handle:\n");
    scanf_s("%s", handle, (unsigned)_countof(handle));
	
	//1.	读取用户基本信息
	cJSON* json_user_info = fetch_user_info(handle);
		//save_cJSON_to_file("user_info.txt", json_user_info);
	generate_info_report(json_user_info);
	

	//2.	读取一年内的比赛列表，过滤出Div1，2，3，4的比赛
	cJSON* json_origin_contest_list = fetch_contest_list();
	cJSON* json_filtered_contest_list = filter_contests(json_origin_contest_list);
	//save_cJSON_to_file("all_contest_list.txt", json_filtered_contest_list);
	generate_contest_list(json_filtered_contest_list);
	

	//3.	读取用户参加比赛的列表，过滤出Div1，2，3，4的比赛
	cJSON* json_user_rating = fetch_user_rating(handle);
	cJSON* json_user_contest_list = filter_user_contests(json_user_rating);
	//save_cJSON_to_file("user_contest_list.txt", json_user_contest_list);
	generate_user_contest_list(json_user_contest_list, handle);
	

	//4.	读取用户参加比赛的排名，各题的分数，获得分数，Rating的变化情况 standing.handle .id      rating
	cJSON* json_user_detail = get_user_contest_detail(handle, json_user_contest_list);
	
	//save_cJSON_to_file("user_contest_detail.txt", json_user_detail);
	generate_user_performance(json_user_contest_list, json_user_detail);
	

	//5.	统计和展示用户参加各级比赛的次数，参赛次数的月度分布，出勤率
	struct user_attend_contest_information information =
		user_attend_contest_information_calculate(json_user_contest_list, json_filtered_contest_list);
	cJSON* json_user_contest_information = convert_struct_to_json(information, handle);
	//save_cJSON_to_file("json_user_contest_information.txt", json_user_contest_information);
	//save_user_contest_detail(information, handle);
	generate_user_information(json_user_contest_information);

	
	//6.	展示所有参赛的比赛的级别，名称，时间，排名，题目得分情况，rating的变化，头衔的变化
		//展示4中的数据 rating    standing
	generate_user_contest_summary(json_user_contest_list, json_user_detail);
	
	
	//7.	统计全年各级比赛各题的通过准确性，题目分值的分布（比如最低，最高，均值，方差）等信息，并使用合适的图例展示
		//standing
	//json_user_detail
	generate_analyze_contest_problem_statistics(json_user_detail);

	cJSON_Delete(json_user_detail);////////////////////////////////////////
	cJSON_Delete(json_filtered_contest_list);
	cJSON_Delete(json_origin_contest_list);
	cJSON_Delete(json_user_info);
	cJSON_Delete(json_user_contest_list);         
	cJSON_Delete(json_user_contest_information);  

	printf("按Enter键退出...");
	while (getchar() != '\n');//清除缓冲区
	while (getchar() != '\n');
	return 0;
}
