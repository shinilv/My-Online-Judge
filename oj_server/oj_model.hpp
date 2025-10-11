// #pragma once

// // 根据题目list文件， 加载所有的题目信息
// // model：主要用来和数据库交互， 对外提供访问数据的接口

// #include <iostream>
// #include <string>
// #include <vector>
// #include <map>
// #include <set>
// #include <unordered_map>
// #include <unordered_set>
// #include <queue>
// #include <stack>
// #include <algorithm>
// #include "../common/log.hpp"
// #include "../common/util.hpp"
// #include <cassert>
// #include <fstream>

// namespace ns_model {
//     using namespace ns_log;
//     using namespace ns_util;


//     struct Question {
//         std::string number; // 题目编号
//         std::string title; // 题目标题
//         std::string difficulty; // 题目难度
//         std::string desc; // 题目描述
//         std::string header; // 题目预设置的代码
//         std::string tail; // 题目测试用例代码
//         std::string test; // 题目运行代码
//         int cpu_limit; // 题目CPU限制
//         int mem_limit; // 题目内存限制
//     };
//     const std::string question_list_path = "question/questions.list";
//     const std::string question_path = "question/";
    
//     class Model {
//     private:
//         std::unordered_map<std::string, Question> question_map;
//     public:
//         Model() {
//             assert(LoadQuestionList(question_list_path));
//         }
//         ~Model() {}
        
        
//         bool LoadQuestionList(const std::string& question_list) {
//             // 加载配置文件
//             std::ifstream ifs(question_list);
//             if (!ifs.is_open()) {
//                 LOG(ERROR) << "文件打开失败" << std::endl;
//                 return false;
//             }
//             std::string line;
//             // 按行读取文件
//             while (std::getline(ifs, line)) {
//                 std::vector<std::string> result;
//                 ns_util::StringUtil::SplitString(line, " ", &result);
//                 if (result.size() != 5) {
//                     LOG(WARNING) << "读取出现问题" << std::endl;
//                     continue;
//                 }
//                 Question question;
//                 question.number = result[0];
//                 question.title = result[1];
//                 question.difficulty = result[2];
//                 question.cpu_limit = std::stoi(result[3]);
//                 question.mem_limit = std::stoi(result[4]);  
//                 std::string question_number_path = question_path;
//                 question_number_path += question.number;
//                 question_number_path += "/";

//                 FileUtil::ReadFile(question_number_path + "desc.txt", &(question.desc), true);
//                 FileUtil::ReadFile(question_number_path + "header.cpp", &(question.header), true);
//                 FileUtil::ReadFile(question_number_path + "tail.cpp", &(question.tail), true);
//                 FileUtil::ReadFile(question_number_path + "test.cpp", &(question.test), true);
                


//                 question_map[question.number] = question;
//             }
//             ifs.close();
//             return true;
//         }
//         // 查询所有题目
//         bool GetAllQuestions(std::vector<Question>* questions) {
//             if (questions == nullptr) {
//                 LOG(ERROR) << "题目列表为空" << std::endl;
//                 return false;
//             }
//             for (auto& [number, question] : question_map) {
//                 questions->push_back(question);
//             }
//             LOG(INFO) << "加载题库成功" << std::endl;

//             return true;
//         }
//         // 查询单个题目
//         bool GetOneQuestion(const std::string& number, Question* question) {
//             const auto& iter = question_map.find(number);
//             if (iter == question_map.end()) {
//                 LOG(ERROR) << "获取题目失败" << std::endl;
//                 return false;
//             }
//             *question = iter->second;
//             LOG(INFO) << "获取题目成功" << std::endl;
//             return true;
//         }
//     };


// }