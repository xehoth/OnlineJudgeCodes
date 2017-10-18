/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「UVA 10534」Wavio Sequence 18-10-2017
 * DP
 * @author xehoth
 */
#include <bits/extc++.h>

namespace {

const int MAXN = 10000;

int inc[MAXN + 1], dec[MAXN + 1], a[MAXN + 1], g[MAXN + 1];

inline void solve(int n) {
    for (register int i = 1; i <= n; i++) std::cin >> a[i];
    memset(g, 0x3f, sizeof(int) * (n + 1));
    for (register int i = 1, k; i <= n; i++) {
        k = std::lower_bound(g + 1, g + n + 1, a[i]) - g;
        inc[i] = k, g[k] = a[i];
    }
    memset(g, 0x3f, sizeof(int) * (n + 1));
    for (register int i = n, k; i >= 1; i--) {
        k = std::lower_bound(g + 1, g + n + 1, a[i]) - g;
        dec[i] = k, g[k] = a[i];
    }
    register int ans = 0;
    for (register int i = 1; i <= n; i++)
        ans = std::max(ans, std::min(dec[i], inc[i]));
    std::cout << ans * 2 - 1 << '\n';
}
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    for (register int n; std::cin >> n;) solve(n);
    return 0;
}