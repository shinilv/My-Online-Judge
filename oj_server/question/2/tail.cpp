#ifndef COMPILER_ONLINE

#include "header.cpp"

#endif

void Test1() {
    int ret = Solution().Max({1, 2, 3, 4, 5});
    if (ret == 5) {
        std::cout << "通过用例1通过 ...OK!" << std::endl;
    } else {
        std::cout << "测试5不通过 ...Failed!" << std::endl;
    }
}

void Test2() {
    int ret = Solution().Max({1, 2, 3, 4, 6});
    if (ret == 6) {
        std::cout << "通过用例2通过 ...OK!" << std::endl;
    } else {
        std::cout << "测试6不通过 ...Failed!" << std::endl;
    }
}

int main() {
    Test1();
    Test2();
}