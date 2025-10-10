#ifndef COMPILER_ONLINE

#include "header.cpp"

#endif

void Test1() {
    bool ret = Solution().isPalindrome(121);
    if (ret != true) {
        exit(-5);
    }
}

void Test2() {
    bool ret = Solution().isPalindrome(-121);
    if (ret != false) {
        exit(-5);
    }
}

void Test3() {
    bool ret = Solution().isPalindrome(123456);
    if (ret != false) {
        exit(-5);
    }
}

int main() {
    Test1();
    Test2();
    Test3();
    return 0;
}