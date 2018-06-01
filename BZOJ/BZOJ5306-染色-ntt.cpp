/**
 * Copyright (c) 2018, xehoth
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
 * 「BZOJ 5306」染色 30-05-2018
 * ntt
 * @author xehoth
 */
#include <bits/stdc++.h>

struct InputStream {
    enum { SIZE = 1024 * 1024 };
    char ibuf[SIZE], *s, *t;

    char read() {
        if (s == t) t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin);
        return s == t ? -1 : *s++;
    }

    InputStream &operator>>(int &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        if (iosig) x = -x;
        return *this;
    }
} io;

const int MOD = 1004535809;
const int G = 3;
const int MAXL = 1 << 18 | 1;
const int MAXN = 10000000;
const int MAXM = 100000;

typedef unsigned long long ull;

inline int add(int x, int v) { return x + v >= MOD ? x + v - MOD : x + v; }

inline int dec(int x, int v) { return x - v < 0 ? x - v + MOD : x - v; }

inline int modPow(int a, int b) {
    int ret = 1;
    for (; b; b >>= 1, a = (ull)a * a % MOD)
        if (b & 1) ret = (ull)ret * a % MOD;
    return ret;
}

int rt[MAXL], irt[MAXL], a[MAXL], b[MAXL];

inline void init(int n) {
    rt[0] = 1;
    rt[1] = modPow(G, (MOD - 1) / n / 2);
    for (int i = 2; i < n; i++) rt[i] = (ull)rt[i - 1] * rt[1] % MOD;
    irt[0] = 1;
    irt[1] = modPow(rt[1], MOD - 2);
    for (int i = 2; i < n; i++) irt[i] = (ull)irt[i - 1] * irt[1] % MOD;
    for (int i = 0, j = 0; i < n; i++) {
        if (i > j) {
            std::swap(rt[i], rt[j]);
            std::swap(irt[i], irt[j]);
        }
        for (int k = n >> 1; (j ^= k) < k; k >>= 1)
            ;
    }
}

inline void dit(int *a, int n) {
    for (int i = 1, l = n >> 1; i < n; i <<= 1, l >>= 1) {
        for (int j = 0, w, o = 0; j < i; j++, o += l << 1) {
            w = rt[i + j];
            for (int k = o, t; k < o + l; k++) {
                t = (ull)a[k + l] * w % MOD;
                a[k + l] = dec(a[k], t);
                a[k] = add(a[k], t);
            }
        }
    }
}

inline void dif(int *a, int n) {
    for (int i = n >> 1, l = 1; i; i >>= 1, l <<= 1) {
        for (int j = 0, w, o = 0; j < i; j++, o += l << 1) {
            w = irt[i + j];
            for (int k = o, t; k < o + l; k++) {
                t = a[k + l];
                a[k + l] = (ull)dec(a[k], t) * w % MOD;
                a[k] = add(a[k], t);
            }
        }
    }
    const int inv = modPow(n, MOD - 2);
    for (int i = 0; i < n; i++) a[i] = (ull)a[i] * inv % MOD;
}

int w[MAXM + 1], fac[MAXN + 1], inv[MAXN + 1], pw[MAXM + 1];

int n, m, s;

inline void init() {
    int k = std::max(n, m);
    fac[0] = fac[1] = 1;
    for (int i = 2; i <= k; i++) fac[i] = (ull)i * fac[i - 1] % MOD;
    inv[k] = modPow(fac[k], MOD - 2);
    for (int i = k - 1; i >= 0; i--) inv[i] = (i + 1ull) * inv[i + 1] % MOD;
    pw[0] = 1;
    for (int i = 1; i <= m; i++) pw[i] = (ull)pw[i - 1] * inv[s] % MOD;
}

int main() {
    // freopen("color.in", "r", stdin);
    // freopen("color.out", "w", stdout);
    io >> n >> m >> s;
    for (int i = 0; i <= m; i++) io >> w[i];
    init();
    int k = std::min(n / s, m), l = 1;
    for (; l <= k + k;) l <<= 1;
    init(l);
    for (int i = 0; i <= k; i++) {
        a[i] = (ull)w[i] * inv[i] % MOD;
        b[i] = (i & 1) ? dec(0, inv[i]) : inv[i];
    }
    dit(a, l);
    dit(b, l);
    for (int i = 0; i < l; i++) a[i] = (ull)a[i] * b[i] % MOD;
    dif(a, l);
    int ret = 0;
    for (int i = 0; i <= k; i++) {
        ret = (ret + (ull)inv[m - i] * inv[n - i * s] % MOD * pw[i] % MOD *
                         modPow(m - i, n - i * s) % MOD * a[i]) %
              MOD;
    }
    ret = (ull)ret * fac[n] % MOD * fac[m] % MOD;
    std::cout << ret;
    return 0;
}