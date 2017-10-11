/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3673」可持久化并查集 by zky 11-10-2017
 * rope
 * @author xehoth
 */
#include <bits/stdc++.h>
#include <ext/rope>

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
const int MAXN = 20000;

__gnu_cxx::rope<int> *fa[MAXN + 1];
int f[MAXN + 1];
inline int get(int num, int x) {
    if (fa[num]->operator[](x) == x) return x;
    register int y = get(num, fa[num]->operator[](x));
    if (y == fa[num]->operator[](x)) return y;
    fa[num]->replace(x, y);
    return y;
}

inline void put(int num, int x, int y) {
    x = get(num, x), y = get(num, y);
    fa[num]->replace(x, y);
}

inline void solve() {
    register int n, m;
    io >> n >> m;
    for (register int i = 1; i <= n; i++) f[i] = i;
    fa[0] = new __gnu_cxx::rope<int>(f, f + n + 1);
    for (register int i = 1, cmd, a, b; i <= m; i++) {
        fa[i] = new __gnu_cxx::rope<int>(*fa[i - 1]);
        io >> cmd;
        switch (cmd) {
            case 1: {
                io >> a >> b, put(i, a, b);
                break;
            }
            case 2: {
                io >> a, fa[i] = fa[a];
                break;
            }
            case 3: {
                io >> a >> b, io << (get(i, a) == get(i, b)) << '\n';
                break;
            }
        }
    }
}
}

int main() {
    solve();
    return 0;
}