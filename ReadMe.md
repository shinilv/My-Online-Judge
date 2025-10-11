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


## 基于MVC结构的oj服务设计 oj_server
建议一个小型网站
~~~
1. 获取首页
2. 编辑区域页面
3. 判题功能（compile_sever）

目录question： 题目信息


M - V - C
M: model， 和数据交互的模块， 比如 对题库进行增删查改
oj_model.hpp

V: view 进行页面渲染， 展示给用户
ctemplate 库 进行网页渲染

oj_view.hpp

C: control， 控制器， 核心业务逻辑  
oj_control.hpp


~~~


文件版本题目设计
1. 题目的编号
2. 题目的标题
3. 题目的难度
4. 题目的描述， 题面
5. 时间和空间要求（内部处理）

两批文件
1. questions.list: 题目列表
2. 题目的描述， 预设置的代码（header.cpp）， 测试用例代码（tail.cpp）

这两块内容， 通过题目的编号产生关联


## 负载均衡

~~~
目录conf
service_machine.conf     # 机器
~~~

## mysql

~~~
use oj;

create table if not exists `oj_questios`(
    `number` int primary key auto_increment comment '题目编号',
    `title` varchar(128) NOT NULL comment '题目标题',
    `star` varchar(8) NOT NULL comment '题目难度',
    `desc` text NOT NULL comment '题目描述',
    `header` text NOT NULL comment '预设代码',
    `tail` text NOT NULL comment '对应题目测试用例',
    `test` text NOT NULL comment '对应题目判题代码',
    `cpu_limit` int default 1 comment '对应题目时间限制',
    `mem_limit` int default 2560000 comment '对应题目内存限制'
)engine=InnoDB default charset=utf8;
~~~

插入信息举例
~~~
INSERT INTO oj_questios (
    title,
    star,
    `desc`,
    header,
    tail,
    test,
    cpu_limit,
    mem_limit
) VALUES (
    '求最大值',
    'easy',
    '给定一个数组， 返回数组中最大值\n\n输入：{1, 2, 3, 4, 5} 输出：5\n\n输入：{1, 2, 3, 4, 6} 输出：6',
    '#include <bits/stdc++.h>\n\nusing namespace std;\n\nclass Solution {\npublic:\n    int Max(const vector<int>& nums) {\n\n        \n    }\n};',
    '#ifndef COMPILER_ONLINE\n\n#include "header.cpp"\n\n#endif\n\nvoid Test1() {\n    int ret = Solution().Max({1, 2, 3, 4, 5});\n    if (ret == 5) {\n        std::cout << "通过用例1通过 ...OK!" << std::endl;\n    } else {\n        std::cout << "测试1不通过 ...Failed!" << std::endl;\n    }\n}\n\nvoid Test2() {\n    int ret = Solution().Max({1, 2, 3, 4, 6});\n    if (ret == 6) {\n        std::cout << "通过用例2通过 ...OK!" << std::endl;\n    } else {\n        std::cout << "测试2不通过 ...Failed!" << std::endl;\n    }\n}\n\n\n\nint main() {\n    Test1();\n    Test2();\n    \n}',
    '#ifndef COMPILER_ONLINE \n\n#include "header.cpp"\n\n#endif\n\nvoid Test1() {\n    int ret = Solution().Max({1, 2, 3, 4, 5});\n    std::cout << ret << std::endl;\n\n    if (ret != 5) {\n        exit(-5);\n    }\n}\n\nvoid Test2() {\n    int ret = Solution().Max({1, 2, 3, 4, 6});\n    std::cout << ret << std::endl;\n\n    if (ret != 6) {\n        exit(-5);\n    }\n\n}\n\nvoid Test3() {\n    int ret = Solution().Max({1, 2, 3, 4, 6, 7, 8, 9, 100});\n    std::cout << ret << std::endl;\n\n    if (ret != 100) {\n        exit(-5);\n    }\n\n}\n\nint main() {\n    Test1();\n    Test2();\n    Test3();\n    return 0;\n}',
    1,
    256000
);

~~~


使用第三方库，访问数据库

model两个版本
版本1: oj_model.hpp 文件版本

版本2: _oj_model.hpp 数据库版本

执行命令导出sql脚本 mysqldump -u root -p oj oj_questios > beifen.sql
