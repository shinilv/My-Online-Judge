#include <iostream>
#include "../common/httplib.h"
#include "../common/log.hpp"

using namespace ns_log;
using namespace httplib;

int main(int argc, char* argv[]) {

    if (argc != 2) {
        LOG(ERROR) << "参数不正确" << std::endl;
        return 1;
    }
    // 用户请求的服务路由功能
    Server svr;

    // 获取所有的题目列表，用户要根据题目编号
    svr.Get("/all_questions", [](const Request& req, Response& resp) {
        resp.set_content("这是所有的题目列表", "text/plain; charset=utf-8");
    });

    // 根据题目编号， 获取题目内容, question/100, 
    // 正则匹配， R("") 原始字符串， 保持字符串内容原貌， 不需要相关转义
    svr.Get(R"(/question/(\d+))", [](const Request& req, Response& resp) {
        // 正则匹配到的内容在 matches[1]中
        std::string number = req.matches[1];
        resp.set_content("这是一道题: " + number, "text/plain; charset=utf-8");
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