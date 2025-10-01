#include <iostream>
#include <jsoncpp/json/json.h>

int main() {
    // 序列化 工具
    Json::Value root;
    root["code"] = "mycode";
    root["user"] = "nilv";
    root["age"] = "19";

    Json::StyledWriter writer;
    std::string str = writer.write(root);
    std::cout << str << std::endl;
}