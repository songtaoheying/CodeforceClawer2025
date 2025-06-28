#pragma once
#include"xiong.h"
struct MemoryStruct {
	char* memory;
	size_t size;
	size_t total_received; // 记录总接收字节数
};
//const char* format_size(size_t bytes);
//static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userdata);
//char* fetch_json_str(char* url);
//cJSON* Convert_json_str_to_JSON(char* json_str);
//cJSON* fetch_user_info(const char* Handle);
//cJSON* fetch_user_rating(const char* Handle);
//cJSON* fetch_contest_standings(int contest_id, const char* Handle);
//cJSON* fetch_contest_status(int contest_id, const char* Handle);
//cJSON* fetch_contest_list();
//cJSON* fetch_problemset_problems();
// 

/**
 * @struct MemoryStruct
 * @brief 用于存储HTTP请求返回内容的结构体
 *
 * @field memory 指向存储返回内容的内存地址
 * @field size 当前存储的字节数
 * @field total_received 记录接收到的总字节数
 */

/**
 * @function format_size
 * @brief 将字节数格式化为人类可读的字符串形式
 *
 * @param bytes 要格式化的字节数
 * @return 格式化后的字符串，例如 "1.23 KB"
 */
const char* format_size(size_t bytes);

/**
 * @function WriteMemoryCallback
 * @brief 回调函数，用于将HTTP请求的内容写入到MemoryStruct结构体中
 *
 * @param contents 从HTTP响应中接收到的数据块
 * @param size 数据块的大小（字节数）
 * @param nmemb 数据块的数量
 * @param userdata 指向MemoryStruct的指针，用于存储接收到的数据
 * @return 实际写入的字节数
 */
static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userdata);

/**
 * @function fetch_json_str
 * @brief 从指定URL获取JSON格式的字符串
 *
 * @param url 请求的URL地址
 * @return 获取到的JSON字符串，如果请求失败则返回NULL
 */
char* fetch_json_str(char* url);

/**
 * @function Convert_json_str_to_JSON
 * @brief 将JSON格式的字符串转换为cJSON对象
 *
 * @param json_str JSON格式的字符串
 * @return 转换后的cJSON对象，如果转换失败则返回NULL
 */
cJSON* Convert_json_str_to_JSON(char* json_str);

/**
 * @function fetch_user_info
 * @brief 根据用户句柄获取用户信息
 *
 * @param Handle 用户的句柄
 * @return 包含用户信息的cJSON对象，如果请求失败则返回NULL
 */
cJSON* fetch_user_info(const char* Handle);

/**
 * @function fetch_user_rating
 * @brief 根据用户句柄获取用户评分信息
 *
 * @param Handle 用户的句柄
 * @return 包含用户评分信息的cJSON对象，如果请求失败则返回NULL
 */
cJSON* fetch_user_rating(const char* Handle);

/**
 * @function fetch_contest_standings
 * @brief 根据比赛ID和用户句柄获取比赛排名信息
 *
 * @param contest_id 比赛的ID
 * @param Handle 用户的句柄
 * @return 包含比赛排名信息的cJSON对象，如果请求失败则返回NULL
 */
cJSON* fetch_contest_standings(int contest_id, const char* Handle);

/**
 * @function fetch_contest_status
 * @brief 根据比赛ID和用户句柄获取比赛状态信息
 *
 * @param contest_id 比赛的ID
 * @param Handle 用户的句柄
 * @return 包含比赛状态信息的cJSON对象，如果请求失败则返回NULL
 */
cJSON* fetch_contest_status(int contest_id, const char* Handle);

/**
 * @function fetch_contest_list
 * @brief 获取比赛列表信息
 *
 * @return 包含比赛列表信息的cJSON对象，如果请求失败则返回NULL
 */
cJSON* fetch_contest_list();

/**
 * @function fetch_problemset_problems
 * @brief 获取问题集中的问题信息
 *
 * @return 包含问题集问题信息的cJSON对象，如果请求失败则返回NULL
 */
cJSON* fetch_problemset_problems();

