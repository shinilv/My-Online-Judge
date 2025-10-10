#ifndef COMPILER_ONLINE

#include "header.cpp"

#endif

void Test1() {
    int ret = Solution().Max({1, 2, 3, 4, 5});
    std::cout << ret << std::endl;

    if (ret != 5) {
        exit(-5);
    }
}

void Test2() {
    int ret = Solution().Max({1, 2, 3, 4, 6});
    std::cout << ret << std::endl;

    if (ret != 6) {
        exit(-5);
    }

}

void Test3() {
    int ret = Solution().Max({1, 2, 3, 4, 6, 7, 8, 9, 100});
    std::cout << ret << std::endl;

    if (ret != 100) {
        exit(-5);
    }

}

int main() {
    Test1();
    Test2();
    Test3();
    return 0;
}