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
 * 「APIO2015」Bali Sculptures 08-05-2017
 * @author xehoth
 */
namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace IO

namespace Task {

#define long long long

const int MAXN = 2005;

int n, a, b, g[MAXN];
bool f[MAXN][MAXN];
long s[MAXN];

inline bool check(long sum, int bit, long ans) {
    return (sum >> bit + 1 | ans) == ans && (sum >> bit & 1) == 0;
}

inline long solve1() {
    register long ans = 0;
    for (register int bit = log2(s[n]); bit >= 0; bit--) {
        memset(f[0], 0, sizeof(f[0]));
        f[0][0] = true;
        for (register int i = 1; i <= n; i++) {
            for (register int j = 1; j <= std::min(i, b); j++) {
                f[i][j] = false;
                for (register int k = 1; k <= i && !f[i][j]; k++)
                    f[i][j] |=
                        f[k - 1][j - 1] && check(s[i] - s[k - 1], bit, ans);
            }
        }
        register bool flag = false;
        for (register int i = a; i <= b && !flag; i++) flag |= f[n][i];
        ans = ans << 1 | !flag;
    }
    return ans;
}

inline long solve2() {
    register long ans = 0;
    for (register int bit = log2(s[n]); bit >= 0; bit--) {
        g[0] = 0;
        for (register int i = 1; i <= n; i++) {
            g[i] = b + 1;
            for (register int k = 1; k <= i; k++)
                if (g[k - 1] + 1 < g[i] && check(s[i] - s[k - 1], bit, ans))
                    g[i] = std::min(g[i], g[k - 1] + 1);
        }
        ans = ans << 1 | (g[n] > b);
    }
    return ans;
}

inline void solve() {
    using namespace IO;
    read(n), read(a), read(b);
    for (register int i = 1, t; i <= n; i++) read(t), s[i] = s[i - 1] + t;
    print(a > 1 ? solve1() : solve2());
}
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
#endif
    Task::solve();
    IO::flush();
    return 0;
}