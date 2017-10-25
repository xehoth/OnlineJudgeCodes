/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4001」概率论 25-10-2017
 * 生成函数
 * @author xehoth
 */
#include <bits/stdc++.h>

int main() {
    double n;
    std::cin >> n;
    std::cout << std::fixed << std::setprecision(9)
              << (n * n + n) / (4 * n - 2);
}