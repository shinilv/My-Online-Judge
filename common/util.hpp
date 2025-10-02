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
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <atomic>
#include <fstream> 

namespace ns_util {
    static const std::string temp_path = "./temp/";
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

        // 获取毫秒级时间戳
        static std::string GetTimeMs() {
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      now.time_since_epoch())
                      .count();
            return std::to_string(ms);
        }
    };
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
        static std::string Stderr(const std::string& file_name) {
            return AddSuffix(file_name, ".stderr");
        }
        // 编译错误
        static std::string CompilerError(const std::string&file_name) {
            return AddSuffix(file_name, ".compiler_error");
        }
        // 运行时文件 stdin, stdout

        static std::string Stdin(const std::string& file_name) {
            return AddSuffix(file_name, ".stdin");
        }

        // 标准错误的完整路径
        static std::string Stdout(const std::string& file_name) {
            return AddSuffix(file_name, ".stdout");
        }
    };
    class FileUtil {
    public:

        // 删除文件
        static void RemoveTempFile(const std::string& file_name) {
            // 清理的文件个数不确定

            std::string _src = PathUtil::Src(file_name);
            if (FileUtil::IsFileExists(_src)) {
                unlink(_src.c_str());
            }
            std::string _compiler_error = PathUtil::CompilerError(file_name);
            if (FileUtil::IsFileExists(_compiler_error)) {
                unlink(_compiler_error.c_str());
            }
            std::string _execute = PathUtil::Exe(file_name);
            if (FileUtil::IsFileExists(_execute)) {
                unlink(_execute.c_str());
            }
            std::string _stderr = PathUtil::Stderr(file_name);
            if (FileUtil::IsFileExists(_stderr)) {
                unlink(_stderr.c_str());
            }
            std::string _stdin = PathUtil::Stdin(file_name);
            if (FileUtil::IsFileExists(_stdin)) {
                unlink(_stdin.c_str());
            }
            std::string _stdout = PathUtil::Stdout(file_name);
            if (FileUtil::IsFileExists(_stdout)) {
                unlink(_stdout.c_str());
            }
        }

        // 读取文件内容, 参数为文件名
        static bool ReadFile(const std::string& target, std::string *content, bool keep) {
            content->clear();
            std::ifstream in(target);
            if (!in.is_open()) {
                std::cerr << "Open file failed: " << target << std::endl;
                return false;
            }
            std::string line;
            while (std::getline(in, line)) {
                (*content) += line;
                if (keep) {
                    (*content) += "\n";  // 保留换行符
                }
            }
            in.close();
            return true;
        }
        // 根据时间戳，设置唯一文件名
        static std::string UniqFileName() {
            // 原子性来保持唯一性， 加锁
            static std::atomic_uint id(0);
            std::string ms = TimeUtil::GetTimeMs();
            std::string uniq_id = std::to_string(id);
            id++;
            return ms + "_" + uniq_id;

        }
        // 把code写进 file_name文件中
        static int WriteFile(const std::string& file_name, std::string& code) {
            std::ofstream ofs(file_name);
            if (!ofs.is_open()) {
                // LOG(ERROR) << "打开文件失败 " << file_name << std::endl;
                std::cerr << "Open file failed: " << file_name << std::endl;
                return -1;
            }
            ofs << code;
            return 0; // 成功返回 0
        }

    // 判断是否文件存在， int stat(), 获取文件属性， 成功返回0
        static bool IsFileExists(const std::string& path_name) {
            struct stat st;
            if (stat(path_name.c_str(), &st) == 0) {
                return true;
            }
            return false;
        }
    };
}