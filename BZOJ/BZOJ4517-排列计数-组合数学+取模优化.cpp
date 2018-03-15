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
 * 「BZOJ 4517」排列计数 15-03-2018
 * 组合数学 + 取模优化
 * @author xehoth
 */
#include <cctype>
#include <climits>
#include <cstdio>
#include <iostream>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 * 7 };

    char ibuf[SIZE], *s, obuf[SIZE], *oh;

    InputOutputStream() : s(ibuf), oh(obuf) { fread(ibuf, 1, SIZE, stdin); }

    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    inline InputOutputStream &operator>>(int &x) {
        while (!isdigit(*s)) s++;
        for (x = 0; isdigit(*s);) x = x * 10 + (*s++ ^ '0');
        return *this;
    }

    inline InputOutputStream &operator<<(int x) {
        static int buf[21], cnt;
        if (x != 0) {
            for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
            while (cnt) *oh++ = buf[cnt--];
        } else {
            *oh++ = '0';
        }
        *oh++ = '\n';
        return *this;
    }
} io;

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
        if ((x += p.x - mod) > 2147483647) x += mod;
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

const int MOD = 1000000007;
const int MAXN = 1000000 + 1;
const int MAXT = 500000 + 1;

typedef Mod<MOD> Int;

Int fac[MAXN], inv[MAXN], f[MAXN];
int n[MAXT], m[MAXT];

inline int fix(int x) { return x >= MOD ? x - MOD : x; }

inline Int modPow(Int a, int b) {
    Int ret(1);
    for (; b; b >>= 1, a *= a)
        if (b & 1) ret *= a;
    return ret;
}

inline Int nCr(int n, int r) { return fac[n] * inv[r] * inv[n - r]; }

int main() {
    int T, len1 = 0, len2 = 0;
    io >> T;
    for (int i = 0; i < T; i++) {
        io >> n[i] >> m[i];
        len1 = std::max(len1, n[i]);
        len2 = std::max(len2, n[i] - m[i]);
    }
    fac[0] = 1;
    for (int i = 1; i <= len1; i++) fac[i] = fac[i - 1] * i;
    inv[len1] = modPow(fac[len1], MOD - 2);
    for (int i = len1 - 1; i >= 0; i--) inv[i] = inv[i + 1] * (i + 1);
    f[2] = 1;
    for (int i = 3; i <= len2; i++) f[i] = Int(i - 1) * (f[i - 1] + f[i - 2]);
    f[0] = 1;
    for (int i = 0; i < T; i++)
        io << fix((nCr(n[i], m[i]) * f[n[i] - m[i]]).get());
    return 0;
}