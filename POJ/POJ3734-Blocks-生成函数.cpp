/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「POJ 3734」Blocks 29-07-2017
 * 生成函数
 * @author xehoth
 */
#include <iostream>

const int MOD = 10007;

inline int modPow(int a, int b, int c) {
    register int ret = 1;
    for (; b; b >>= 1, a = a * a % c) (b & 1) ? ret = ret * a % c : 0;
    return ret;
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register int t, n;
    for (std::cin >> t; t--;) {
        std::cin >> n;
        std::cout << (modPow(4, n - 1, MOD) + modPow(2, n - 1, MOD)) % MOD
                  << '\n';
    }
    return 0;
}