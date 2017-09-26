/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1096」仓库建设 26-09-2017
 * dp + 斜率优化
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
    for (iosig = false, c = read(); !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
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

const int OUT_LEN = 10000000;

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

namespace {

#define long long long

using IO::io;

const int MAXN = 1000010;

long f[MAXN + 1], sum[MAXN + 1], s[MAXN + 1];
long x[MAXN + 1], c[MAXN + 1], p[MAXN + 1];

inline long up(const int a, const int b) { return f[a] - f[b] + s[a] - s[b]; }

inline long down(const int a, const int b) { return sum[a] - sum[b]; }

inline void solve() {
    register int n;
    io >> n;
    for (register int i = 1; i <= n; i++) io >> x[i] >> p[i] >> c[i];
    for (register int i = 1; i <= n; i++)
        sum[i] = sum[i - 1] + p[i], s[i] = s[i - 1] + p[i] * x[i];
    static long q[MAXN + 1];
    register long *l = q, *r = q;
    for (register int i = 1; i <= n; i++) {
        while (l < r && up(*(l + 1), *l) < x[i] * down(*(l + 1), *l)) l++;
        f[i] = f[*l] + (sum[i] - sum[*l]) * x[i] - (s[i] - s[*l]) + c[i];
        while (l < r &&
               up(*r, *(r - 1)) * down(i, *r) > up(i, *r) * down(*r, *(r - 1)))
            r--;
        *++r = i;
    }
    io << f[n];
}

#undef long
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}
