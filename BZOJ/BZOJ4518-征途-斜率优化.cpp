/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4518」征途 26-09-2017
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

const int MAXN = 3000;

int n, m;

int a[MAXN + 1], sum[MAXN + 1];
int f[MAXN + 1], g[MAXN + 1];

inline double slope(const int a, const int b) {
    return (double)(g[a] - g[b] + sum[a] * sum[a] - sum[b] * sum[b]) /
           (double)(sum[a] - sum[b]);
}

inline void solve() {
    io >> n >> m;
    for (register int i = 1; i <= n; i++) io >> a[i];
    for (register int i = 1; i <= n; i++) sum[i] = sum[i - 1] + a[i];
    memset(f + 1, 0x3f, sizeof(int) * n);
    for (register int j = 1; j <= m; j++) {
        memcpy(g, f, sizeof(f));
        memset(f, 0, sizeof(f));
        static int q[MAXN + 1];
        register int *l = q, *r = q;
        *r = 0;
        for (register int i = 1; i <= n; i++) {
            while (l < r && slope(*(l + 1), *l) < 2 * sum[i]) l++;
            f[i] = g[*l] + (sum[i] - sum[*l]) * (sum[i] - sum[*l]);
            while (l < r && slope(*r, *(r - 1)) > slope(*r, i)) r--;
            *++r = i;
        }
    }

    io << f[n] * m - sum[n] * sum[n];
}

#undef long
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}
