#ifndef COMPILER_ONLINE

#include "header.cpp"

#endif

void Test1() {
    bool ret = Solution().isPalindrome(121);
    if (res) {
        std::cout << "通过用例1, 测试121通过 ...OK!" << std::endl;
    } else {
        std::cout << "测试121不通过 ...Failed!" << std::endl;
    }
}

void Test2() {
    bool ret = Solution().isPalindrome(-121);
    if (!res) {
        std::cout << "通过用例2, 测试-121通过 ...OK!" << std::endl;
    } else {
        std::cout << "测试-121不通过 ...Failed!" << std::endl;
    }
}

int main() {
    Test1();
    Test2();
}
