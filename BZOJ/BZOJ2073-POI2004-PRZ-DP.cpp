/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2073」PRZ 1-11-2017
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

const int MAXN = 16;
const int MAXS = 1 << MAXN;
int t[MAXN + 1], w[MAXN + 1];
int f[MAXS + 1], sum[MAXS + 1], max[MAXS + 1];

inline void solve() {
    register int maxW, n;
    std::cin >> maxW >> n;
    for (register int i = 1; i <= n; i++) std::cin >> t[i] >> w[i];
    register int maxState = 1 << n;
    for (register int i = 0; i < maxState; i++)
        for (register int j = 0; j < n; j++)
            if ((i >> j) & 1)
                sum[i] += w[j + 1], max[i] = std::max(max[i], t[j + 1]);
    memset(f, 0x3f, sizeof(int) * maxState);
    f[0] = 0;
    for (register int i = 0; i < maxState; i++)
        for (register int j = i; j; j = (j - 1) & i)
            if (sum[j] <= maxW) f[i] = std::min(f[i], max[j] + f[i ^ j]);
    std::cout << f[maxState - 1];
}
}

int main() {
    solve();
    return 0;
}