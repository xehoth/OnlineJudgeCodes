/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「ARC 058E」和風いろはちゃん / Iroha and Haiku 14-08-2017
 * 状压dp
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace Task {

#define long long long

const int MAXN = 40;
const int MAX_STATE = 1 << 18;
const int MOD = 1e9 + 7;

int f[MAXN + 1][MAX_STATE];

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register int n, x, y, z;
    std::cin >> n >> x >> y >> z;
    register int mask = (1 << z) | (1 << y + z) | (1 << x + y + z);
    register int s = (1 << x + y + z + 1) - 1;
    register int res = 10;
    for (register int i = 2; i <= n; i++) res = (long)res * 10 % MOD;
    f[0][1] = 1;
    for (register int i = 1, state; i <= n; i++) {
        for (register int j = 0; j <= s; j++) {
            for (register int k = 1; k <= 10; k++) {
                state = (j << k & s) + 1;
                if ((mask & state) == mask) continue;
                f[i][state] = (f[i][state] + f[i - 1][j]) % MOD;
            }
        }
    }
    for (register int i = 0; i < s; i++) res = (res - f[n][i]) % MOD;
    std::cout << (res % MOD + MOD) % MOD;
}

#undef long
}

int main() {
    Task::solve();
    return 0;
}