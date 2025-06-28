#include "xiong.h"
#pragma once
//void generate_info_report(cJSON *root);
//void get_file_path(char *buffer, int size);
//void run_file(const char* path);
//void generate_contest_list(cJSON* root);
//void generate_user_contest_list(cJSON* root, char* handle);
//void generate_user_performance(cJSON* list, cJSON* detail);
//void generate_user_information(cJSON* root);
//void generate_user_contest_summary(cJSON* list, cJSON* detail);
//void generate_analyze_contest_problem_statistics(cJSON* root);



/**
 * @brief 生成信息报告
 * 
 * @param root 指向cJSON对象的指针，该对象作为JSON报告的根节点
 * 
 * @return void
 * 
 * @details 该函数根据传入的cJSON对象root，生成一个包含信息的报告，并将其存储在该对象中。
 */
void generate_info_report(cJSON *root);

/**
 * @brief 获取文件路径
 * 
 * @param buffer 用于存储文件路径的字符数组
 * @param size   buffer数组的大小，以确保不发生缓冲区溢出
 * 
 * @return void
 * 
 * @details 该函数将文件路径写入到buffer数组中，供后续调用使用。size参数确保了buffer有足够的空间来存储路径。
 */
void get_file_path(char *buffer, int size);

/**
 * @brief 运行指定路径的文件
 * 
 * @param path 指向C字符串的指针，表示要运行的文件路径
 * 
 * @return void
 * 
 * @details 该函数接收一个文件路径作为参数，并运行该路径下的文件。
 */
void run_file(const char* path);

/**
 * @brief 生成竞赛列表
 * 
 * @param root 指向cJSON对象的指针，该对象作为JSON报告的根节点
 * 
 * @return void
 * 
 * @details 该函数生成一个竞赛列表，并将其存储在传入的cJSON对象root中。
 */
void generate_contest_list(cJSON* root);

/**
 * @brief 生成指定用户的竞赛列表
 * 
 * @param root   指向cJSON对象的指针，该对象作为JSON报告的根节点
 * @param handle 表示用户标识的C字符串
 * 
 * @return void
 * 
 * @details 该函数根据用户的handle生成一个竞赛列表，并将其存储在传入的cJSON对象root中。
 */
void generate_user_contest_list(cJSON* root, char* handle);

/**
 * @brief 生成用户的竞赛表现详情
 * 
 * @param list   指向cJSON对象的指针，该对象包含竞赛列表信息
 * @param detail 指向cJSON对象的指针，该对象用于存储用户的竞赛表现详情
 * 
 * @return void
 * 
 * @details 该函数生成用户的竞赛表现详情，并将其存储在detail对象中，该对象通常会被添加到list对象中。
 */
void generate_user_performance(cJSON* list, cJSON* detail);

/**
 * @brief 生成用户信息
 * 
 * @param root 指向cJSON对象的指针，该对象作为JSON报告的根节点
 * 
 * @return void
 * 
 * @details 该函数生成用户的基本信息，并将其存储在传入的cJSON对象root中。
 */
void generate_user_information(cJSON* root);

/**
 * @brief 生成用户的竞赛总结
 * 
 * @param list   指向cJSON对象的指针，该对象包含竞赛列表信息
 * @param detail 指向cJSON对象的指针，该对象用于存储用户的竞赛总结信息
 * 
 * @return void
 * 
 * @details 该函数生成用户的竞赛总结信息，并将其存储在detail对象中，该对象通常会被添加到list对象中。
 */
void generate_user_contest_summary(cJSON* list, cJSON* detail);

/**
 * @brief 分析并生成竞赛问题统计信息
 * 
 * @param root 指向cJSON对象的指针，该对象作为JSON报告的根节点
 * 
 * @return void
 * 
 * @details 该函数分析竞赛中的问题，并生成相关的统计信息，将其存储在传入的cJSON对象root中。
 */
void generate_analyze_contest_problem_statistics(cJSON* root);
