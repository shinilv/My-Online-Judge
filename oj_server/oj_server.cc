#include <iostream>
#include "../common/httplib.h"
#include "../common/log.hpp"
#include "oj_control.hpp"


using namespace ns_log;
using namespace httplib;
using namespace ns_control;

int main(int argc, char* argv[]) {

    // 测试 分隔符

    // std::string str = "1 判断回文数 简单 1 300000";
    // std::vector<std::string> result;
    // StringUtil::SplitString(str, " ", &result);
    // for (auto& x : result) {
    //     std::cout << x << '\n';
    // }

    if (argc != 2) {
        LOG(ERROR) << "参数不正确" << std::endl;
        return 1;
    }
    // 用户请求的服务路由功能
    Server svr;

    Control ctrl;



    // 获取所有的题目列表，用户要根据题目编号
    svr.Get("/all_questions", [&ctrl](const Request& req, Response& resp) {
        // 获取所有题目列表
        std::string html;
        ctrl.AllQuestions(&html);
        
        resp.set_content(html, "text/html; charset=utf-8");
    });

    // 根据题目编号， 获取题目内容, question/100, 
    // 正则匹配， R("") 原始字符串， 保持字符串内容原貌， 不需要相关转义
    svr.Get(R"(/question/(\d+))", [&ctrl](const Request& req, Response& resp) {
        // 正则匹配到的内容在 matches[1]中
        std::string number = req.matches[1];
        std::string html;
        ctrl.Question(number, html);
        resp.set_content(html, "text/html; charset=utf-8");
    });

    // 用户提交代码， 使用我们的判题功能
    svr.Get(R"(/judge/(\d+))", [](const Request& req, Response& resp) {
        // 正则匹配到的内容在 matches[1]中
        std::string number = req.matches[1];
        resp.set_content("指定题目的判题: " + number, "text/plain; charset=utf-8");
    });
    svr.set_base_dir("./wwwroot");


    svr.listen("0.0.0.0", atoi(argv[1])); // 启动http服务

    return 0;
}