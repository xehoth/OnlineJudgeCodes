/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「ARC 071D」井井井 20-08-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace Task {

#define long long long

const int MAXN = 100000;
const int MOD = 1e9 + 7;
int x[MAXN + 1], y[MAXN + 1];

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register int n, m, sumX = 0, sumY = 0;
    std::cin >> n >> m;
    for (register int i = 1; i <= n; i++) std::cin >> x[i];
    for (register int i = 1; i <= m; i++) std::cin >> y[i];
    for (register int i = 1; i < n; i++)
        sumX = (sumX + (long)(x[i + 1] - x[i]) * i % MOD * (n - i)) % MOD;
    for (register int i = 1; i < m; i++)
        sumY = (sumY + (long)(y[i + 1] - y[i]) * i % MOD * (m - i)) % MOD;
    std::cout << (long)sumX * sumY % MOD;
}

#undef long
}

int main() {
    Task::solve();
    return 0;
}