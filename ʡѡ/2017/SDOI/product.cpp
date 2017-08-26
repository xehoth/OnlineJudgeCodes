/*******************************************************************************
 * Copyright (c) 2016-2017, xehoth
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name xehoth, nor the names of its contributors may be used
 *       to endorse or promote products derived from this software without 
 *       specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY XEHOTH AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEHOTH AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/
#include <bits/stdc++.h>
/**
 * 「SDOI2017」序列计数 09-05-2017
 * @author xehoth
 */
namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template<class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read())
        x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template<class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        if (x < 0) print('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() {
    fwrite(obuf, 1, oh - obuf, stdout);
}

}

namespace Task {

const int MAXN = 1000005;
const int MAXQ = 1005;
const int MOD = 1000000007;
const int P = MOD - 1;

#define long long long

inline int modPow(int a, int b) {
    if (b == 0) return 1;
    for (; ~b & 1; b >>= 1, a = (long)a * a % MOD);
    register int ret = a;
    for (b >>= 1; b; b >>= 1)
        a = (long)a * a % MOD, (b & 1) ? ret = (long)ret * a % MOD : 0;
    return ret;
}

inline int add(int a, const int b) {
    return (a += b) >= MOD ? a - MOD : a;
}

inline int minus(int a, const int b) {
    return (a -= b) < 0 ? a + MOD : a;
}

int n, qn[MAXQ], qm[MAXQ];
int fibl[MAXN] = { 0, 1, 1 }, fibr[MAXN], inv[MAXN] = { 1, 1, 1 };

inline void init(const int n) {
    for (register int i = 3; i <= n; i++) {
        fibl[i] = add(fibl[i - 1], fibl[i - 2]);
        inv[i] = (long)inv[i - 1] * fibl[i] % MOD;
    }
    inv[n] = modPow(inv[n], MOD - 2);
    for (register int i = n; i; i--) {
        fibr[i] = (long)inv[i] * inv[i - 1] % MOD;
        inv[i - 1] = (long)inv[i] * fibl[i] % MOD;
    }

    for (register int i = 1; i <= n; i++) {
        for (register int j = i + i; j <= n; j += i) {
            fibl[j] = (long)fibl[j] * fibr[i] % MOD;
            fibr[j] = (long)fibr[j] * fibl[i] % MOD;
        }
    }

    fibl[0] = fibr[0] = 1;
    for (register int i = 1; i <= n; i++) {
        fibl[i] = (long)fibl[i - 1] * fibl[i] % MOD;
        fibr[i] = (long)fibr[i - 1] * fibr[i] % MOD;
    }
}

inline int query(int n, int m) {
    register int res = 1;
    for (register int i = 1, j, base, exp; i <= n; i = j + 1) {
        j = std::min(n / (n / i), m / (m / i));
        base = (long)fibl[j] * fibr[i - 1] % MOD,
        exp = (long)(n / i) * (m / i) % P;
        res = (long)res * modPow(base, exp) % MOD;
    }
    return res;
}

inline void solve() {
    using namespace IO;
    read(n);
    for (register int i = 0; i < n; i++) {
        read(qn[i]), read(qm[i]);
        qn[i] > qm[i] ? (std::swap(qn[i], qm[i]), 0) : 0;
    }
    init(*std::max_element(qn, qn + n));
    for (register int i = 0; i < n; i++)
        print(query(qn[i], qm[i])), print('\n');
}
}

int main() {
    freopen("product.in", "r", stdin);
    freopen("product.out", "w", stdout);
    Task::solve();
    IO::flush();
    return 0;
}
