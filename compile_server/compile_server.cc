#include "compile_run.hpp"


using namespace ns_compile_and_run;

// 编译服务随时可能被多个人请求，必须保证传递上来的code，形成源文件名称的时候， 要具有
// 唯一性， 多个用户会相互影响

int main() {
    // http 让client给我们 上传一个json string

    return 0;
}