/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3112」「ZJOI 2013」防守战线 18-08-2017
 * 单纯形
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

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}

namespace Simplex {

const int MAXN = 1000;
const int MAXM = 10000;
const double EPS = 1e-8;
const double INF = 1e15;

struct Simplex {
    int n, m;
    double a[MAXN + 5][MAXM + 5];
    int q[MAXM + 5], id[MAXN + MAXM + 5];

    inline void pivot(int l, int e) {
        std::swap(id[n + l], id[e]);
        register double t = a[l][e];
        a[l][e] = 1;
        for (register int i = 0; i <= n; i++) a[l][i] /= t;
        register int p = 0;
        for (register int i = 0; i <= n; i++)
            if (fabs(a[l][i]) > EPS) q[++p] = i;
        for (register int i = 0; i <= m; i++) {
            if (i != l && fabs(a[i][e]) > EPS) {
                t = a[i][e], a[i][e] = 0;
                for (register int j = 1; j <= p; j++)
                    a[i][q[j]] -= t * a[l][q[j]];
            }
        }
    }

    inline bool simplex() {
        for (register int i = 1; i <= n; i++) id[i] = i;
        for (;;) {
            register int l = 0, e = 0;
            register double min = INF;
            for (register int j = 1; j <= n; j++) {
                if (a[0][j] > EPS) {
                    e = j;
                    break;
                }
            }
            if (!e) break;
            for (register int i = 1; i <= m; i++) {
                if (a[i][e] > EPS && a[i][0] / a[i][e] < min)
                    min = a[i][0] / a[i][e], l = i;
            }
            if (!l) return false;
            pivot(l, e);
        }
    }
} task;

inline void solve() {
    using namespace IO;
    read(task.m), read(task.n);
    register int n = task.n, m = task.m;
    for (register int i = 1, t; i <= m; i++) read(t), task.a[i][0] = t;
    for (register int i = 1, l, r, d; i <= n; i++) {
        read(l), read(r), read(d);
        for (register int j = l; j <= r; j++) task.a[j][i] = 1;
        task.a[0][i] = d;
    }
    task.simplex();
    print((int)(-task.a[0][0] + 0.5));
}
}

int main() {
    Simplex::solve();
    IO::flush();
    return 0;
}