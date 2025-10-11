#pragma once

// 根据题目list文件， 加载所有的题目信息
// model：主要用来和数据库交互， 对外提供访问数据的接口
// mysql版本



#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <algorithm>
#include "../common/log.hpp"
#include "../common/util.hpp"
#include <cassert>
#include <fstream>

#include "include/mysql.h"

namespace ns_model {
    using namespace ns_log;
    using namespace ns_util;


    struct Question {
        std::string number; // 题目编号
        std::string title; // 题目标题
        std::string difficulty; // 题目难度
        std::string desc; // 题目描述
        std::string header; // 题目预设置的代码
        std::string tail; // 题目测试用例代码
        std::string test; // 题目运行代码
        int cpu_limit; // 题目CPU限制
        int mem_limit; // 题目内存限制
    };
    // 表名
    const std::string oj_questions = "oj_questios";
    const std::string host = "127.0.0.1";
    const std::string user = "oj_client";
    const std::string passwd = "123456";
    const std::string db = "oj";
    const int port = 3306;
    
    class Model {
    public:
        Model() {
        }
        ~Model() {}

        bool QueryMySql(const std::string& sql, std::vector<Question>* out) {
            MYSQL *my = mysql_init(nullptr);
            if (my == nullptr) {
                LOG(FATAL) << "初始化 MySQL 失败" << std::endl;
                return false;
            }
        
            // 链接数据库
            if (nullptr == mysql_real_connect(my, host.c_str(), user.c_str(), passwd.c_str(), db.c_str(), port, nullptr, 0)) {
                LOG(FATAL) << "链接数据库失败: " << mysql_error(my) << std::endl;
                mysql_close(my);
                return false;
            }
        
            LOG(INFO) << "链接数据库成功" << std::endl;
            
            // 设置字符集，防止中文乱码
            mysql_set_character_set(my, "utf8");
        
            if (mysql_query(my, sql.c_str()) != 0) {
                LOG(WARNING) << sql << " execute error: " << mysql_error(my) << std::endl;
                mysql_close(my);
                return false;
            }
        
            // 提取结果
            MYSQL_RES *res = mysql_store_result(my);
            if (res == nullptr) {
                // 可能是没有结果集的查询（如 INSERT/UPDATE/DELETE）
                if (mysql_field_count(my) == 0) {
                    LOG(INFO) << "查询执行成功，无结果集返回" << std::endl;
                } else {
                    LOG(WARNING) << "获取结果集失败: " << mysql_error(my) << std::endl;
                }
                mysql_close(my);
                return true; // 查询执行成功，只是没有结果集
            }
        
            int rows = mysql_num_rows(res); // 行数
            int cols = mysql_num_fields(res); // 列数
            
            LOG(INFO) << "查询结果: " << rows << " 行, " << cols << " 列" << std::endl;
        
            // 检查列数是否符合预期
            if (cols < 9) {
                LOG(WARNING) << "结果集列数不足，期望至少9列，实际" << cols << "列" << std::endl;
                mysql_free_result(res);
                mysql_close(my);
                return false;
            }
        
            MYSQL_ROW row;
            
            // 遍历提取结果
            for (int i = 0; i < rows; i++) {
                row = mysql_fetch_row(res);
                if (row == nullptr) {
                    LOG(WARNING) << "获取第 " << i << " 行数据失败" << std::endl;
                    break;
                }
        
                Question q;
                
                // 安全地处理每一列数据，检查是否为 NULL
                q.number = row[0] ? row[0] : "";
                q.title = row[1] ? row[1] : "";
                q.difficulty = row[2] ? row[2] : "";
                q.desc = row[3] ? row[3] : "";
                q.header = row[4] ? row[4] : "";
                q.tail = row[5] ? row[5] : "";
                q.test = row[6] ? row[6] : "";
                
                // 数值类型的安全转换
                q.cpu_limit = row[7] ? atoi(row[7]) : 0;
                q.mem_limit = row[8] ? atoi(row[8]) : 0;
        
                out->push_back(q);
            }
            
            mysql_free_result(res); // 使用正确的释放函数
            mysql_close(my);
            return true;
        }
        // 查询所有题目
        bool GetAllQuestions(std::vector<Question>* questions) {
            std::string sql = "select * from ";
            sql += oj_questions;
            sql += ";";
            return QueryMySql(sql, questions);
        }
        // 查询单个题目
        bool GetOneQuestion(std::string& number, Question* question) {
            std::string sql = "select * from ";
            sql += oj_questions;
            sql += " where number=";
            sql += number;
            sql += ";";
            std::vector<Question> result;
            if(QueryMySql(sql, &result)) {
                *question = result[0];
                return true;
            } else {
                return false;
            }
        }
    };


}