/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「UVA 1366」Martian Mining 18-10-2017
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {
const int MAXN = 500;

int n, m;

int sumA[MAXN + 1][MAXN + 1], sumB[MAXN + 1][MAXN + 1], f[MAXN + 1][MAXN + 1];
int b[MAXN + 1][MAXN + 1];

inline void solveCase() {
    for (register int i = 1, x; i <= n; i++) {
        sumA[i][0] = 0;
        for (register int j = 1; j <= m; j++)
            std::cin >> x, sumA[i][j] = sumA[i][j - 1] + x;
    }
    for (register int i = 1; i <= n; i++)
        for (register int j = 1; j <= m; j++) std::cin >> b[i][j];
    for (register int j = 1, x; j <= m; j++) {
        sumB[j][0] = 0;
        for (register int i = 1; i <= n; i++)
            sumB[j][i] = sumB[j][i - 1] + b[i][j];
    }
    for (register int i = 0; i <= n; i++)
        memset(f[i], 0, sizeof(int) * (m + 1));
    for (register int i = 1; i <= n; i++) {
        for (register int j = 1; j <= m; j++) {
            f[i][j] =
                std::max(f[i][j - 1] + sumB[j][i], f[i - 1][j] + sumA[i][j]);
        }
    }
    std::cout << f[n][m] << '\n';
}

inline void solve() {
    while (std::cin >> n >> m, n && m) solveCase();
}
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    solve();
    return 0;
}