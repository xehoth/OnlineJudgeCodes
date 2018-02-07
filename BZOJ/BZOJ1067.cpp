/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1067」17-04-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    if (s == t) {
        t = (s = buf) + fread(buf, 1, IN_LEN, stdin);
        if (s == t) return -1;
    }
    return *s++;
}

template <class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        if (c == '-') iosig = true;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    if (iosig) x = -x;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}

template <class T>
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

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

namespace SegmentTree {

const int MAXN = 50001;

int d[131072 + 1], x[MAXN], y[MAXN];
int M;

inline void build(const int n, const int *a) {
    for (M = 1; M < n + 2; M <<= 1)
        ;
    for (register int i = 1; i <= n; i++) d[i + M] = a[i];
    for (register int i = M - 1; i; i--)
        d[i] = std::max(d[i << 1], d[i << 1 | 1]);
}

inline int query(int s, int t) {
    register int ret = INT_MIN;
    for (s = s + M - 1, t = t + M + 1; s ^ t ^ 1; s >>= 1, t >>= 1) {
        (~s & 1) ? ret = std::max(ret, d[s ^ 1]) : 0;
        (t & 1) ? ret = std::max(ret, d[t ^ 1]) : 0;
    }
    return ret;
}

inline void solve() {
    register int n, q;
    read(n);
    x[n] = INT_MAX;
    for (register int i = 0; i < n; i++) read(x[i]), read(y[i]);
    build(n, y - 1);
    read(q);
    while (q--) {
        register int a, b;
        read(a), read(b);
        register int l = std::lower_bound(x, x + n, a) - x;
        register int r = std::upper_bound(x, x + n, b) - x - 1;
        if (a > b) {
            print('f'), print('a'), print('l'), print('s'), print('e'),
                print('\n');
            continue;
        }
        register bool lk = x[l] == a, rk = x[r] == b;
        register int max = query(l + lk + 1, r - rk + 1);
        if (lk && max >= y[l] || rk && max >= y[r] || lk && rk && y[l] < y[r]) {
            print('f'), print('a'), print('l'), print('s'), print('e'),
                print('\n');
            continue;
        }
        if (r - l == b - a && lk && rk && y[l] >= y[r]) {
            print('t'), print('r'), print('u'), print('e'), print('\n');
            continue;
        }
        print('m'), print('a'), print('y'), print('b'), print('e'), print('\n');
    }
}
}  // namespace SegmentTree

int main() {
    // freopen("in.in", "r", stdin);
    SegmentTree::solve();
    flush();
    return 0;
}
