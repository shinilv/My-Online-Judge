# 项目核心是三个模块
1. common：公共模块
2. compile_server：编译与运行模块
3. oj_server：获取题目列表，查看题目编写题目， 负载均衡， 其他功能




## compiler 服务设计

**提供的服务：编译并运行代码， 得到格式化的相关数据**

compiler.hpp：编译代码
runner.hpp：运行代码
compile_run.hpp：整合运行编译, 适配用户请求
compile_server.cc：服务
makefile：自动化编译
temp目录：保存临时文件

## common 通用功能设计

log.hpp：日志功能
util.hpp：公共模块

cpp-httplib 轻量级网络库, 阻塞式

