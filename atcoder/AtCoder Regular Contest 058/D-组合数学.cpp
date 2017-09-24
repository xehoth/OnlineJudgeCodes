/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「ARC 058D」いろはちゃんとマス目 / Iroha and a Grid 14-08-2017
 * 组合数学
 * @author xehoth
 */
#include <bits/stdc++.h>

#define long long long

const int MOD = 1e9 + 7;
const int MAXN = 200000;

inline int modPow(int a, int b, int c) {
    register int ret = 1;
    for (; b; b >>= 1, a = (long)a * a % c)
        (b & 1) ? ret = (long)ret * a % c : 0;
    return ret;
}

int fac[MAXN + 1] = {1}, inv[MAXN + 1];

inline void init(const int n) {
    for (register int i = 1; i <= n; i++) fac[i] = (long)fac[i - 1] * i % MOD;
    inv[n] = modPow(fac[n], MOD - 2, MOD);
    for (register int i = n - 1; i >= 0; i--)
        inv[i] = (long)inv[i + 1] * (i + 1) % MOD;
}

inline int nCr(int n, int r) {
    return (long)fac[n] * inv[r] % MOD * inv[n - r] % MOD;
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    init(MAXN);
    register int H, W, A, B;
    std::cin >> H >> W >> A >> B;
    register int ans = 0, tmp;
    for (register int i = B + 1, h, w; i <= W; i++) {
        h = H - A - 1, w = i - 1, tmp = nCr(h + w, h);
        h = A - 1, w = W - i, ans = (ans + (long)nCr(h + w, w) * tmp) % MOD;
    }
    std::cout << ans;
    return 0;
}

#undef long