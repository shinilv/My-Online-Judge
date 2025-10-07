#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../common/util.hpp"
#include "../common/log.hpp"


// 只负责代码编译
// 远端提交过来的代码
/*
编译通过：
编译出错(stderr)：需要形成临时文件， 保存编译出错的结果

核心思路： fork(), 子进程执行编译工作， --> 重定向stderr

*/

namespace ns_compiler{
    using namespace ns_util;
    using namespace ns_log;
    class Compiler {
    public:
        Compiler() {}
        ~Compiler() {}
        // 返回编译成功或失败， 输入参数：编译的文件名
        // file_name: 1234     -->  ./temp/1234.cpp, ./temp/1234.exe, ./temp/1234.stderr
        static bool Compile(const std::string& file_name) {
            pid_t pid = fork();
            if (pid == 0) {
                // 子进程, 调用编译器（程序替换）
                umask(0);
                int _stderr = open(PathUtil::CompilerError(file_name).c_str(), O_CREAT | O_WRONLY, 0644);
                if (_stderr < 0) {
                    LOG(WARNING) << "文件创建失败" << std::endl;
                    exit(1);
                }
                // 重定向
                dup2(_stderr, 2);
                // 关闭原始文件描述符，避免泄漏
                close(_stderr);
                // 进程替换，不影响文件描述符表
                execlp("g++", "g++", "-o", PathUtil::Exe(file_name).c_str(),
                PathUtil::Src(file_name).c_str(), "-std=c++11", nullptr);
                LOG(ERROR) << "启动编译器g++失败， 可能是参数错误" << std::endl;
                exit(2);
            } else if (pid < 0) {
                // 创建子进程失败
                LOG(ERROR) << "内部错误， 创建子进程失败" << std::endl;
                return false;
            } else {

                // 等待子进程结果
                waitpid(pid, nullptr, 0);
                // 编译是否成功, 看是否形成可执行程序
                if (FileUtil::IsFileExists(PathUtil::Exe(file_name))) {
                    LOG(INFO) << PathUtil::Src(file_name) << "编译成功!" << std::endl;
                    return true;
                }
                LOG(ERROR) << "编译失败， 没有形成可执行程序" << std::endl;
                return false;
            }
        }
    };
}