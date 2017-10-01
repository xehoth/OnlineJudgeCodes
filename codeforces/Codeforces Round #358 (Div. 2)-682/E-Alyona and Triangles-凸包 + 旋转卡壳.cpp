/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「CF 682E」Alyona and Triangles 01-10-2017
 * 凸包 + 旋转卡壳
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 100000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
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

inline void read(char &x) {
    while (x = read(), isspace(x) && x != -1)
        ;
}

const int OUT_LEN = 100000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, oh - obuf, stdout), oh = obuf) : 0;
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
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    ~InputOutputStream() { flush(); }
} io;
}

namespace {

using IO::io;

#define long long long
const int MAXN = 5010;

struct Point {
    long x, y;

    Point(long x = 0, long y = 0) : x(x), y(y) {}

    inline Point operator+(const Point &p) const {
        return Point(x + p.x, y + p.y);
    }

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline long operator*(const Point &p) const { return x * p.y - y * p.x; }

    inline bool operator<(const Point &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }

    inline friend IO::InputOutputStream &operator>>(IO::InputOutputStream &io,
                                                    Point &p) {
        io >> p.x >> p.y;
        return io;
    }

    inline friend IO::InputOutputStream &operator<<(IO::InputOutputStream &io,
                                                    const Point &p) {
        io << p.x << ' ' << p.y << '\n';
        return io;
    }
} p[MAXN + 1], con[MAXN + 1];

inline int andrew(Point *p, Point *con, const int n) {
    std::sort(p, p + n);
    register int top = 0, k;
    for (register int i = 0; i < n; i++) {
        while (top > 1 &&
               (con[top - 1] - con[top - 2]) * (p[i] - con[top - 2]) < 0)
            top--;
        con[top++] = p[i];
    }
    k = top;
    for (register int i = n - 2; i >= 0; i--) {
        while (top > k &&
               (con[top - 1] - con[top - 2]) * (p[i] - con[top - 2]) < 0)
            top--;
        con[top++] = p[i];
    }
    return n > 1 ? --top : top;
}

inline bool check(const Point &p1, const Point &p2, const Point &p) {
    return p * p2 >= 0 && p * p1 <= 0;
}

int n, m;
long S;

inline void rotatingCalispers() {
    for (register int i = 0, k; i < m; i++) {
        k = i + 2;
        for (register int j = i + 1; j < m - 1; ++j) {
            while (k <= j ||
                   (con[i] - con[j]) * (con[(k + 1) % m] - con[k]) < 0)
                k++;
            if (check(con[j] - con[(j + m - 1) % m], con[(j + 1) % m] - con[j],
                      con[k] - con[i]) &&
                check(con[i] - con[(i + m - 1) % m], con[(i + 1) % m] - con[i],
                      con[j] - con[k])) {
                Point a = con[i], b = con[j], c = con[k];
                io << (a + (b - c)) << (a + (c - b)) << (b + (c - a));
                return;
            }
        }
    }
}

inline void solve() {
    io >> n >> S;
    for (register int i = 0; i < n; i++) io >> p[i];
    m = andrew(p, con, n);
    rotatingCalispers();
}
}

int main() {
    solve();
    return 0;
}