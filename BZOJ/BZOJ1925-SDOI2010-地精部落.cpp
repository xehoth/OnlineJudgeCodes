/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1925」地精部落 10-10-2017
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {
const int MAXN = 4200;

int n, p;

inline void solve() {
    scanf("%d %d", &n, &p);

    static int f[2][MAXN];
    f[1][1] = 1;
    for (register int i = 2; i < n; i++) {
        for (register int j = 1; j <= i; j++) {
            f[i & 1][j] = (f[i & 1][j - 1] + f[~i & 1][i - j]) % p;
        }
    }

    register int ans = 0;
    for (register int i = 0; i < n; i++) ans = (ans + f[(n - 1) % 2][i]) % p;

    printf("%d\n", ans * 2 % p);
}
}

int main() {
    solve();
    return 0;
}