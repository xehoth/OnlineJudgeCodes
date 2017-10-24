/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 2009」Math 24-10-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

int a, n, mod;

inline int modPow(int a, int b) {
    register int ret = 1;
    for (; b; b >>= 1, a = ((long long)a * a) & (mod - 1))
        (b & 1) ? ret = ((long long)ret * a) & (mod - 1) : 0;
    return ret;
}

inline void solveCase() {
    mod = 1 << n;
    if (a & 1) {
        std::cout << "1\n";
        return;
    }
    register int tmp = n / a;
    if (tmp * a < n) tmp++;
    register int ans = mod / (1 << tmp) - n / (1 << tmp);
    for (register int i = 1; i <= n; i++)
        if (modPow(a, i) == modPow(i, a)) ans++;
    std::cout << ans << '\n';
}

inline void solve() {
    register int T;
    std::cin >> T;
    while (T--) {
        std::cin >> a >> n;
        solveCase();
    }
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}