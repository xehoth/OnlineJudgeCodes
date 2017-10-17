/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 1989」购买板凳 17-10-2017
 * 扫描线
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

const int MAXN = 100000;

struct Node {
    int x, val;

    inline bool operator<(const Node &p) const { return x < p.x; }

    Node(int x = 0, int val = 0) : x(x), val(val) {}
} data[MAXN * 2 + 5];

int cnt;

inline void solve() {
    register int n;
    io >> n;
    for (register int i = 1, x, a, b, c, d; i <= n; i++) {
        io >> x >> a >> b >> c >> d;
        data[++cnt] = Node(a * 60 + b, x);
        data[++cnt] = Node(c * 60 + d, -x);
    }
    std::sort(data + 1, data + cnt + 1);
    register int max = 0, now = 0;
    for (register int i = 1; i <= cnt; i++) {
        now += data[i].val;
        while (i + 1 <= cnt && data[i].x == data[i + 1].x)
            i++, now += data[i].val;
        max = std::max(max, now);
    }
    io << max;
}
}

int main() {
    // freopen("chair.in", "r", stdin);
    // freopen("chair.out", "w", stdout);
    solve();
    return 0;
}