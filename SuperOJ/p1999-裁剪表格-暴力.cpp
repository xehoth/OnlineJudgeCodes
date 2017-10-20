/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 1999」裁剪表格 20-10-2017
 * 暴力
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

namespace DataAccess {

const int MAXN = 1000;

int mat[MAXN + 1][MAXN + 1];
int n, m, q;

using IO::io;
}

namespace Task1 {

using namespace DataAccess;

inline void solve() {
    for (register int i = 1; i <= n; i++) {
        for (register int j = 1; j < m; j++) io << "1 ";
        io << "1\n";
    }
}
}

namespace Task2 {

using namespace DataAccess;

inline void solve() {
    for (register int r1, c1, r2, c2, h, w; q--;) {
        io >> r1 >> c1 >> r2 >> c2 >> h >> w;
        for (register int i = 1; i <= h; i++)
            for (register int j = 1; j <= w; j++)
                std::swap(mat[r1 + i - 1][c1 + j - 1],
                          mat[r2 + i - 1][c2 + j - 1]);
    }
    for (register int i = 1; i <= n; i++) {
        for (register int j = 1; j <= m; j++) io << mat[i][j] << ' ';
        io << '\n';
    }
}
}

namespace {

using namespace DataAccess;

inline void solve() {
    io >> n >> m >> q;
    register bool flagTask1 = true;
    for (register int i = 1; i <= n; i++) {
        for (register int j = 1; j <= m; j++) {
            io >> mat[i][j];
            if (mat[i][j] != 1) {
                flagTask1 = false;
            }
        }
    }
    if (flagTask1) {
        Task1::solve();
        return;
    }
    Task2::solve();
}
}

int main() {
    // freopen("table.in", "r", stdin);
    // freopen("table.out", "w", stdout);
    solve();
    return 0;
}