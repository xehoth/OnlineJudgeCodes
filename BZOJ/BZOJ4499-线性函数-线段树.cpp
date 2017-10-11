/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4499」线性函数 11-10-2017
 * 线段树
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

const int MOD = 1e9 + 7;
const int MAXN = 200000;
#define long long long

struct Node {
    int k, b;

    Node(int k = 1, int b = 0) : k(k), b(b) {}

    inline void read() { io >> k >> b; }
} d[MAXN * 4 + 1];

inline Node operator+(const Node &l, const Node &r) {
    return Node((long)l.k * r.k % MOD, ((long)r.k * l.b + r.b) % MOD);
}

int M;

inline void solve() {
    register int n, m;
    io >> n >> m;
    for (M = 1; M < n + 2; M <<= 1)
        ;
    for (register int i = 1; i <= n; i++) d[i + M].read();
    for (register int i = M - 1; i; i--) d[i] = d[i << 1] + d[i << 1 | 1];
    register char cmd;
    for (register int l, r, x; m--;) {
        io >> cmd;
        switch (cmd) {
            case 'M': {
                io >> x;
                register Node tmp;
                tmp.read();
                for (d[x += M] = tmp; x;)
                    d[x >>= 1] = d[x << 1] + d[x << 1 | 1];
                break;
            }
            case 'Q': {
                io >> l >> r >> x;
                register Node L, R;
                for (l += M - 1, r += M + 1; l ^ r ^ 1; l >>= 1, r >>= 1) {
                    if (~l & 1) L = L + d[l ^ 1];
                    if (r & 1) R = d[r ^ 1] + R;
                }
                L = L + R;
                io << ((long)L.k * x + L.b) % MOD << '\n';
                break;
            }
        }
    }
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}