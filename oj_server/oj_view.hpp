#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <ctemplate/template.h>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <algorithm>
#include "oj_model.hpp"

namespace ns_view {
    using namespace ns_model;

    // struct Question {
    //     std::string number; // 题目编号
    //     std::string title; // 题目标题
    //     std::string difficulty; // 题目难度
    //     int cpu_limit; // 题目CPU限制
    //     int mem_limit; // 题目内存限制
    //     std::string desc; // 题目描述
    //     std::string header; // 题目预设置的代码
    //     std::string tail; // 题目测试用例代码
    // };
    
    class View {
    public:
        View() {}

        void AllExpandHtml(std::vector<Question>& questions, std::string* html) {
            // 设置模板根目录
            ctemplate::Template::SetTemplateRootDirectory("./template/");
            
            // 创建模板字典
            ctemplate::TemplateDictionary dict("all_questions");
            
            // 添加题目数据
            for (const auto& question : questions) {
                auto* question_dict = dict.AddSectionDictionary("QUESTIONS");
                question_dict->SetValue("NUMBER", question.number);
                question_dict->SetValue("TITLE", question.title);
                question_dict->SetValue("DIFFICULTY", question.difficulty);
                
                // 根据难度设置CSS类
                std::string difficulty_class;
                if (question.difficulty == "简单") {
                    difficulty_class = "easy";
                } else if (question.difficulty == "中等") {
                    difficulty_class = "medium";
                } else if (question.difficulty == "困难") {
                    difficulty_class = "hard";
                } else {
                    difficulty_class = "easy";
                }
                question_dict->SetValue("DIFFICULTY_CLASS", difficulty_class);
                
                question_dict->SetValue("CPU_LIMIT", std::to_string(question.cpu_limit));
                question_dict->SetValue("MEM_LIMIT", std::to_string(question.mem_limit));
            }
            
            // 渲染模板
            ctemplate::ExpandTemplate("all_questions.html", ctemplate::DO_NOT_STRIP, &dict, html);
        }

        void OneExpandHtml(const Question& q, std::string* html) {
            // 设置模板根目录
            ctemplate::Template::SetTemplateRootDirectory("./template/");
            
            // 创建模板字典
            ctemplate::TemplateDictionary dict("one_question");
            
            // 设置题目数据
            dict.SetValue("NUMBER", q.number);
            dict.SetValue("TITLE", q.title);
            dict.SetValue("DIFFICULTY", q.difficulty);
            dict.SetValue("DESC", q.desc);
            dict.SetValue("HEADER", q.header);
            dict.SetValue("CPU_LIMIT", std::to_string(q.cpu_limit));
            dict.SetValue("MEM_LIMIT", std::to_string(q.mem_limit));
            
            // 设置测试输入（从tail.cpp中提取）
            // dict.SetValue("TEST_INPUT", q.tail);
            
            // 根据难度设置CSS类
            std::string difficulty_class;
            if (q.difficulty == "简单") {
                difficulty_class = "easy";
            } else if (q.difficulty == "中等") {
                difficulty_class = "medium";
            } else if (q.difficulty == "困难") {
                difficulty_class = "hard";
            } else {
                difficulty_class = "easy";
            }
            dict.SetValue("DIFFICULTY_CLASS", difficulty_class);
            
            // 渲染模板
            ctemplate::ExpandTemplate("one_question.html", ctemplate::DO_NOT_STRIP, &dict, html);
        }

        ~View() {}
    };
}