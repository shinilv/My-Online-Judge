#ifndef COMPILER_ONLINE

#include "header.cpp"

#endif

int Test1() {
    auto ret = Solution().countOfPairs(3, 1, 3);
    vector<int> tem({6, 0, 0});
    if (ret != tem) {
        return -5;
    }
    return 0;
}

int Test2() {
    auto ret = Solution().countOfPairs(4, 1, 1);
    vector<int> tem({6, 4, 2, 0});
    if (ret != tem) {
        return -5;
    }
    return 0;
}

int Test3() {
    auto ret = Solution().countOfPairs(3, 1, 2);
    vector<int> tem({4, 2, 0});
    if (ret != tem) {
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