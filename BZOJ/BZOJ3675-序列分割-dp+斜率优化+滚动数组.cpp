/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3675」序列分割 26-09-2017
 * dp + 斜率优化 + 滚动数组
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

const int MAXN = 100010;

long sum[MAXN + 1], f[2][MAXN + 1];
int a[MAXN + 1];

bool flag;

inline double slope(const int a, const int b) {
    return (double)(f[flag ^ 1][b] - f[flag ^ 1][a] + sum[a] * sum[a] -
                    sum[b] * sum[b]) /
           (double)(sum[a] - sum[b]);
}

inline void solve() {
    register int n, k;
    io >> n >> k;
    for (register int i = 1; i <= n; i++) {
        io >> a[i];
        if (a[i] != 0)
            sum[i] = sum[i - 1] + a[i];
        else
            n--, i--;
    }
    for (register int j = 1; j <= k; j++) {
        flag ^= 1;
        static int q[MAXN + 1];
        register int *l = q, *r = q;
        for (register int i = j; i <= n; i++) {
            while (l < r && slope(*(l + 1), *l) < sum[i]) l++;
            f[flag][i] = f[flag ^ 1][*l] + (sum[i] - sum[*l]) * sum[*l];
            while (l < r && slope(*(r - 1), *r) >= slope(*r, i)) r--;
            *++r = i;
        }
    }
    io << f[k & 1][n];
}

#undef long
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}
