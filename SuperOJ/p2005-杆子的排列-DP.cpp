/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 2005」杆子的排列 23-10-2017
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

long long f[201][201][201];

long long dp(int num, int l, int r) {
    if (num == 1 && l == 1 && r == 1) return 1;
    register long long &ret = f[num][l][r];
    if (~ret) return ret;
    if (num < 1 || l < 1 || r < 1) return ret = 0;
    return ret = (dp(num - 1, l - 1, r) + dp(num - 1, l, r - 1) +
                  dp(num - 1, l, r) * (num - 2)) %
                 998244353;
}

inline void solve() {
    register int T, n, l, r;
    memset(f, -1, sizeof(f));
    for (std::cin >> T; T--;)
        std::cin >> n >> l >> r, std::cout << dp(n, l, r) << '\n';
}
}

int main() {
    // freopen("pole.in", "r", stdin);
    // freopen("pole.out", "w", stdout);
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    solve();
    return 0;
}