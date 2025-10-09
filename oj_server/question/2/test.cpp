#ifndef COMPILER_ONLINE

#include "header.cpp"

#endif

int Test1() {
    int ret = Solution().Max({1, 2, 3, 4, 5});
    if (ret != 5) {
        return -5;
    }
    return 0;
}

int Test2() {
    int ret = Solution().Max({1, 2, 3, 4, 6});
    if (ret != 6) {
        return -5;
    }
    return 0;
}

int Test3() {
    int ret = Solution().Max({1, 2, 3, 4, 6, 7, 8, 9, 100});
    if (ret != 100) {
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