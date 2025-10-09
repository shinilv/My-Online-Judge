#ifndef COMPILER_ONLINE

#include "header.cpp"

#endif

void Test1() {
    auto ret = Solution().countOfPairs(3, 1, 3);
    vector<int> tem({6, 0, 0});
    if (ret == tem) {
        std::cout << "通过用例1通过 ...OK!" << std::endl;
    } else {
        std::cout << "测试1不通过 ...Failed!" << std::endl;
    }
}

void Test2() {
    auto ret = Solution().countOfPairs(4, 1, 1);
    vector<int> tem({6, 4, 2, 0});
    if (ret == tem) {
        std::cout << "通过用例2通过 ...OK!" << std::endl;
    } else {
        std::cout << "测试2不通过 ...Failed!" << std::endl;
    }
}



int main() {
    Test1();
    Test2();
    
}