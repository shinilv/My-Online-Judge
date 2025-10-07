#pragma once

#include "compiler.hpp"
#include "runner.hpp"
#include "../common/util.hpp"
#include "../common/log.hpp"
#include <signal.h>

#include <jsoncpp/json/json.h>


using namespace ns_log;
using namespace ns_compiler;
using namespace ns_runner;
using namespace ns_util;

namespace ns_compile_and_run {
    class CompileAndRun {
    public:
        /*
        code > 0 异常崩溃
        code < 0 整个过程非运行报错
        == 0 整个过程全部完成
        
        */
        static std::string CodeToDesc(const std::string &file_name,  int code) {
            std::string desc;
            switch (code) {
                case 0:
                    desc = "编译并运行成功！";
                    break;
                case -1:
                    desc = "用户提交代码为空";
                    break;
                case -2:
                    desc = "未知错误";
                    break;
                case -3:
                    // desc = "代码编译时发生错误";
                    FileUtil::ReadFile(PathUtil::CompilerError(file_name), &desc, true);
                    break;
                case SIGABRT: // 6
                    desc = "内存超限";
                    break;
                case SIGXCPU: 
                    desc = "运行超时";
                    break;
                    case SIGFPE:
                    desc = "浮点数错误";
                break;
                // case: break;
                // case: break;
                // case: break;
                // case: break;
                default: 
                    desc = "未知错误" + std::to_string(code);
                    break;
            }
            return desc;
        }


     /*
     输入：
     code:  用户提交的代码
     input： 用户给自己提交的代码的对应的输入, 不做处理
     // 时间空间要求

     输出：
        必填
        status：状态码
        reason： 请求结果
        选填：
        stdout：运行完的结果
        stderr：我的程序运行完的错误结果
     */

        // 参数 in_json: {"code": "#incldue...", "input": "", "cpu_limit":1, "mem_limit": 10240}
        // out_json: {"status":"0", "reason":"", "stdout":"", stderr:""}
        static void Start(const std::string& in_json, std::string *out_json) {
            LOG(INFO) << "开始处理请求" << std::endl;
            Json::Value in_value;
            Json::Reader reader;
            if (!reader.parse(in_json, in_value)) {
                LOG(ERROR) << "JSON 解析失败: " << std::endl;
                *out_json = "{\"status\": -2, \"reason\": \"JSON 解析失败\"}";
                return;
            }
            // LOG(INFO) << std::endl;
            std::string code = in_value["code"].asString();
            std::string input = in_value["input"].asString();
            int cpu_limit = in_value["cpu_limit"].asInt();
            int mem_limit_mb = in_value["mem_limit"].asInt();

            // LOG(INFO) << std::endl;

            Json::Value out_value;
            int status_code = 0, rescode;
            std::string file_name;

            if (code.size() == 0) {
                // 差错
                status_code = -1;
                LOG(WARNING) << "代码为空" << std::endl;
                goto END;
            }
            // 形成的文件具有唯一性， 没有目录， 没有后缀
            // 毫秒级递增的时间戳， 原子性递增唯一值。保证唯一性
            // 确保 temp 目录存在
            PathUtil::EnsureTempDir();
            file_name = FileUtil::UniqFileName();

            if (FileUtil::WriteFile(PathUtil::Src(file_name), code)) {
                
                status_code = -2;
                goto END;
            }
            // LOG(INFO) << std::endl;

            // 添加：写入输入数据到stdin文件
            // if (!input.empty()) {
            //     FileUtil::WriteFile(PathUtil::Stdin(file_name), input);
            // }

            if (!Compiler::Compile(file_name)) {
                status_code = -3;
                goto END;
            }
            // LOG(INFO) << std::endl;



            // 将内存限制从 MB 转为字节后传入 Runner
            rescode = Runner::Run(file_name, cpu_limit, (long)mem_limit_mb * 1024 * 1024);
            // 返回值 > 0， 程序异常， 返回值对应异常信号
            // 返回0， 运行完成， 保存到相关临时文件中
            // 返回 -1, 内部错误
            if (rescode < 0) {
                status_code = -2;
                goto END;
            } else if (rescode > 0) {
                // 运行时错误
                status_code = rescode;
            } else {
                status_code = 0;
            }
            END:
                out_value["status"] = status_code;
                out_value["reason"] = CodeToDesc(file_name, status_code);
                if (status_code == 0) {
                    // 整个过程全部成功
                    LOG(INFO) << "整个过程全部成功" << std::endl;
                    std::string _stderr, _stdout;
                    FileUtil::ReadFile(PathUtil::Stdout(file_name), &_stdout, true);
                    FileUtil::ReadFile(PathUtil::Stderr(file_name), &_stderr, true);

                    // 限制回传大小（各 64KB），避免响应体过大
                    const size_t kMaxReturn = 64 * 1024;
                    if (_stdout.size() > kMaxReturn) {
                        _stdout.resize(kMaxReturn);
                        _stdout += "\n...[truncated]";
                    }
                    if (_stderr.size() > kMaxReturn) {
                        _stderr.resize(kMaxReturn);
                        _stderr += "\n...[truncated]";
                    }
                    out_value["stdout"] = _stdout;
                    out_value["stderr"] = _stderr;  
                } 
            // LOG(INFO) << std::endl;

                
                // 使用StreamWriterBuilder替代StyledWriter
                Json::StreamWriterBuilder builder;
                builder.settings_["indentation"] = "";  // 紧凑格式
                builder.settings_["emitUTF8"] = true;   // 直接输出UTF-8字符
                
                *out_json = Json::writeString(builder, out_value);
                

                // 执行完毕， 需要清理所有临时文件
                FileUtil::RemoveTempFile(file_name);
                LOG(INFO) << "执行完毕， 清理所有临时文件" << std::endl;
        }

    };
}