/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1037」生日聚会Party 10-10-2017
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
}  // namespace IO

namespace {

using IO::io;

const int MAXN = 150;
const int MAXK = 20;
const int MOD = 12345678;

using IO::io;

inline void solve() {
    register int n, m, k;
    io >> n >> m >> k;
    static int f[MAXN * 2 + 1][MAXN + 1][MAXK + 2][MAXK + 2];
    f[0][0][0][0] = 1;
    for (register int i = 0; i < n + m; i++) {
        for (register int j = 0; j <= n; j++) {
            for (register int p = 0; p <= k; p++) {
                for (register int q = 0; q <= k; q++) {
                    (f[i + 1][j + 1][p + 1][std::max(q - 1, 0)] +=
                     f[i][j][p][q]) %= MOD;
                    (f[i + 1][j][std::max(p - 1, 0)][q + 1] += f[i][j][p][q]) %=
                        MOD;
                }
            }
        }
    }

    register int ans = 0;
    for (register int p = 0; p <= k; p++)
        for (register int q = 0; q <= k; q++) (ans += f[n + m][n][p][q]) %= MOD;
    io << ans;
}
}  // namespace

int main() {
    solve();
    return 0;
}