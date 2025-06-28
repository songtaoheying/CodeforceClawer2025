#include "xiong.h"
#pragma once
struct user_attend_contest_information {
    double attendance_rate;
    int user_attend_div1;
    int user_attend_div2;
    int user_attend_div3;
    int user_attend_div4;
    int eligible_contest;
    int attend_contest;
    int maxRating;
    int month[13];
    int sum_div1;
    int sum_div2;
    int sum_div3;
    int sum_div4;
};
//cJSON* filter_contests(cJSON* api_response);
//cJSON* filter_user_contests(cJSON* root);
//cJSON* get_user_contest_detail(char *user_name, cJSON* user_contest_list);//获取所有参赛的比赛的级别，名称，时间，用户排名
//struct user_attend_contest_information user_attend_contest_information_calculate(cJSON* user_attend_contest, cJSON* all_contest);
//void calculate_user_attend_information(struct user_attend_contest_information* information, cJSON* user_attend_contest);
//void calculate_all_contest_information(struct user_attend_contest_information* information, cJSON* all_contest);
//void eligible_cont(struct user_attend_contest_information* information);
//int is_the_past_12_month(int TimeSeconds);
//cJSON* json_filter_array(cJSON* origin_array, char* labelName, char* filterStr);
//cJSON* assemble_cJSON_array_to_object(cJSON* cJSON_array);
//cJSON* get_all_contest_statistics(cJSON* contest_list);
//cJSON* convert_struct_to_json(struct user_attend_contest_information information, const char* user_name);
//void saveStringToFile(const char* filename, const char* text);
//void save_cJSON_to_file(const char* saveFileName, cJSON* response);
//void save_user_contest_detail(struct user_attend_contest_information information, const char* user_name);


/**
 * @brief 过滤API响应中的比赛信息。
 * 
 * @param api_response 包含原始比赛信息的cJSON对象。
 * @return cJSON* 过滤后的包含比赛信息的cJSON对象。
 */
cJSON* filter_contests(cJSON* api_response);

/**
 * @brief 从JSON根对象中过滤用户比赛信息。
 * 
 * @param root 包含所有用户比赛信息的cJSON对象。
 * @return cJSON* 过滤后的仅包含用户比赛信息的cJSON对象。
 */
cJSON* filter_user_contests(cJSON* root);

/**
 * @brief 获取指定用户参加的比赛的详细信息，包括级别、名称、时间及用户排名。
 * 
 * @param user_name 用户名。
 * @param user_contest_list 用户参加的比赛列表的cJSON对象。
 * @return cJSON* 包含用户参加的比赛详细信息的cJSON对象。
 */
cJSON* get_user_contest_detail(char *user_name, cJSON* user_contest_list);

/**
 * @brief 计算用户参加比赛的信息。
 * 
 * @param user_attend_contest 用户参加的比赛信息的cJSON对象。
 * @param all_contest 所有比赛信息的cJSON对象。
 * @return struct user_attend_contest_information 计算后的用户参加比赛信息结构体。
 */
struct user_attend_contest_information user_attend_contest_information_calculate(cJSON* user_attend_contest, cJSON* all_contest);

/**
 * @brief 计算用户参加比赛的具体信息。
 * 
 * @param information 用于存储计算结果的user_attend_contest_information结构体指针。
 * @param user_attend_contest 用户参加的比赛信息的cJSON对象。
 */
void calculate_user_attend_information(struct user_attend_contest_information* information, cJSON* user_attend_contest);

/**
 * @brief 计算所有比赛的相关信息。
 * 
 * @param information 用于存储计算结果的user_attend_contest_information结构体指针。
 * @param all_contest 所有比赛信息的cJSON对象。
 */
void calculate_all_contest_information(struct user_attend_contest_information* information, cJSON* all_contest);

/**
 * @brief 过滤出用户在过去12个月中参加的符合条件的比赛。
 * 
 * @param information 用户参加比赛信息的user_attend_contest_information结构体指针。
 */
void eligible_cont(struct user_attend_contest_information* information);

/**
 * @brief 检查给定的时间是否在过去12个月之内。
 * 
 * @param TimeSeconds 以秒为单位的时间戳。
 * @return int 如果时间在过去12个月之内，返回1；否则返回0。
 */
int is_the_past_12_month(int TimeSeconds);

/**
 * @brief 根据标签名称和过滤字符串过滤JSON数组。
 * 
 * @param origin_array 原始的cJSON数组对象。
 * @param labelName 标签名，用于过滤的标签。
 * @param filterStr 用于过滤的字符串。
 * @return cJSON* 过滤后的cJSON数组对象。
 */
cJSON* json_filter_array(cJSON* origin_array, char* labelName, char* filterStr);

/**
 * @brief 将cJSON数组组装成一个cJSON对象。
 * 
 * @param cJSON_array 需要组装的cJSON数组对象。
 * @return cJSON* 组装后的cJSON对象。
 */
cJSON* assemble_cJSON_array_to_object(cJSON* cJSON_array);

/**
 * @brief 获取所有比赛的统计信息。
 * 
 * @param contest_list 包含所有比赛信息的cJSON对象。
 * @return cJSON* 包含比赛统计信息的cJSON对象。
 */
cJSON* get_all_contest_statistics(cJSON* contest_list);

/**
 * @brief 将用户参加比赛的信息结构体转换为JSON对象。
 * 
 * @param information 用户参加比赛的信息结构体。
 * @param user_name 用户名。
 * @return cJSON* 转换后的包含用户比赛信息的cJSON对象。
 */
cJSON* convert_struct_to_json(struct user_attend_contest_information information, const char* user_name);

/**
 * @brief 将字符串保存到文件。
 * 
 * @param filename 文件名。
 * @param text 需要保存的字符串。
 */
void saveStringToFile(const char* filename, const char* text);

/**
 * @brief 将cJSON对象保存到文件。
 * 
 * @param saveFileName 文件名。
 * @param response 需要保存的cJSON对象。
 */
void save_cJSON_to_file(const char* saveFileName, cJSON* response);

/**
 * @brief 保存用户参加比赛的详细信息到文件。
 * 
 * @param information 用户参加比赛的信息结构体。
 * @param user_name 用户名。
 */
void save_user_contest_detail(struct user_attend_contest_information information, const char* user_name);
