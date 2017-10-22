/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「UVA 10163」Storage Keepers 22-10-2017
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

const int MAXN = 110;
int n, m, f[MAXN][MAXN], g[MAXN][MAXN], p[MAXN];

inline void solve() {
    while (std::cin >> n >> m && n && m) {
        for (register int i = 1; i <= m; i++) std::cin >> p[i];
        memset(f, 0, sizeof(f));
        for (register int i = 1; i <= m; i++) {
            f[i - 1][0] = 0x3f3f3f3f;
            for (register int j = 1; j <= n; j++) {
                f[i][j] = f[i - 1][j];
                for (register int k = 1; k <= j; k++)
                    f[i][j] =
                        std::max(f[i][j], std::min(f[i - 1][j - k], p[i] / k));
            }
        }
        memset(g, 0x3f, sizeof(g));
        for (register int i = 1; i <= m; i++) {
            g[i - 1][0] = 0;
            for (register int j = 1; j <= n; j++) {
                g[i][j] = g[i - 1][j];
                for (register int k = 1; k <= j; k++)
                    if (p[i] / k >= f[m][n])
                        g[i][j] = std::min(g[i][j], g[i - 1][j - k] + p[i]);
            }
        }
        std::cout << f[m][n] << ' ' << (f[m][n] == 0 ? 0 : g[m][n]) << '\n';
    }
}
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    solve();
    return 0;
}