/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3376」Cube Stacking 方块游戏 11-10-2017
 * 带权并查集
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

const int MAXN = 300000;

int h[MAXN + 1], fa[MAXN + 1], d[MAXN + 1];

inline int get(int x) {
    if (x == fa[x]) return x;
    register int y = get(fa[x]);
    d[x] += d[fa[x]], fa[x] = y;
    return y;
}

inline void put(int x, int y) { h[get(x)] = h[get(y)], fa[y] = x, d[y] = 1; }

inline void solve() {
    register int n;
    for (register int i = 1; i <= MAXN; i++) fa[i] = h[i] = i;
    io >> n;
    register char cmd;
    for (register int x, y; n--;) {
        io >> cmd;
        switch (cmd) {
            case 'M': {
                io >> x >> y;
                put(h[get(y)], get(x));
                break;
            }
            case 'C': {
                io >> x;
                get(x);
                io << d[x] << '\n';
                break;
            }
        }
    }
}
}  // namespace

int main() {
    solve();
    return 0;
}