#include <iostream>
#include <string>
#include <istream>
#include <vector>
#include "./common/util.hpp"
using namespace ns_util;

int main() {
    std::ifstream in("./oj_server/conf/service_machine.conf");
    std::string line;
    if (!in.is_open()) {
        std::cout << false << '\n';
        return 0;
    }
    while (std::getline(in, line)) {
        std::cout << line << '\n';
        std::vector<std::string> res;
        StringUtil::SplitString(line, ":", &res);
        for (auto x : res) {
            std::cout << x << '\n';
        }
    }
    return 0;
}