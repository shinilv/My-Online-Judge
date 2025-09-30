#pragma once
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>

namespace ns_util {
    static const std::string temp_path = "./temp/";
    class PathUtil {
    public:
        static std::string AddSuffix(const std::string& file_name, const std::string suffix) {
            std::string path_name = temp_path;
            path_name += file_name;
            path_name += suffix;
            return path_name;
        }
        // 构建源文件路径 + 后缀的完整文件名
        static std::string Src(const std::string&file_name) {
            return AddSuffix(file_name, ".cpp");
        }
        // 构建可执行程序的完整路径名

        static std::string Exe(const std::string& file_name) {
            return AddSuffix(file_name, ".exe");
        }

        // 标准错误的完整路径
        static std::string stderr(const std::string& file_name) {
            return AddSuffix(file_name, ".stderr");
        }
    };
    class FileUtil {
    public:
    // 判断是否文件存在， int stat(), 获取文件属性， 成功返回0
        static bool IsFileExists(const std::string& path_name) {
            struct stat st;
            if (stat(path_name.c_str(), &st) == 0) {
                return true;
            }
            return false;
        }
    };
    class TimeUtil {
    public:
        static std::string GetTimeStamp() {
            using namespace std::chrono;

            // 获取当前时间点
            auto now = system_clock::now();
            auto in_time_t = system_clock::to_time_t(now);

            // 转换为本地时间
            std::tm buf;
            localtime_r(&in_time_t, &buf); // Linux 下用 localtime_r，Windows 可用 localtime_s

            // 格式化到秒
            std::ostringstream oss;
            oss << std::put_time(&buf, "%Y-%m-%d %H:%M:%S");

            // 获取毫秒部分
            auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
            oss << '.' << std::setw(3) << std::setfill('0') << ms.count();

            return oss.str();
        }
    };
}