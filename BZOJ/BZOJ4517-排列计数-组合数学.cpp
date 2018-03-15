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
 * 组合数学
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

const int MOD = 1000000007;
const int MAXN = 1000000 + 1;
const int MAXT = 500000 + 1;

int fac[MAXN], inv[MAXN], f[MAXN], n[MAXT], m[MAXT];

inline int modPow(int a, int b) {
    int ret = 1;
    for (; b; b >>= 1, a = (unsigned long long)a * a % MOD)
        if (b & 1) ret = (unsigned long long)ret * a % MOD;
    return ret;
}

inline int nCr(int n, int r) {
    return (unsigned long long)fac[n] * inv[r] % MOD * inv[n - r] % MOD;
}

int main() {
    int T, len1 = 0, len2 = 0;
    io >> T;
    for (int i = 0; i < T; i++) {
        io >> n[i] >> m[i];
        len1 = std::max(len1, n[i]);
        len2 = std::max(len2, n[i] - m[i]);
    }
    fac[0] = 1;
    for (int i = 1; i <= len1; i++)
        fac[i] = fac[i - 1] * (unsigned long long)i % MOD;
    inv[len1] = modPow(fac[len1], MOD - 2);
    for (int i = len1 - 1; i >= 0; i--) inv[i] = inv[i + 1] * (i + 1ull) % MOD;
    f[2] = 1;
    for (int i = 3; i <= len2; i++)
        f[i] = (i - 1ull) * (f[i - 1] + f[i - 2]) % MOD;
    f[0] = 1;
    for (int i = 0; i < T; i++)
        io << nCr(n[i], m[i]) * (unsigned long long)f[n[i] - m[i]] % MOD;
    return 0;
}