/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2321」星器 18-10-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

const int MAXN = 205;

#define long long long

inline long square(int x) { return x * x; }
int n, m, tmp, a[MAXN][MAXN];
long sum;
inline void solve() {
    std::cin >> n >> m;
    for (register int i = 1; i <= n; i++)
        for (register int j = 1; j <= m; j++) std::cin >> a[i][j];
    for (register int i = 1; i <= n; i++) {
        for (register int j = 1; j <= m; j++) {
            std::cin >> tmp;
            sum += (long)(a[i][j] - tmp) * (square(i) + square(j));
        }
    }
    std::cout << sum / 2;
}
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    solve();
    return 0;
}