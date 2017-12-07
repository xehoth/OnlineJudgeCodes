/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 5104」Fib数列 07-12-2017
 * BSGS + 二次剩余
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

const int MOD = 1e9 + 9;
const int SQRT_5 = 383008016;
const int INV_5 = 200000002;
const int FRAC_SQRT_5_5 = 276601605;
const int INV_FRAC_SQRT_5_5 = 383008016;
const int INV_2 = 500000005;
const int X = 691504013;

const int FRAC_SQRT_5_5_2_MUL_4 = 800000008;

typedef unsigned long long ulong;
typedef unsigned int uint;

inline uint nextUint() {
    static uint x = 495;
    return x ^= x << 13, x ^= x >> 17, x ^= x << 5, x;
}

inline int modPow(register int a, register int b) {
    register int ret = 1;
    for (; b; b >>= 1, a = (ulong)a * a % MOD)
        (b & 1) ? ret = (ulong)ret * a % MOD : 0;
    return ret;
}

struct Complex {
    int r, i;
    static int w;

    Complex(int r, int i) : r(r), i(i) {}

    inline Complex operator*(const Complex &x) const {
        return Complex(((ulong)r * x.r + (ulong)i * x.i % MOD * w) % MOD,
                       ((ulong)r * x.i + (ulong)i * x.r) % MOD);
    }
};

inline Complex modPow(register Complex a, register int b) {
    register Complex ret(1, 0);
    for (; b; b >>= 1, a = a * a)
        if (b & 1) ret = ret * a;
    return ret;
}

int Complex::w;

inline int legendre(const register int n) {
    return modPow(n, MOD - 1 >> 1) == 1 ? 1 : (-(n != 0));
}

inline int cipolla(register int n) {
    if (n == 0) return n;
    if (legendre(n) == -1) return -1;
    register int a;
    for (;;) {
        a = nextUint() % MOD;
        Complex::w = ((ulong)a * a + MOD - n) % MOD;
        if (legendre(Complex::w) == -1) break;
    }
    return modPow(Complex(a, 1), MOD + 1 >> 1).r;
}

const int H = 42899;

struct HashMap {
    std::vector<std::pair<int, int> > d[H];

    inline std::pair<int, int> *find(int i) {
        register int idx = i % H;
        for (register int j = 0; j < d[idx].size(); j++)
            if (d[idx][j].first == i) return &d[idx][j];
        return NULL;
    }

    inline void put(const int i, const int v) {
        register int idx = i % H;
        for (register int j = 0; j < d[idx].size(); j++)
            if (d[idx][j].first == i) return (void)(d[idx][j].second = v);
        d[idx].push_back(std::pair<int, int>(i, v));
    }
} map[2];

int S;

inline int bsgs(int pos, int b) {
    register int min = INT_MAX, least = modPow(modPow(X, S), MOD - 2);
    register std::pair<int, int> *it;
    for (register int i = 0, p, tmp = b; i <= MOD / S;
         i++, tmp = (ulong)tmp * least % MOD) {
        p = i * S, it = map[(pos - (p & 1) + 2) % 2].find(tmp);
        if (!it) continue;
        min = std::min(min, it->second + p);
    }
    return min;
}

inline void solve() {
    register long long n;
    scanf("%lld", &n);
    if (n >= MOD) {
        puts("-1");
        return;
    }

    n = (ulong)SQRT_5 * n % MOD;

    S = sqrt(MOD);
    for (register int i = 0, tmp = 1; i < S; i++, tmp = (ulong)tmp * X % MOD)
        map[i & 1].put(tmp, i);
    register int delta = cipolla(((ulong)n * n + 4ull) % MOD);
    register int ans = INT_MAX;
    if (delta != -1) {
        ans = std::min(ans, bsgs(0, ((ulong)n + delta) * INV_2 % MOD));
        ans = std::min(ans, bsgs(0, ((ulong)n + MOD - delta) * INV_2 % MOD));
    }
    delta = cipolla(((ulong)n * n + MOD - 4ull) % MOD);
    if (delta != -1) {
        ans = std::min(ans, bsgs(1, ((ulong)n + delta) * INV_2 % MOD));
        ans = std::min(ans, bsgs(1, ((ulong)n + MOD - delta) * INV_2 % MOD));
    }
    if (ans == INT_MAX) {
        puts("-1");
        return;
    }
    printf("%d", ans);
}
}  // namespace

int main() {
    solve();
    return 0;
}