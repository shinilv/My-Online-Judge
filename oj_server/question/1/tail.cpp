#ifndef COMPILER_ONLINE

#include "header.cpp"

#endif

void Test1() {
    bool ret = Solution().isPalindrome(121);
    if (ret) {
        std::cout << "通过用例1, 测试121通过 ...OK!" << std::endl;
    } else {
        std::cout << "测试1不通过 ...Failed!" << std::endl;
    }
}

void Test2() {
    bool ret = Solution().isPalindrome(-121);
    if (!ret) {
        std::cout << "通过用例2, 测试-121通过 ...OK!" << std::endl;
    } else {
        std::cout << "测试2不通过 ...Failed!" << std::endl;
    }
}

int main() {
    Test1();
    Test2();
}