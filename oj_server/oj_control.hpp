#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "../common/log.hpp"
#include "../common/util.hpp"
#include "oj_model.hpp"
#include "oj_view.hpp"
#include <cassert>
#include <fstream>
#include <istream>
#include <mutex>
#include <assert.h>
#include <jsoncpp/json/json.h>
#include "../common/httplib.h"


// 逻辑控制

namespace ns_control {
    using namespace ns_log;
    using namespace ns_model;
    using namespace ns_util;
    using namespace ns_view;
    using namespace httplib;

    //  提供服务的主机
    class Machine {
    public:
        std::string ip;  // 编译服务ip
        int port;        // 编译服务port
        uint64_t load;   // 编译服务负载
        std::mutex *mtx; // 禁止拷贝， 使用指针
    public:
        Machine() :ip(""), port(0), load(0), mtx(nullptr)
        {}
        ~Machine() {}

        // 更新负载
        // 增加
        void IncLoad() {
            if (mtx) mtx->lock();
            load++;
            if (mtx) mtx->unlock();
        }
        //减少
        void DecLoad() {
            if (mtx) mtx->lock();
            load--;
            if (mtx) mtx->unlock();
        }
        // 获取主机负载
        uint64_t Load() {
            uint64_t _load = 0;
            if (mtx) mtx->lock();
            _load = load;
            if (mtx) mtx->unlock();
            return _load;
        }
    };

    const std::string service_machine = "./conf/service_machine.conf";

    // 负载均衡模块
    class LoadBlance {
    private:
        // 每一台主机都有自己的下标， 充当当前主机的id
        std::vector<Machine> machines; // 可以提供编译服务的所有主机
        std::vector<int> online; // 所有在线的主机
        std::vector<int> offline; // 离线主机id
        std::mutex mtx;
    public:
        LoadBlance() {
            assert(LoadCond(service_machine));
            LOG(INFO) << "成功加载配置文件" << std::endl;

        }

        bool LoadCond(const std::string& machine_conf) {
            std::ifstream in(machine_conf);
            if (!in.is_open()) {
                LOG(FATAL) << " 加载" << machine_conf << "失败" << std::endl;
                return false;
            }
            std::string line;
            while (std::getline(in, line)) {
                std::vector<std::string> res;
                StringUtil::SplitString(line, ":", &res);
                if (res.size() != 2) {
                    LOG(WARNING) << "切分" << line << "失败" << std::endl;
                    continue;
                }
                Machine mc;
                mc.ip = res[0];
                mc.port = std::stoi(res[1]);
                mc.load = 0;
                mc.mtx = new std::mutex();
                online.push_back(machines.size());
                machines.push_back(mc);
            }
            in.close();
            return true;
        }
        // 智能选择一台机器
        bool SmartChoice(int *id, Machine **mc) {
            // 使用选择好的主机（更新该主机的负载）
            // 可能需要离线该主机
            // 加锁
            mtx.lock();

            // 负载均衡的选择算法， 1. 随机数法， 
            // 2. 轮询 + 随机, 选择方法2
            int online_num = online.size();
            if (online_num == 0) {
                LOG(FATAL) << "所有的后端编译主机， 已经全部离线， 尽快查看" << std::endl;
                mtx.unlock();
                return false;
            }

            // 找到所有负载最小的机器
            uint64_t min_load = machines[online[0]].Load();
            *id = online[0];
            *mc = &machines[online[0]];

            for (int i = 1; i < online_num; i++) {
                uint64_t cur_load = machines[online[i]].Load();
                if (min_load > cur_load) {
                    min_load = cur_load;
                    *id = online[i];
                    *mc = &machines[online[i]];
                }
            }

            mtx.unlock();
            return true;
        }

        void OffLineMachine(int which) {
            mtx.lock();
            for (auto iter = online.begin(); iter != online.end(); iter++) {
                if (*iter == which) {
                    // 要离线的主机
                    online.erase(iter);
                    offline.push_back(which);
                    break;
                }
            }

            mtx.unlock();
        }
        void OnLineMachine() {
            // 当所有主机都离线的时候。统一上线
        }

        void ShowMachines() {
            mtx.lock();
            std::cout << "当前在线主机列表" << std::endl;
            for (auto& iter : online) {
                std::cout << iter << " ";
            }
            std::cout << std::endl;
            std::cout << "当前离线主机列表" << std::endl;
            for (auto& iter : offline) {
                std::cout << iter << " ";
            }
            std::cout << std::endl;
            mtx.unlock();
        }

        ~LoadBlance() {}

    };



    // 核心业务逻辑
    class Control {
    private:
        Model _model; // 提供后台数据
        View _view; // 提供网页渲染功能
        LoadBlance _load_blance;  // 核心负载均衡控制器
    
    public:
        Control() {

        }
        // 根据题目数据构建网页， 输出型参数
        bool AllQuestions(std::string *html) {
            std::vector<Question> all;
            if (_model.GetAllQuestions(&all)) {
                // 获取题目信息成功
                _view.AllExpandHtml(all, html);
            } else {
                *html = "获取网页失败";
                return false;
            }
            return true;
        }

        bool OneQuestion(const std::string& number, std::string* html) {
            Question question;
            if (_model.GetOneQuestion(number, &question)) {
                // 获取一个题目信息成功
                _view.OneExpandHtml(question, html);
            } else {
                *html = "指定题目" + number + "不存在";
                return false;
            }
            return true;
        }

        // 判题功能
        // id, code, input
        void Judge(const std::string number, const std::string in_json, std::string *out_json) {
            // 根据题目编号， 得到题目信息
            Question q;
            _model.GetOneQuestion(number, &q);

            // in_json 进行反序列化， 得到题目id, 得到用户提交源代码 input
            Json::Reader reader;
            Json::Value in_value;
            reader.parse(in_json, in_value);

            // 重新拼接用户代码 + 测试用例代码， 形成新的代码
            Json::Value compile_value;
            std::string code = in_value["code"].asString();
            compile_value["input"] = in_value["input"].asString();
            compile_value["code"] =  code + q.tail;
            compile_value["cpu_limit"] = q.cpu_limit;
            compile_value["mem_limit"] = q.mem_limit;

            Json::StreamWriterBuilder builder;
            builder.settings_["indentation"] = "";  // 紧凑格式
            builder.settings_["emitUTF8"] = true;   // 直接输出UTF-8字符
            
            std::string compile_string = Json::writeString(builder, compile_value);

            // 选择负载最低的主机， 
            // 一直选择
            while (true) {
                int id = 0;
                Machine *mc = nullptr;
                if (!_load_blance.SmartChoice(&id, &mc)) {
                    break;
                }
                LOG(INFO) << "选择主机成功， 主机id：" << id << "， 详情：" << mc->ip << ":" << mc->port << std::endl;
                // 发起http请求， 得到结果
                Client cli(mc->ip, mc->port);
                mc->IncLoad();
                if (auto res = cli.Post("/compile_and_run", compile_string,"application/json;charset=utf-8")) {
                    if (res->status == 200) {
                        // 结果给out_json
                        *out_json = res->body;
                        // 更新负载
                        mc->DecLoad();
                        LOG(INFO) << "编译和运行服务成功..." << std::endl;
                        break;
                    }
                    mc->DecLoad();
                } else {
                    LOG(ERROR) << "当前请求的主机已关闭id：" << id << "， 详情：" << mc->ip << ":" << mc->port << std::endl;
                    mc->DecLoad();
                    _load_blance.OffLineMachine(id);
                    _load_blance.ShowMachines();
                }
            }


        }

        ~Control() {}
        
    };
}