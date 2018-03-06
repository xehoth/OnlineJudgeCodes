/**
 * Copyright (c) 2016-2018, xehoth
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * 「SuperOJ 2103」呼吸决定 06-03-2018
 * 扩展埃筛 + 自然数幂和 + 取模优化
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

#ifdef DBG
clock_t startClock, endClock;
#endif

namespace {

typedef unsigned int uint;
typedef unsigned long long ulong;

template <uint mod, int e = 6, uint inv = 1>
struct MulInv {
    static const uint INV = MulInv<mod, e - 1, (inv * (2 - inv * mod))>::INV;
};

template <uint mod, uint inv>
struct MulInv<mod, 0, inv> {
    static const uint INV = inv;
};

template <uint mod>
struct Mod {
    static const uint inv = MulInv<mod>::INV;
    static const uint r2 = -ulong(mod) % mod;
    uint x;
    Mod() : x() {}
    Mod(const uint n) : x(init(n)) {}
    static inline uint init(const uint w) { return reduce(ulong(w) * r2); }
    static inline uint reduce(const ulong w) {
        return uint(w >> 32) + mod - uint((ulong(uint(w) * inv) * mod) >> 32);
    }

    inline void operator+=(const Mod &p) {
        x += p.x - mod;
        if ((int)x < 0) x += mod;
    }

    inline Mod operator+(const Mod &p) const {
        Mod t = *this;
        t += p;
        return t;
    }

    inline void operator*=(const Mod &p) { x = reduce(ulong(x) * p.x); }

    inline Mod operator*(const Mod &p) const {
        Mod t = *this;
        t *= p;
        return t;
    }

    inline uint get() const { return reduce(x); }
};
}  // namespace

const int MOD = 998244353;
const int G = 3;
const int MAXM = 200000 + 3;
const int MAXG = 1 << 19 | 1;
const int MAXT = 3000000 + 1;

int w[MAXG] = {1}, pres[MAXT];

typedef Mod<998244353> Int;

inline int modPow(Int a, int b) {
    Int ret(1);
    for (; b; b >>= 1, a *= a)
        if (b & 1) ret *= a;
    return ret.get();
}

int n, m, k;

Int fac[MAXG], inv[MAXG];

inline void ntt(int *a, int n, int f) {
    for (int i = 0, j = 0; i < n; i++) {
        if (i > j) std::swap(a[i], a[j]);
        for (int k = n >> 1; (j ^= k) < k; k >>= 1)
            ;
    }
    for (int i = 1; i < n; i <<= 1) {
        for (int j = 0; j < n; j += i << 1) {
            int *x = a + j, *y = a + i + j, *w = ::w + i;
            for (int k = 0, t; k < i; k++) {
                t = (long long)w[k] * y[k] % MOD;
                y[k] = (x[k] - t <= 0 ? x[k] - t + MOD : x[k] - t);
                x[k] = (x[k] + t >= MOD ? x[k] + t - MOD : x[k] + t);
            }
        }
    }
    if (f == -1) {
        std::reverse(a + 1, a + n);
        const int inv = modPow(n, MOD - 2);
        for (int i = 0; i < n; i++) a[i] = (long long)a[i] * inv % MOD;
    }
}

inline void init(const int k) {
    int bn = k >> 1;
    w[bn + 0] = 1;
    w[bn + 1] = modPow(G, (MOD - 1) / k);
    for (int i = 2; i < bn; i++)
        w[bn + i] = (long long)w[bn + i - 1] * w[bn + 1] % MOD;
    for (int i = bn - 1; i > 0; i--) w[i] = w[i << 1];
}

inline int dec(const int a, const int v) {
    return a - v < 0 ? a - v + MOD : a - v;
}

inline int add(const int a, const int v) {
    return a + v >= MOD ? a + v - MOD : a + v;
}

struct Polynomial {
    int deg;
    int a[MAXG];

    void inverse(const int n, Polynomial &p) {
        if (n == 1) return (void)(p.deg = 1, p.a[0] = modPow(a[0], MOD - 2));
        static int tmp[MAXG];
        inverse((n + 1) >> 1, p);
        int k = 1;
        for (; k < n << 1;) k <<= 1;
        memcpy(tmp, a, sizeof(int) * n);
        memset(tmp + n, 0, sizeof(int) * (k - n + 1));
        ntt(tmp, k, 1);
        memset(p.a + p.deg, 0, sizeof(int) * (k - p.deg + 1));
        ntt(p.a, k, 1);
        for (int i = 0; i < k; i++)
            p.a[i] = (long long)p.a[i] *
                     dec(2, (long long)tmp[i] * p.a[i] % MOD) % MOD;
        ntt(p.a, k, -1), p.deg = n;
    }
} A, B;

inline Int nCr(int n, int r) { return fac[n] * inv[r] * inv[n - r]; }

Int invB, c[MAXM];

inline int get(int n) {
    Int ret(0), mul(1), v(n + 1);
    for (int i = 1; i <= m + 1; i++) {
        mul *= v;
        ret += mul * c[i];
    }
    return (ret * invB).get();
}

const int MAXS = 31631;

int sprime[MAXS], cnt, pm[MAXS], M;
bool vis[MAXS];

inline void sieve(int n) {
    sprime[cnt++] = 2;
    for (int i = 3; i <= n; i += 2) {
        if (!vis[i]) sprime[cnt++] = i;
        for (int j = 0, k; j < cnt && (k = i * sprime[j]) <= n; j++)
            if (i % sprime[j] == 0) break;
    }
    for (int i = 0; i < cnt; i++) pm[sprime[i]] = modPow(sprime[i], m);
}

std::vector<int> pre[2], suc[2], primes;

int rec(int res, int last, int mul) {
    int t =
        dec(res > M ? suc[1][n / res] : pre[1][res], pre[1][primes[last] - 1]);
    int ret = (long long)(MOD - t) * mul % MOD;
    for (int i = last, p; i < (int)primes.size(); i++) {
        p = primes[i];
        if ((long long)p * p > res) break;
        ret =
            add(ret, rec(res / p, i + 1, (MOD - (long long)mul * pm[p] % MOD)));
    }
    return ret;
}

inline int solve(int n) {
    for (int i = 1; i < MAXT; i++) pres[i] = add(pres[i - 1], modPow(i, m));
    M = sqrt(n);
    sieve(M + 1);
    pre[0].resize(M + 1);
    pre[1].resize(M + 1);
    suc[0].resize(M + 1);
    suc[1].resize(M + 1);
    primes.reserve(M + 1);
#ifdef DBG
    startClock = clock();
#endif
    for (int i = 1; i <= m + 1; i++) c[i] = nCr(m + 1, i) * B.a[m + 1 - i];
    if (m == 1) {
        for (int i = 1; i <= M; i++) {
            pre[0][i] = i - 1;
            pre[1][i] = (i * (i + 1ll) / 2) % MOD;
            const int t = (n / i) % MOD;
            suc[0][i] = t - 1;
            suc[1][i] = (t * (t + 1ll) / 2) % MOD;
        }
    } else if (m == 2) {
        const int inv6 = modPow(6, MOD - 2);
        for (int i = 1; i <= M; i++) {
            pre[0][i] = i - 1;
            pre[1][i] =
                (i * (i + 1ll) % MOD * (2 * i + 1ll) % MOD) * inv6 % MOD;
            const int t = (n / i) % MOD;
            suc[0][i] = t - 1;
            suc[1][i] =
                (t * (t + 1ll) % MOD * (2 * t + 1ll) % MOD) * inv6 % MOD;
        }
    } else if (m == 3) {
        const int inv4 = modPow(4, MOD - 2);
        for (int i = 1; i <= M; i++) {
            pre[0][i] = i - 1;
            pre[1][i] = (long long)i * i % MOD * (i + 1ll) % MOD * (i + 1ll) %
                        MOD * inv4 % MOD;
            const int t = (n / i) % MOD;
            suc[0][i] = t - 1;
            suc[1][i] = (long long)t * t % MOD * (t + 1ll) % MOD * (t + 1ll) %
                        MOD * inv4 % MOD;
        }
    } else {
        for (int i = 1; i <= M; i++) {
            pre[0][i] = i - 1;
            pre[1][i] = pres[i];
            const int t = (n / i) % MOD;
            suc[0][i] = t - 1;
            suc[1][i] = t < MAXT ? pres[t] : get(t);
        }
    }
    for (int i = 1; i <= M; i++) {
        pre[1][i] = dec(pre[1][i], 1);
        suc[1][i] = dec(suc[1][i], 1);
    }
#ifdef DBG
    endClock = clock();
    std::cerr << "get " << (double)(endClock - startClock) / CLOCKS_PER_SEC
              << std::endl;
#endif
    for (int p = 2, end; p <= M; p++) {
        if (pre[0][p] == pre[0][p - 1]) continue;
        primes.push_back(p);
        const int q = p * p, m = n / p;
        const int pcnt = pre[0][p - 1], psum = pre[1][p - 1];
        end = std::min(M, n / q);
        for (int i = 1, w = M / p; i <= w; i++) {
            suc[0][i] = dec(suc[0][i], dec(suc[0][i * p], pcnt));
            suc[1][i] = dec(suc[1][i],
                            dec(suc[1][i * p], psum) * (long long)pm[p] % MOD);
        }
        for (int i = M / p + 1; i <= end; i++) {
            suc[0][i] = dec(suc[0][i], dec(pre[0][m / i], pcnt));
            suc[1][i] = dec(suc[1][i],
                            dec(pre[1][m / i], psum) * (long long)pm[p] % MOD);
        }
        for (int i = M; i >= q; i--) {
            pre[0][i] = dec(pre[0][i], dec(pre[0][i / p], pcnt));
            pre[1][i] = dec(pre[1][i],
                            dec(pre[1][i / p], psum) * (long long)pm[p] % MOD);
        }
    }
    primes.push_back(M + 1);

    return rec(n, 0, 1);
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    // freopen("calc.out", "w", stdout);
    std::cin >> n >> m;
#ifdef DBG
    std::cerr << "mem "
              << (sizeof(A) * 2 + sizeof(w) + sizeof(sprime) * 2 + sizeof(vis) +
                  sizeof(c) + sizeof(fac) * 2 + sizeof(pres)) /
                     1024.0 / 1024.0
              << std::endl;
    startClock = clock();
#endif
    invB = modPow(m + 1, MOD - 2);
    fac[0] = 1;
    k = m + 1;
    for (int i = 1; i <= k; i++) fac[i] = fac[i - 1] * i;
    inv[k] = modPow(fac[k], MOD - 2);
    for (int i = m; i >= 0; i--) inv[i] = inv[i + 1] * (i + 1ull);
    for (int i = 0; i < k; i++) A.a[i] = inv[i + 1].get();
    A.deg = k;
    int l = 1;
    for (; l < k * 2;) l <<= 1;
    init(l);
    A.inverse(k, B);
    for (int i = 0; i < k; i++) B.a[i] = (long long)B.a[i] * fac[i].get() % MOD;
#ifdef DBG
    endClock = clock();
    std::cerr << "init " << (double)(endClock - startClock) / CLOCKS_PER_SEC
              << std::endl;
#endif
    std::cout << (n > 1 ? 1 + solve(n) : 1) % MOD;
    return 0;
}