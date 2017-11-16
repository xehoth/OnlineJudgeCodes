/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「CF 245B」Working out 16-11-2017
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

const int MAXN = 1010;

long long f[5][MAXN][MAXN], map[MAXN][MAXN];

const int INF = 0x3f3f3f3f;

inline void solve() {
    register int n, m;
    std::cin >> n >> m;
    for (register int i = 1; i <= n; i++)
        for (register int j = 1; j <= m; j++) std::cin >> map[i][j];
    for (register int i = 1; i <= n; i++) {
        for (register int j = 1; j <= m; j++)
            f[1][i][j] = std::max(f[1][i - 1][j], f[1][i][j - 1]) + map[i][j];
        for (register int j = m; j >= 1; j--)
            f[3][i][j] = std::max(f[3][i - 1][j], f[3][i][j + 1]) + map[i][j];
    }
    for (register int i = n; i >= 1; i--) {
        for (register int j = 1; j <= m; j++)
            f[2][i][j] = std::max(f[2][i + 1][j], f[2][i][j - 1]) + map[i][j];
        for (register int j = m; j >= 1; j--)
            f[4][i][j] = std::max(f[4][i + 1][j], f[4][i][j + 1]) + map[i][j];
    }
    for (register int k = 1; k <= 4; k++)
        for (register int i = 1; i <= n + 1; i++)
            f[k][i][0] = f[k][i][m + 1] = -INF;
    for (register int k = 1; k <= 4; k++)
        for (register int i = 1; i <= m + 1; i++)
            f[k][0][i] = f[k][n + 1][i] = -INF;
    register long long ans = 0;
    for (register int i = 1; i <= n; i++) {
        for (register int j = 1; j <= m; j++) {
            ans = std::max(ans, std::max(f[1][i][j - 1] + f[4][i][j + 1] +
                                             f[2][i + 1][j] + f[3][i - 1][j],
                                         f[1][i - 1][j] + f[4][i + 1][j] +
                                             f[2][i][j - 1] + f[3][i][j + 1]));
        }
    }
    std::cout << ans;
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    solve();
    return 0;
}