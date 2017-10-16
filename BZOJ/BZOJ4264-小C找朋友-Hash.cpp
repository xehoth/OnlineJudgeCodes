/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4264」小C找朋友 16-10-2017
 * Hash
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

typedef unsigned long long ulong;

inline ulong xorShift128Plus() {
    static ulong seed1 = 495;
    static ulong seed2 = 233;
    register ulong x = seed1;
    register const ulong y = seed2;
    seed1 = y, x ^= x << 23;
    seed2 = x ^ y ^ (x >> 17) ^ (y >> 26);
    return seed2 + y;
}

const int MAXN = 1000000;
ulong hash[MAXN + 1], val[MAXN + 1], ans, cnt;

struct Edge {
    int u, v;

    inline bool operator<(const Edge &x) const {
        return u == x.u && v < x.v && u < x.u;
    }

    inline bool operator==(const Edge &x) const { return u == x.u && v == x.v; }
} edge[MAXN + 1];

inline void solve() {
    register int n, m;
    io >> n >> m;
    for (register int i = 1; i <= n; i++) val[i] = xorShift128Plus();
    for (register int i = 1; i <= m; i++) {
        io >> edge[i].u >> edge[i].v;
        if (edge[i].u > edge[i].v) std::swap(edge[i].u, edge[i].v);
    }
    std::sort(edge + 1, edge + m + 1);
    m = std::unique(edge + 1, edge + m + 1) - edge - 1;
    for (register int i = 1; i <= m; i++) {
        if (edge[i].u != edge[i].v) {
            hash[edge[i].u] ^= val[edge[i].v];
            hash[edge[i].v] ^= val[edge[i].u];
        }
    }
    for (register int i = 1; i <= m; i++) {
        if (edge[i].u != edge[i].v && ((hash[edge[i].u] ^ val[edge[i].v]) ==
                                       (hash[edge[i].v] ^ val[edge[i].u])))
            ans++;
    }
    std::sort(hash + 1, hash + n + 1);
    for (register int i = 1; i <= n; i++) {
        cnt++;
        if (i == n || hash[i] != hash[i + 1])
            ans += cnt * (cnt - 1) >> 1, cnt = 0;
    }
    io << ans << '\n';
}
}

int main() {
    solve();
    return 0;
}
