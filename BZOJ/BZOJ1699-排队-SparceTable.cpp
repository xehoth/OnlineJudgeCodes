/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1699」排队 08-09-2017
 * SparseTable
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

const int MAXN = 50000;
const int MAX_LOG = 16;

struct SparseTable {
    int min[MAX_LOG][MAXN + 1], max[MAX_LOG][MAXN + 1], logs[MAXN + 1];
    int bit;

    inline void init(const int n) {
        for (register int w = 1; w <= MAX_LOG; w++) {
            for (register int i = 1; i + (1 << w) - 1 <= n; i++) {
                min[w][i] =
                    std::min(min[w - 1][i], min[w - 1][i + (1 << w - 1)]);
                max[w][i] =
                    std::max(max[w - 1][i], max[w - 1][i + (1 << w - 1)]);
            }
        }
    }

    inline int queryMin(int l, int r) {
        bit = logs[r - l + 1];
        return std::min(min[bit][l], min[bit][r - (1 << bit) + 1]);
    }

    inline int queryMax(int l, int r) {
        bit = logs[r - l + 1];
        return std::max(max[bit][l], max[bit][r - (1 << bit) + 1]);
    }

    inline void solve() {
        register int n, m;
        io >> n >> m;
        logs[0] = -1;
        for (register int i = 1; i <= MAXN; i++) logs[i] = logs[i >> 1] + 1;
        for (register int i = 1; i <= n; i++)
            io >> min[0][i], max[0][i] = min[0][i];
        init(n);
        for (register int l, r; m--;) {
            io >> l >> r;
            io << queryMax(l, r) - queryMin(l, r) << '\n';
        }
    }
} task;
}

int main() {
    task.solve();
    return 0;
}
