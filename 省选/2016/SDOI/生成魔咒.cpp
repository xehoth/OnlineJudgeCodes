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
 * 「SDOI2016」生成魔咒 24-05-2017
 * 后缀数组
 * @author xehoth
 */
namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template<typename T>
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

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1);
    if (c == -1) {
        *buf = '\0';
        return -1;
    }
    do buf[s++] = c; while (c = read(), !isspace(c) && c != -1);
    buf[s] = '\0';
    return s;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template<typename T>
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

inline void flush() {
    fwrite(obuf, 1, oh - obuf, stdout);
}
}

namespace SuffixArray {

const int MAXN = 100005;

int sa[MAXN], rk[MAXN << 1], ht[MAXN << 1];
bool t[MAXN << 1];

inline bool islms(const int i, const bool *t) {
    return i > 0 && t[i] && !t[i - 1];
}

template<typename T>
inline void sort(T s, int *sa, const int len, const int sz, const int sigma, 
    bool *t, int *b, int *cb, int *p) {
    memset(b, 0, sizeof(int) * sigma);
    memset(sa, -1, sizeof(int) * len);
    for (register int i = 0; i < len; i++) b[s[i]]++;
    cb[0] = b[0];
    for (register int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i];
    for (register int i = sz - 1; i >= 0; i--) sa[--cb[s[p[i]]]] = p[i];
    for (register int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i - 1];
    for (register int i = 0; i < len; i++) if (sa[i] > 0 && !t[sa[i] - 1])
        sa[cb[s[sa[i] - 1]]++] = sa[i] - 1;
    cb[0] = b[0];
    for (register int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i];
    for (register int i = len - 1; i >= 0; i--) if (sa[i] > 0 && t[sa[i] - 1])
        sa[--cb[s[sa[i] - 1]]] = sa[i] - 1;
}

template<typename T>
inline void sais(T s, int *sa, const int len, bool *t, int *b, int *b1, 
    const int sigma) {
    register int i, j, x, p = -1, cnt = 0, sz = 0, *cb = b + sigma;
    for (t[len - 1] = 1, i = len - 2; i >= 0; i--) t[i] = s[i] < s[i + 1] ||
        (s[i] == s[i + 1] && t[i + 1]);
    for (i = 1; i < len; i++) if (t[i] && !t[i - 1]) b1[sz++] = i;
    sort(s, sa, len, sz, sigma, t, b, cb, b1);
    for (i = sz = 0; i < len; i++) if (islms(sa[i], t)) sa[sz++] = sa[i];
    for (i = sz; i < len; i++) sa[i] = -1;
    for (i = 0; i < sz; i++) {
        for (x = sa[i], j = 0; j < len; j++) {
            if (p == -1 || s[x + j] != s[p + j] || t[x + j] != t[p + j]) {
                p = x, cnt++;
                break;
            } else if (j > 0 && (islms(x + j, t) || islms(p + j, t))) {
                break;
            }
        }
        sa[sz + (x >>= 1)] = cnt - 1;
    }
    for (i = j = len - 1; i >= sz; i--) if (sa[i] >= 0) sa[j--] = sa[i];
    register int *s1 = sa + len - sz, *b2 = b1 + sz;
    if (cnt < sz) sais(s1, sa, sz, t + len, b, b1 + sz, cnt);
    else for (i = 0; i < sz; i++) sa[s1[i]] = i;
    for (i = 0; i < sz; i++) b2[i] = b1[sa[i]];
    sort(s, sa, len, sz, sigma, t, b, cb, b2);
}

template<typename T>
inline void getHeight(T s, const int n) {
    for (register int i = 1; i <= n; i++) rk[sa[i]] = i;
    for (register int i = 0, j = 0, k = 0; i < n; ht[rk[i++]] = k)
        for (k ? k-- : 0, j = sa[rk[i] - 1]; s[i + k] == s[j + k]; k++);
            
}

template<typename T>
inline void init(T s, const int n, const int sigma) {
    sais(s, sa, n, t, rk, ht, sigma), rk[0] = 0;
    getHeight(s, n - 1);
}

#define long long long

long sum;
int fa[MAXN], min[MAXN];

inline int get(int x) {
    while (x != fa[x]) x = fa[x] = fa[fa[x]];
    return x;
}

inline void put(int x, int y) {
    if ((x = get(x)) != (y = get(y))) {
        fa[x] = y, sum += std::max(min[x], min[y]);
        min[y] = std::min(min[y], min[x]);
    }
}

inline void solve() {
    using namespace IO;
    register int n, m = 0;
    read(n);
    static int a[MAXN], tmp[MAXN];
    for (register int i = n - 1; i >= 0; i--) read(a[i]), tmp[m++] = a[i];
    std::sort(tmp, tmp + m), m = std::unique(tmp, tmp + m) - tmp;
    
    for (register int i = 0; i < n; i++) 
        a[i] = std::lower_bound(tmp, tmp + m, a[i]) - tmp + 1;
    m = n++;
    init(a, n, n);
    sum = (long)m * (m + 1) >> 1;
    for (register int i = 1; i < n; i++) sum -= ht[i];
    static long ans[MAXN];
    ans[n] = sum;
    for (register int i = 0; i <= n; ++i) fa[i] = i, min[i] = ht[i];
    for (register int i = 0; i < m; i++)
        sum -= m - i, put(rk[i], rk[i] + 1), ans[m - i] = sum;
    for (register int i = 2; i <= n; i++) print(ans[i]), print('\n');
}

#undef long
}

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    SuffixArray::solve();
    IO::flush();
    return 0;
}
