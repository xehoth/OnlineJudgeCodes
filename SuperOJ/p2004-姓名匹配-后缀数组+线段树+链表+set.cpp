/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 2004」姓名匹配 23-10-2017
 * 后缀数组 + 线段树 + 链表 + std::set
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline bool read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return false;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
    return true;
}

inline void read(char &c) {
    while (c = read(), isspace(c) && c != -1)
        ;
}

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = 0;
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = 0;
    return s;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <typename T>
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

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }

    ~InputOutputStream() { flush(); }
} io;
}

/**
 * 1000000 * 4
 * 15 MB
 */
namespace SegmentTree {

const int MAXN = 1000000;

int d[MAXN * 4], M;

inline int optMin(int a, int b) { return a < b ? a : b; }

inline void build(const int n, const int *a) {
    for (M = 1; M < n + 2; M <<= 1)
        ;
    for (register int i = 1; i <= n; i++) d[i + M] = a[i];
    for (register int i = M - 1; i; i--)
        d[i] = optMin(d[i << 1], d[i << 1 | 1]);
}

inline int query(register int s, register int t) {
    register int ret = INT_MAX;
    for (s = s + M - 1, t = t + M + 1; s ^ t ^ 1; s >>= 1, t >>= 1) {
        (~s & 1) ? ret = optMin(ret, d[s ^ 1]) : 0;
        (t & 1) ? ret = optMin(ret, d[t ^ 1]) : 0;
    }
    return ret;
}
}

namespace {

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
    register int i, j, x, p = -1, sz = 0, cnt = 0, *cb = b + sigma;
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
        for (k ? k-- : 0, j = sa[rk[i] - 1]; s[j + k] == s[i + k]; k++)
            ;
}

const int MAXM = 100010;
const int MAX_LEN = 800010;
const int MAXN = MAXM * 2 + MAX_LEN + 100;

struct SuffixArray {
    int sa[MAXN], rk[MAXN], ht[MAXN], s[MAXN];
    int n;
    bool t[MAXN << 1];

    inline void build(const int sigma) {
        s[n] = 0, sais(s, sa, n + 1, t, rk, ht, sigma);
        rk[0] = ht[0] = 0, getHeight(s, n, sa, rk, ht);
    }

    inline int &operator[](const int i) { return s[i]; }
} suffixArray;

using IO::io;

int *sa, *rk, *ht, pos1[MAXN + 1], pos2[MAXN + 1], lcp[MAXN + 1];
bool t[MAXN + 1];
int next[MAXN + 1], prev[MAXN + 1];

char buf[MAXN * 2 + 1];

typedef std::pair<int, int> Pair;

Pair d[MAXN * 2];

inline bool type(const Pair &x) { return x.first == rk[pos2[x.second]]; }

inline void solve() {
#ifdef DBG
    std::cerr << "static memory = "
              << sizeof(SegmentTree::d) + sizeof(suffixArray) + sizeof(pos1) +
                     sizeof(pos2) + sizeof(lcp) + sizeof(t) + sizeof(next) +
                     sizeof(prev) + sizeof(buf) + sizeof(d)
              << std::endl;
    if (sizeof(SegmentTree::d) + sizeof(suffixArray) + sizeof(pos1) +
            sizeof(pos2) + sizeof(lcp) + sizeof(t) + sizeof(next) +
            sizeof(prev) + sizeof(buf) + sizeof(d) >
        128 * 1024 * 1024) {
        std::cerr << "MLE" << std::endl;
    }
#endif
    register int &sn = suffixArray.n;
    register int n;
    io >> n;
    for (register int i = 0, len; i < n; i++) {
        len = IO::read(buf), pos1[i] = sn;
        for (register char *c = buf; *c; c++) suffixArray[sn++] = *c;
        suffixArray[sn++] = 256 + i;
    }
    for (register int i = 0, len; i < n; i++) {
        len = IO::read(buf), pos2[i] = sn;
        for (register char *c = buf; *c; c++) suffixArray[sn++] = *c;
        suffixArray[sn++] = 256 + n + i;
    }
    suffixArray.build(suffixArray[sn - 1] + 1);
    sa = suffixArray.sa, rk = suffixArray.rk, ht = suffixArray.ht;
    register int cnt = 0;
    for (register int i = 0; i < n; i++) {
        d[++cnt] = Pair(rk[pos1[i]], i);
        d[++cnt] = Pair(rk[pos2[i]], i);
    }
    std::sort(d + 1, d + cnt + 1);
    SegmentTree::build(sn, ht);
    for (register int i = 2; i <= cnt; i++)
        lcp[i] = SegmentTree::query(d[i - 1].first + 1, d[i].first);
    for (register int i = 1; i <= cnt; i++) t[i] = type(d[i]);
    register int head = 0, tail = cnt + 1;
    for (register int i = 0; i <= cnt; i++) next[i] = i + 1, prev[i + 1] = i;
    static std::set<Pair> set;
    for (register int i = 2; i <= cnt; i++)
        if (t[i] != t[i - 1]) set.insert(Pair(-lcp[i], i));
    register long long sum = 0;
    while (!set.empty()) {
        register std::set<Pair>::iterator it = set.begin();
        Pair tmp = *it;
        set.erase(it);
        sum -= tmp.first;
        register int x = tmp.second;
        register int prevX = prev[x], nextX = next[x], prevPrevX = prev[prevX];
        if (nextX != tail && t[x] != t[nextX])
            set.erase(Pair(-lcp[nextX], nextX));
        if (prevPrevX != head && t[prevX] != t[prevPrevX])
            set.erase(Pair(-lcp[prevX], prevX));
        if (nextX != tail) {
            lcp[nextX] = SegmentTree::optMin(lcp[nextX], lcp[x]);
            if (prevPrevX != head) {
                lcp[nextX] = SegmentTree::optMin(lcp[nextX], lcp[prevX]);
                if (t[nextX] != t[prevPrevX])
                    set.insert(Pair(-lcp[nextX], nextX));
            }
        }
        prev[nextX] = prevPrevX;
        next[prevPrevX] = nextX;
    }
    io << sum;
}
}

int main() {
    // freopen("match.in", "r", stdin);
    // freopen("match.out", "w", stdout);
    solve();
    return 0;
}