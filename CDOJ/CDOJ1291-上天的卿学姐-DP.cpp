/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「CDOJ 1291」上天的卿学姐 02-01-2017
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

const int MOD = 1e9 + 7;
const int MAXN = 1 << 8 | 1;

int f[MAXN][MAXN];

char s[17];

inline int get() {
    std::cin >> s;
    return strtol(s, NULL, 2);
}

int n, m;

#define add(x, v) (x + v >= MOD ? x + v - MOD : x + v)

inline void solve() {
    std::cin >> n >> m;
    for (register int i = 1, x, s1, s2; i <= n; i++) {
        x = get();
        s1 = x >> 8;
        s2 = x ^ (s1 << 8);
        register int tmp = 1;
        for (register int j = s1;; j = (--j) & s1) {
            tmp = add(tmp, f[j][s2]);
            if (j == 0) break;
        }
        for (register int j = s2; j <= 255; j = (++j) | s2)
            f[s1][j] = add(f[s1][j], tmp);
    }
    register int ans = 0;
    for (register int i = 0; i <= 255; i++) ans = add(ans, f[i][255]);
    std::cout << ans;
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    solve();
    return 0;
}