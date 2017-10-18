/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「UVA 1452」Jump 18-10-2017
 * 递推
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

int f[3];

inline int mod(int x, int k) { return (x % k + k) % k; }

inline void solveCase() {
    register int n, k;
    std::cin >> n >> k;
    f[0] = mod(k % 3 - 1, 3);
    f[1] = mod(f[0] + 1 + mod(k % 2 - 1, 2), 3);
    f[2] = 3 - f[0] - f[1];
    for (register int i = 4; i <= n; i++)
        f[0] = (f[0] + k) % i, f[1] = (f[1] + k) % i, f[2] = (f[2] + k) % i;
    std::cout << f[0] + 1 << ' ' << f[1] + 1 << ' ' << f[2] + 1 << '\n';
}

inline void solve() {
    register int T;
    std::cin >> T;
    while (T--) solveCase();
}
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    solve();
    return 0;
}