/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3028」食物 29-07-2017
 * 生成函数
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MOD = 10007;
const int INV_SIX = 1668;
const int MAXN = 1000;

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register int n = 0;
    static char s[MAXN];
    std::cin >> s;
    for (register char *c = s; *c; c++) n = (n * 10 + (*c ^ '0')) % MOD;
    std::cout << n * (n + 1ll) * (n + 2ll) * INV_SIX % MOD;
    return 0;
}