/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「Vijos 1891」学姐的逛街计划 04-09-2017
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

const int MAXN = 600;
const int MAXM = 1000;
const double EPS = 1e-8;
const double INF = 1e15;

struct Simplex {
    int n, m;
    double a[MAXM + 1][MAXN + 1];
    int q[MAXN + 1];

    inline void pivot(int l, int e) {
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
        for (;;) {
            register int l = 0, e = 0;
            register double min = INF;
            for (register int i = 1; i <= n; i++) {
                if (a[0][i] > EPS) {
                    e = i;
                    break;
                }
            }
            if (!e) break;
            for (register int i = 1; i <= m; i++)
                if (a[i][e] > EPS && a[i][0] / a[i][e] < min)
                    min = a[i][0] / a[i][e], l = i;
            if (!l) return false;
            pivot(l, e);
        }
        return true;
    }
} task;

inline void solve() {
    using namespace IO;
    register int n, k, m;
    read(n), read(k), m = n * 3;
    for (register int i = 1, t; i <= m; i++) read(t), task.a[0][i] = t;
    for (register int i = 1; i <= m - n + 1; i++) {
        task.a[i][0] = k;
        for (register int j = 1; j <= n; j++) task.a[i][i + j - 1] = 1;
    }
    task.m = m - n + 1, task.n = m;
    for (register int i = 1; i <= task.n; i++)
        task.a[++task.m][i] = 1, task.a[task.m][0] = 1;
    if (task.simplex()) {
        print((int)(-task.a[0][0] + 0.5));
    } else {
        print(-1);
    }
}
}

int main() {
    Simplex::solve();
    IO::flush();
    return 0;
}