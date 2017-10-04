/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2064」分裂 04-10-2017
 * 状压 DP
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

const int MAXN = 1 << 10;

int sum1[MAXN + 1], sum2[MAXN + 1];
int f[MAXN + 1][MAXN + 1];
int a[11], b[11];

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register int n1, n2;
    std::cin >> n1;
    for (register int i = 0; i < n1; i++) std::cin >> a[i];
    std::cin >> n2;
    for (register int i = 0; i < n2; i++) std::cin >> b[i];
    for (register int i = 1, s = 1 << n1; i < s; i++)
        for (register int j = 0; j < n1; j++)
            if (i >> j & 1) sum1[i] += a[j];
    for (register int i = 1, s = 1 << n2; i < s; i++)
        for (register int j = 0; j < n2; j++)
            if (i >> j & 1) sum2[i] += b[j];
    for (register int i = 0, s = 1 << n1; i < s; i++)
        memset(f[i], 0x3f, sizeof(int) * (1 << n2 | 1));
    for (register int i = 1, s1 = 1 << n1; i < s1; i++) {
        for (register int j = 1, s2 = 1 << n2; j < s2; j++) {
            if (sum1[i] == sum2[j]) {
                f[i][j] = __builtin_popcount(i) + __builtin_popcount(j) - 2;
                for (register int k = i & (i - 1); k; k = i & (k - 1))
                    for (register int t = j & (j - 1); t; t = j & (t - 1))
                        if (sum1[k] == sum2[t])
                            f[i][j] =
                                std::min(f[i][j], f[k][t] + f[i - k][j - t]);
            }
        }
    }
    std::cout << f[(1 << n1) - 1][(1 << n2) - 1];
}
}

int main() {
    solve();
    return 0;
}