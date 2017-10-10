/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4145」The Prices 10-10-2017
 * DP
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

namespace {

using IO::io;

const int MAXN = 100;
const int MAXM = 16;
const int MAX_STATUS = 1 << 16;

inline void solve() {
    register int n, m;
    io >> n >> m;
    static int c[MAXN][MAXM], d[MAXN];
    for (register int i = 0; i < n; i++) {
        io >> d[i];
        for (register int j = 0; j < m; j++) io >> c[i][j];
    }
    static int f[MAXN][MAX_STATUS];
    for (register int j = 0; j < (1 << m); j++) {
        f[0][j] = d[0];
        for (register int k = 0; k < m; k++) {
            if (j & (1 << k)) f[0][j] += c[0][k];
        }
    }
    f[0][0] = 0;
    for (register int i = 1; i < n; i++) {
        for (register int j = 0; j < (1 << m); j++)
            f[i][j] = f[i - 1][j] + d[i];
        for (register int j = 0; j < (1 << m); j++) {
            for (register int k = 0; k < m; k++) {
                if (j & (1 << k))
                    f[i][j] = std::min(f[i][j], f[i][j ^ (1 << k)] + c[i][k]);
            }
        }
        for (register int j = 0; j < (1 << m); j++)
            f[i][j] = std::min(f[i][j], f[i - 1][j]);
    }
    io << f[n - 1][(1 << m) - 1];
}
}

int main() {
    solve();
    return 0;
}