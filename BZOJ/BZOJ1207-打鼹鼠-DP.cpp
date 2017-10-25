/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1207」打鼹鼠 25-10-2017
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

const int MAXN = 10000;

int f[MAXN + 1], x[MAXN + 1], y[MAXN + 1], t[MAXN + 1];

inline int dis(const int i, const int j) {
    return abs(x[i] - x[j]) + abs(y[i] - y[j]);
}

inline void solve() {
    register int n, m;
    std::cin >> n >> m;
    for (register int i = 1; i <= m; i++) std::cin >> t[i] >> x[i] >> y[i];
    for (register int i = 1; i <= m; i++) {
        f[i] = 1;
        for (register int j = 1; j < i; j++)
            if (dis(i, j) <= abs(t[i] - t[j])) f[i] = std::max(f[i], f[j] + 1);
    }
    std::cout << *std::max_element(f + 1, f + m + 1);
}
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    solve();
    return 0;
}