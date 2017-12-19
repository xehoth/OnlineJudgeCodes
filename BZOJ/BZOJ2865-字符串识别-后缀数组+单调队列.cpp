/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2865」识别子串 19-12-2017
 * 后缀数组 + 单调队列
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

const int OUT_LEN = 1 << 18 | 1;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    (oh == obuf + OUT_LEN) && (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf);
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[21], cnt;
    if (x != 0) {
        (x < 0) && (print('-'), x = -x);
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    } else {
        print('0');
    }
}

struct OutputStream {
    ~OutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    template <typename T>
    inline OutputStream &operator<<(T x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 500000 + 9;

inline bool islms(const int i, const bool *t) {
    return i > 0 && t[i] && !t[i - 1];
}

template <typename T>
inline void sort(T s, int *sa, const int len, const int sz, const int sigma,
                 bool *t, int *b, int *cb, int *p) {
    memset(b, 0, sizeof(int) * sigma);
    memset(sa, -1, sizeof(int) * len);
    for (register int i = 0; i < len; i++) b[s[i]]++;
    cb[0] = b[0];
    for (register int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i];
    for (register int i = sz - 1; i >= 0; i--) sa[--cb[s[p[i]]]] = p[i];
    for (register int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i - 1];
    for (register int i = 0; i < len; i++)
        if (sa[i] > 0 && !t[sa[i] - 1]) sa[cb[s[sa[i] - 1]]++] = sa[i] - 1;
    cb[0] = b[0];
    for (register int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i];
    for (register int i = len - 1; i >= 0; i--)
        if (sa[i] > 0 && t[sa[i] - 1]) sa[--cb[s[sa[i] - 1]]] = sa[i] - 1;
}

template <typename T>
inline void sais(T s, int *sa, const int len, bool *t, int *b, int *b1,
                 const int sigma) {
    register int i, j, x, p = -1, cnt = 0, sz = 0, *cb = b + sigma;
    for (t[len - 1] = 1, i = len - 2; i >= 0; i--)
        t[i] = s[i] < s[i + 1] || (s[i] == s[i + 1] && t[i + 1]);
    for (i = 1; i < len; i++)
        if (t[i] && !t[i - 1]) b1[sz++] = i;
    sort(s, sa, len, sz, sigma, t, b, cb, b1);
    for (i = sz = 0; i < len; i++)
        if (islms(sa[i], t)) sa[sz++] = sa[i];
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
    for (i = j = len - 1; i >= sz; i--)
        if (sa[i] >= 0) sa[j--] = sa[i];
    register int *s1 = sa + len - sz, *b2 = b1 + sz;
    if (cnt < sz)
        sais(s1, sa, sz, t + len, b, b1 + sz, cnt);
    else
        for (i = 0; i < sz; i++) sa[s1[i]] = i;
    for (i = 0; i < sz; i++) b2[i] = b1[sa[i]];
    sort(s, sa, len, sz, sigma, t, b, cb, b2);
}

template <typename T>
inline void getHeight(T s, const int n, int *sa, int *rk, int *ht) {
    for (register int i = 1; i <= n; i++) rk[sa[i]] = i;
    for (register int i = 0, j = 0, k = 0; i < n; ht[rk[i++]] = k)
        for (k ? k-- : 0, j = sa[rk[i] - 1]; s[i + k] == s[j + k]; k++)
            ;
}

struct SuffixArray {
    char s[MAXN];
    int n, sa[MAXN], rk[MAXN], ht[MAXN];
    bool t[MAXN << 1];

    inline void build(const int sigma) {
        s[n] = 0, sais(s, sa, n + 1, t, rk, ht, sigma);
        rk[0] = ht[0] = 0, getHeight(s, n, sa, rk, ht);
    }
} d;

std::pair<int, int> q[MAXN], *l = q + 1, *r = q;

inline int calc(const std::pair<int, int> *p, const int x) {
    return std::max(p->second, x) - p->first + 1;
}

inline void solve() {
    d.n = fread(d.s, 1, MAXN, stdin);
    while (isspace(d.s[d.n - 1])) d.n--;

    d.build(127);
    for (register int i = 0, tmp; i < d.n; i++) {
        tmp = i + std::max(d.ht[d.rk[i]], d.ht[d.rk[i] + 1]);
        if (tmp < d.n) {
            while (l <= r && tmp - i <= r->second - r->first) r--;
            r++;
            r->first = i, r->second = tmp;
        }
        while (l < r && calc(l + 1, i) <= calc(l, i)) l++;
        io << calc(l, i) << '\n';
    }
}
}  // namespace

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    solve();
    return 0;
}