#pragma once

#include <iostream>
#include <string>
#include "../common/log.hpp"
#include "../common/util.hpp"
#include <sys/time.h>
#include <sys/resource.h>

namespace ns_runner {
    using namespace ns_util;
    using namespace ns_log;
    class Runner {
    public:
        Runner() {}
        ~Runner() {}

        static void SetProcLimit(int _cpu_limit, int _mem_limit) {
            // 设置cpu时长
            struct rlimit cpu_rlimit;
            cpu_rlimit.rlim_max = RLIM_INFINITY;
            cpu_rlimit.rlim_cur = _cpu_limit;
            setrlimit(RLIMIT_CPU, &cpu_rlimit);

            struct rlimit mem_rlimit;
            mem_rlimit.rlim_max = RLIM64_INFINITY;
            mem_rlimit.rlim_cur = _mem_limit;
            
        }
        // 指明文件名即可, 返回值int, 程序异常返回相关信号
        // 返回值 > 0， 程序异常， 返回值对应异常信号
        // 返回0， 运行完成， 保存到相关临时文件中
        // 返回 -1, 内部错误
        // cpu_limit：程序运行时， 可以使用的最大cpu资源上限
        // mem_limit：程序         可以使用的最大内存资源上限
        static int Run(const std::string& file_name, int cpu_limit, int mem_limit) {
            /*
            1. 代码报完， 结果正确
            2. 代码没跑完， 异常

            首先知道可执行程序是谁
            一个程序启动之后：
                标准输入：暂不处理
                标准输出：程序运行完成， 输出结果
                标准错误：运行时错误信息
            */
            const std::string _execute = PathUtil::Exe(file_name);
            const std::string _stdin = PathUtil::Stdin(file_name);
            const std::string _stdout = PathUtil::Stdout(file_name);
            const std::string _stderr = PathUtil::Stderr(file_name);
            // 打开相关文件
            umask(0);
            int _stdin_fd = open(_stdin.c_str(), O_CREAT | O_RDONLY, 0644);
            int _stdout_fd = open(_stdout.c_str(), O_CREAT|O_WRONLY, 0644);
            int _stderr_fd = open(_stderr.c_str(), O_CREAT|O_WRONLY, 0644);
            if (_stderr_fd < 0 || _stdin_fd < 0 || _stdout_fd < 0) {
                LOG(ERROR) << "运行时打开标准文件失败" << std::endl;
                return -1;
            }

            pid_t pid = fork();
            if (pid < 0) {
                LOG(ERROR) << "运行时创建子进程失败" << std::endl;
                // 进程创建失败
                close(_stdin_fd);
                close(_stderr_fd);
                close(_stdout_fd);
                return -2;
            } else if (pid == 0) {
                // 重定向文件
                dup2(_stdin_fd, 0);
                dup2(_stdout_fd, 1);
                dup2(_stderr_fd, 2);

                // 设置进程资源限制
                SetProcLimit(cpu_limit, mem_limit);

                execl(_execute.c_str(), _execute.c_str(), nullptr);

                // 出错
                exit(-1);

            } else {
                close(_stdin_fd);
                close(_stderr_fd);
                close(_stdout_fd);
                int status = 0;
                waitpid(pid, &status, 0);
                LOG(INFO) << "运行完毕" << std::endl;
                // 程序异常退出，收到信号
                return status & 0x7f;
            }
        }
    };
}