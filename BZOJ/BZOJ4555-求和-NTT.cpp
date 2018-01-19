/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 4555」求和 07-01-2018
 * NTT
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

typedef unsigned long long ulong;

const int MAXN = 1 << 18 | 1;
const int MOD = 998244353;
const int G = 3;

int w[MAXN];

inline int modPow(register int a, register int b) {
    register int ret = 1;
    for (; b; b >>= 1, a = (ulong)a * a % MOD)
        (b & 1) && (ret = (ulong)ret * a % MOD);
    return ret;
}

inline void init(const int k) {
    register int n = k >> 1;
    w[n] = 1;
    w[n + 1] = modPow(G, (MOD - 1) / k);
    for (register int i = 2; i < n; i++)
        w[n + i] = (ulong)w[n + i - 1] * w[n + 1] % MOD;
    for (register int i = n - 1; i > 0; i--) w[i] = w[i << 1];
}

inline void ntt(int *a, const int n, const int f) {
    for (register int i = 0, j = 0; i < n; i++) {
        (i > j) && (std::swap(a[i], a[j]), 0);
        for (register int k = n >> 1; (j ^= k) < k; k >>= 1)
            ;
    }
    for (register int i = 1; i < n; i <<= 1) {
        const register int *w = ::w + i;
        for (register int j = 0; j < n; j += i << 1) {
            register int *x = a + j, *y = a + i + j;
            for (register int k = 0, t; k < i; k++) {
                t = (ulong)w[k] * y[k] % MOD;
                y[k] = (x[k] - t <= 0 ? x[k] - t + MOD : x[k] - t);
                x[k] = (x[k] + t >= MOD ? x[k] + t - MOD : x[k] + t);
            }
        }
    }
    if (f == -1) {
        std::reverse(a + 1, a + n);
        const int inv = modPow(n, MOD - 2);
        for (register int i = 0; i < n; i++) a[i] = (ulong)a[i] * inv % MOD;
    }
}

int f[MAXN], g[MAXN], inv[MAXN], fac[MAXN], invI[MAXN];

inline void solve() {
    register int n;
    std::cin >> n;
    register int k = 1;
    for (; k <= (n + 1) << 1;) k <<= 1;
    init(k);
    fac[0] = 1;
    for (register int i = 1; i <= n; i++) fac[i] = (ulong)fac[i - 1] * i % MOD;
    inv[n] = modPow(fac[n], MOD - 2);
    for (register int i = n - 1; i >= 0; i--)
        inv[i] = inv[i + 1] * (i + 1ull) % MOD;
    invI[1] = 1;
    for (register int i = 2; i <= n; i++)
        invI[i] = ((ulong)MOD - MOD / i) * invI[MOD % i] % MOD;
    f[0] = 1;
    f[1] = MOD - 1;
    g[1] = n;
    for (register int i = 2; i <= n; i++) {
        f[i] = (i & 1) ? MOD - inv[i] : inv[i];
        g[i] = modPow(i, n + 1) - i;
        g[i] <= 0 ? g[i] += MOD : 0;
        g[i] = (ulong)g[i] * invI[i - 1] % MOD * inv[i] % MOD;
    }
    ntt(f, k, 1);
    ntt(g, k, 1);
    for (register int i = 0; i < k; i++) f[i] = (ulong)f[i] * g[i] % MOD;
    ntt(f, k, -1);

    register int ans = 1;
    for (register int i = 1, t = 2; i <= n; i++) {
        ans = (ans + (ulong)f[i] * t % MOD * fac[i]) % MOD;
        t = (t + t >= MOD ? t + t - MOD : t + t);
    }
    std::cout << ans;
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}