/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2431」逆序对数列 1-11-2017
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 1000;
const int MOD = 10000;

int f[MAXN + 1][MAXN + 1], sum[MAXN + 1][MAXN + 1];

inline void solve() {
    register int n, k;
    std::cin >> n >> k;
    for (register int i = 0; i <= n; i++) {
        f[i][0] = sum[i][0] = 1;
        for (register int j = 1; j <= k; j++) {
            f[i][j] = sum[i - 1][j];
            if (j >= i) f[i][j] = (f[i][j] - sum[i - 1][j - i] + MOD) % MOD;
            sum[i][j] = (sum[i][j - 1] + f[i][j]) % MOD;
        }
    }
    std::cout << f[n][k];
}

int main() {
    solve();
    return 0;
}