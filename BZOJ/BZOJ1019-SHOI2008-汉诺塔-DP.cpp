/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1019」汉诺塔 10-10-2017
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

const int MAXN = 30;

#define long long long

inline void solve() {
    register int n;
    std::cin >> n;
    static int g[MAXN + 1][3];
    g[1][0] = g[1][1] = g[1][2] = -1;
    for (register int i = 0; i < 6; i++) {
        register char s[3];
        std::cin >> s;
        register int a = s[0] - 'A', b = s[1] - 'A';
        if (g[1][a] == -1) g[1][a] = b;
    }
    static long f[MAXN + 1][3];
    f[1][0] = f[1][1] = f[1][2] = 1;
    for (register int j = 2; j <= n; j++) {
        for (register int i = 0; i < 3; i++) {
            const int a = g[j - 1][i], b = 3 - a - i;
            if (g[j - 1][a] == b) {
                f[j][i] = f[j - 1][i] + 1 + f[j - 1][a];
                g[j][i] = b;
            } else {
                f[j][i] = f[j - 1][i] + 1 + f[j - 1][a] + 1 + f[j - 1][i];
                g[j][i] = a;
            }
        }
    }
    std::cout << f[n][0];
}

#undef long
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    solve();
    return 0;
}
