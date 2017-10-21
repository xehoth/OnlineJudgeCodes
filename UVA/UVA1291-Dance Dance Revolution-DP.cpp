/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「UVA 1291」Dance Dance Revolution 21-10-2017
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>

inline int cost(int f, int t) {
    return f == t ? 1
                  : ((f == 0 || t == 0) ? 2
                                        : ((f + t == 6 || f + t == 4) ? 4 : 3));
}

int f[2][5][5];

inline void solve() {
    for (register int n, s;;) {
        memset(f, 0x3f, sizeof(f)), f[0][0][0] = 0, n = 0;
        for (int i = 1, (*g)[5], (*h)[5]; std::cin >> s, s; i++) {
            g = f[i & 1], h = f[(i - 1) & 1], n++;
            for (register int x = 0; x <= 4; x++) {
                for (register int y = 0; y <= 4; y++) {
                    g[x][s] = std::min(g[x][s], h[x][y] + cost(y, s));
                    g[s][x] = std::min(g[s][x], h[y][x] + cost(y, s));
                }
            }
            memset(f[(i - 1) & 1], 0x3f, sizeof(f[(i - 1) & 1]));
        }
        if (!n) return;
        register int ans = 0x3f3f3f3f;
        for (register int i = 0; i <= 4; i++)
            for (register int j = 0; j <= 4; j++)
                ans = std::min(ans, f[n & 1][i][j]);
        std::cout << ans << '\n';
    }
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    solve();
    return 0;
}