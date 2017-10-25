/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1806」矿工配餐 25-10-2017
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
        return read(x), *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        return print(x), *this;
    }

    ~InputOutputStream() { flush(); }
} io;
}

namespace {

const int MAXN = 10000;

using IO::io;

int f[2][4][4][4][4];

inline int id(const char c) {
    switch (c) {
        case 'M':
            return 1;
        case 'F':
            return 2;
        case 'B':
            return 3;
    }
}

inline int get(int a, int b, int c) {
    register int res = 1;
    if (a != 0 && a != b && a != c) res++;
    if (b != 0 && b != c) res++;
    return res;
}

inline void solve() {
    register int n;
    memset(f, -1, sizeof(f));
    f[0][0][0][0][0] = 0;
    io >> n;
    register char c;
    for (register int i = 0; i < n; i++) {
        io >> c, c = id(c);
        for (register int a1 = 0; a1 <= 3; a1++) {
            for (register int a2 = 0; a2 <= 3; a2++) {
                for (register int b1 = 0; b1 <= 3; b1++) {
                    for (register int b2 = 0; b2 <= 3; b2++) {
                        if (!~f[i & 1][a1][a2][b1][b2]) continue;
                        f[~i & 1][a2][c][b1][b2] =
                            std::max(f[~i & 1][a2][c][b1][b2],
                                     f[i & 1][a1][a2][b1][b2] + get(a1, a2, c));
                        f[~i & 1][a1][a2][b2][c] =
                            std::max(f[~i & 1][a1][a2][b2][c],
                                     f[i & 1][a1][a2][b1][b2] + get(b1, b2, c));
                    }
                }
            }
        }
    }
    register int ans = 0;
    for (register int a1 = 0; a1 <= 3; a1++) {
        for (register int a2 = 0; a2 <= 3; a2++) {
            for (register int b1 = 0; b1 <= 3; b1++) {
                for (register int b2 = 0; b2 <= 3; b2++) {
                    ans = std::max(ans, f[n & 1][a1][a2][b1][b2]);
                }
            }
        }
    }
    io << ans;
}
}

int main() {
    solve();
    return 0;
}