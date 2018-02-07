/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1076」23-10-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

const int MAXN = 100;

int w[16], g[16], maxState, n, k;
double f[MAXN + 1][1 << 15 | 1];

inline void solve() {
    std::cin >> k >> n;
    for (register int i = 0, x; i < n; i++) {
        std::cin >> w[i];
        while (std::cin >> x && x) g[i] |= 1 << (x - 1);
    }
    maxState = 1 << n;
    for (register int i = k - 1; i >= 0; i--) {
        for (register int s = 0; s < maxState; s++) {
            for (register int j = 0; j < n; j++) {
                if ((s & g[j]) == g[j]) {
                    f[i][s] +=
                        std::max(f[i + 1][s], f[i + 1][s | (1 << j)] + w[j]);
                } else {
                    f[i][s] += f[i + 1][s];
                }
            }
            f[i][s] /= n;
        }
    }
    std::cout << std::fixed << std::setprecision(6) << f[0][0];
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}