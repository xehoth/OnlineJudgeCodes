/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1061」26-03-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

inline int read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *h, *t;
    if (h == t) {
        t = (h = buf) + fread(buf, 1, IN_LEN, stdin);
        if (h == t) return -1;
    }
    return *h++;
}
template <class T>
inline bool read(T &x) {
    static bool iosig = 0;
    static char c;
    for (iosig = 0, c = read(); !isdigit(c); c = read()) {
        if (c == -1) return false;
        if (c == '-') iosig = 1;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    if (iosig) x = -x;
    return true;
}

const int OUT_LEN = 10000000;
char obuf[OUT_LEN], *oh = obuf;
inline void print(const char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}

template <class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (!x)
        print('0');
    else {
        if (x < 0) print('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print((char)buf[cnt--]);
    }
}
inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

#define long long long

const int MAXN = 1005;
const int MAXM = 1e4 + 5;

double a[MAXM][MAXN];
int q[MAXN];

int n, m;

int id[MAXN << 1];

const double EPS = 1e-7;
const double INF = 1e15;
inline void pivot(int l, int e) {
    std::swap(id[n + l], id[e]);
    double t = a[l][e];
    a[l][e] = 1;
    for (register int j = 0; j <= n; j++) a[l][j] /= t;
    register int p = 0;
    for (register int j = 0; j <= n; j++)
        if (std::abs(a[l][j]) > EPS) q[++p] = j;
    for (register int i = 0; i <= m; i++) {
        if (i != l && std::abs(a[i][e]) > EPS) {
            t = a[i][e], a[i][e] = 0;
            for (register int j = 1; j <= p; j++) a[i][q[j]] -= t * a[l][q[j]];
        }
    }
}

inline void simplex() {
    while (true) {
        register int l = 0, e = 0;
        double min = INF;
        for (register int j = 1; j <= n; j++) {
            if (a[0][j] > EPS) {
                e = j;
                break;
            }
        }
        if (!e) break;
        for (register int i = 1; i <= m; i++)
            if (a[i][e] > EPS && a[i][0] / a[i][e] < min)
                min = a[i][0] / a[i][e], l = i;
        if (!l) {
            // puts("Unbounded");
            return;  // false;
        }
        pivot(l, e);
    }
    // return true;
}

int main() {
    // freopen("in.in", "r", stdin);
    read(n), read(m);
    for (register int i = 1, t; i <= n; i++) read(t), a[0][i] = t;
    for (register int i = 1, x, y, z; i <= m; i++) {
        read(x), read(y), read(z);
        for (register int j = x; j <= y; j++) a[i][j] = 1;
        a[i][0] = z;
    }
    simplex();
    print(int(-a[0][0] + 0.5));
    flush();
    return 0;
}
