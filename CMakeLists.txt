cmake_minimum_required(VERSION 3.16)
project(Online_Judge LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 将所有可执行文件输出到项目根目录下的 excu 目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/excu")

file(MAKE_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

set(COMMON_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/common")

add_executable(compile_server
    compile_server/compile_server.cc
)
target_include_directories(compile_server
    PRIVATE
        ${COMMON_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/compile_server
)
target_link_libraries(compile_server
    PRIVATE
        jsoncpp
)

add_executable(oj_server
    oj_server/oj_server.cc
)
target_include_directories(oj_server
    PRIVATE
        ${COMMON_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/oj_server
        ${CMAKE_SOURCE_DIR}/oj_server/include
)
target_link_directories(oj_server
    PRIVATE
        ${CMAKE_SOURCE_DIR}/oj_server/lib
)
target_link_libraries(oj_server
    PRIVATE
        ctemplate
        jsoncpp
        mysqlclient
)

