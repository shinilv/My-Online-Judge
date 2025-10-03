#include "compile_run.hpp"
#include "../common/httplib.h"

using namespace ns_compile_and_run;
using namespace httplib;

// 编译服务随时可能被多个人请求，必须保证传递上来的code，形成源文件名称的时候， 要具有
// 唯一性， 多个用户会相互影响

int main() {
    // http 让client给我们 上传一个json string

    Server svr;
    
    svr.Get("/hello", [](const Request& req, Response& res) {
        res.set_content("Hello World!, 你好", "text/plain; charset=utf-8");
    });

    // 参数 in_json: {"code": "#incldue...", "input": "", "cpu_limit":1, "mem_limit": 10240}
    // out_json: {"status":"0", "reason":"", "stdout":"", stderr:""}
    svr.Post("/compile_and_run", [](const Request& req, Response& resp) {
        // 用户请求的服务正文是我们想要的json
        std::string in_json = req.body, out_json;
        if (!in_json.empty()) {
			// std::cout << in_json << std::endl;
            CompileAndRun::Start(in_json, &out_json);
            // std::cout << in_json << std::endl;
            // std::cout << out_json << std::endl;
            resp.set_content(out_json, "application/json;charset=utf-8");
        }
     });

    svr.listen("0.0.0.0", 9090); // 启动http服务













//     // 阶段测试代码

//     std::setlocale(LC_ALL, "en_US.UTF-8");

//     // 模拟客户端进行测试
//     std::string in_json;
//     Json::Value in_value;
//     in_value["code"] = R"(#include <bits/stdc++.h>

// int main() {
//     std::cout << "hello world" << std::endl;
//     int cnt = 10;
//     while (cnt--) std::cout << "hello world" << std::endl;
//     return 0;
// })";
//     in_value["input"] = "你好";
//     in_value["cpu_limit"] = 1;
//     in_value["mem_limit"] = 1024 * 1024 * 20;

//     // Json::StyledWriter writer;
//     // in_json = writer.write(in_value);
//     // std::cout << in_json << std::endl;
//     // 确保中文编码
//     Json::StreamWriterBuilder builder;
//     builder.settings_["indentation"] = "";  // 紧凑格式
//     builder.settings_["emitUTF8"] = true;   // 直接输出UTF-8
    
//     in_json = Json::writeString(builder, in_value);
//     std::cout << in_json << std::endl;

//     std::string out_json;
//     CompileAndRun::Start(in_json, &out_json);
//     std::cout << out_json << std::endl;

    // cpp-httplib 网络服务库

 
    return 0;
}
