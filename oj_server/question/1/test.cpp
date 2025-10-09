#ifndef COMPILER_ONLINE

#include "header.cpp"

#endif

int Test1() {
    bool ret = Solution().isPalindrome(121);
    if (ret != true) {
        return -5;
    }
    return 0;
}

int Test2() {
    bool ret = Solution().isPalindrome(-121);
    if (ret != false) {
        return -5;
    }
    return 0;
}

int Test3() {
    bool ret = Solution().isPalindrome(123456);
    if (ret != false) {
        return -5;
    }
    return 0;
}

int main() {
    if (Test1() == -5) return -5;
    if (Test2() == -5) return -5;
    if (Test3() == -5) return -5;
    return 0;
}