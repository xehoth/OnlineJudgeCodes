/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1084」20-10-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 100;
int s[3][MAXN + 1], f[MAXN + 1][MAXN + 1][11];

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register int n, m, K;
    std::cin >> n >> m >> K;
    for (register int i = 1, x; i <= n; i++)
        for (register int j = 1; j <= m; j++)
            std::cin >> x, s[j][i] = s[j][i - 1] + x;
    for (register int i = 1; i <= n; i++) {
        for (register int j = 1; j <= n; j++) {
            for (register int k = 0; k <= K; k++) {
                f[i][j][k] = std::max(f[i - 1][j][k], f[i][j - 1][k]);
                if (k) {
                    for (register int t = 0; t < i; t++)
                        f[i][j][k] = std::max(
                            f[i][j][k], f[t][j][k - 1] + s[1][i] - s[1][t]);
                    for (register int t = 0; t < j; t++)
                        f[i][j][k] = std::max(
                            f[i][j][k], f[i][t][k - 1] + s[2][j] - s[2][t]);
                    if (i != j) continue;
                    for (register int t = 0; t < i; t++)
                        f[i][j][k] = std::max(
                            f[i][j][k], f[t][t][k - 1] + s[1][i] - s[1][t] +
                                            s[2][j] - s[2][t]);
                }
            }
        }
    }
    std::cout << f[n][n][K];
}