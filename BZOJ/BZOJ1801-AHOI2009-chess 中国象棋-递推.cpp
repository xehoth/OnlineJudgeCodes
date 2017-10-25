/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1801」中国象棋 25-10-2017
 * 递推
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

const int MOD = 9999973;
const int MAXN = 100;

int f[MAXN + 1][MAXN + 1][MAXN + 1];

int m, n, ans;

inline int C(int x, int y) { return x * (x - 1) >> 1; }
inline void solve() {
    std::cin >> m >> n;
    f[0][0][0] = 1;
    for (register int i = 1; i <= m; i++) {
        for (register int j = 0; j <= n; j++) {
            for (register int k = 0; j + k <= n; k++) {
                f[i][j][k] = f[i - 1][j][k];
                if (j >= 1) {
                    f[i][j][k] =
                        (f[i][j][k] +
                         (long long)f[i - 1][j - 1][k] * (n - j - k + 1)) %
                        MOD;
                }
                if (k >= 1) {
                    f[i][j][k] = (f[i][j][k] +
                                  (long long)f[i - 1][j + 1][k - 1] * (j + 1)) %
                                 MOD;
                }
                if (j >= 2) {
                    f[i][j][k] =
                        (f[i][j][k] +
                         (long long)f[i - 1][j - 2][k] * C(n - j - k + 2, 2)) %
                        MOD;
                }
                if (k >= 1) {
                    f[i][j][k] =
                        (f[i][j][k] +
                         (long long)f[i - 1][j][k - 1] * (n - j - k + 1) * j) %
                        MOD;
                }
                if (k >= 2) {
                    f[i][j][k] =
                        (f[i][j][k] +
                         (long long)f[i - 1][j + 2][k - 2] * C(j + 2, 2)) %
                        MOD;
                }
            }
        }
    }
    for (register int j = 0; j <= n; j++)
        for (register int k = 0; j + k <= n; k++) ans += f[m][j][k], ans %= MOD;
    std::cout << ans;
}
}

int main() {
    solve();
    return 0;
}