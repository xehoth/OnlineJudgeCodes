/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2748」音量调节 25-10-2017
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

bool f[51][1001];
int c[51];

inline void solve() {
    register int n, init, max;
    std::cin >> n >> init >> max;
    for (register int i = 1; i <= n; i++) std::cin >> c[i];
    f[0][init] = true;
    for (register int i = 1; i <= n; i++) {
        for (register int j = 0; j <= max; j++) {
            f[i][j] |= ((j + c[i] <= max && f[i - 1][j + c[i]]) ||
                        (j - c[i] >= 0 && f[i - 1][j - c[i]]));
        }
    }
    for (register int i = max; i >= 0; i--) {
        if (f[n][i]) {
            std::cout << i;
            return;
        }
    }
    std::cout << "-1";
}
}

int main() {
    solve();
    return 0;
}