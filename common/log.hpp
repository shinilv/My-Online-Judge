#pragma once

#include <iostream>
#include <string>
#include "util.hpp"

namespace ns_log {
    using namespace ns_util;
    // 日志等级
    enum {
        // 整数
        INFO, 
        DEBUG,
        WARNING,
        ERROR,
        FATAL,
    };
    // LOG() << "message"
    std::ostream& Log(const std::string& level, const std::string& file_name, int line) {
        std::string message = "[";
        message += level;
        message += "]";
        // 添加 报错文件名称
        message += " [";
        message += file_name;
        message += "]";
        // 添加 报错行
        message += " [";
        message += std::to_string(line);
        message += "]";
        // 日志时间戳
        message += " [";
        message += TimeUtil::GetTimeStamp();
        message += "]";
        std::cout << message;
        return std::cout;
    }
    // 开放式日志接口
    #define LOG(level) Log(#level, __FILE__, __LINE__)
}