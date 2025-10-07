#pragma once

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
#include "oj_model.hpp"
#include <cassert>
#include <fstream>

// 逻辑控制

namespace ns_control {
    using namespace ns_log;
    using namespace ns_model;
    using namespace ns_util;
    class Control {
    private:
        Model _model;
    
    public:
        Control() {

        }
        // 根据题目数据构建网页， 输出型参数
        bool AllQuestions(std::string *html) {
            std::vector<Question> all;
            if (_model.GetAllQuestions(&all)) {
                // 获取题目信息成功

            } else {

            }
        }

        bool Question(const std::string& number, std::string* html) {
            Question question;
            if (_model.GetOneQuestion(number, &question)) {
                // 获取一个题目信息成功
                
            } else {

            }
        }

        ~Control() {}
        
    };
}