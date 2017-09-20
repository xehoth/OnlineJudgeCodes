/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「POJ 2420」A Star not a Tree? 20-09-2017
 * 模拟退火
 * @author xehoth
 */
#include <algorithm>
#include <cfloat>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>

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

inline void read(double &x) {
    static char buf[40];
    read(buf);
    sscanf(buf, "%lf", &x);
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

inline void print(double x) {
    static char buf[40];
    sprintf(buf, "%.10f", x);
    print((const char *)buf);
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
const int MAXN = 110;

typedef unsigned int uint;

inline uint nextUint() {
    static uint seed = 495;
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}

struct Point {
    double x, y;

    Point(double x = 0, double y = 0) : x(x), y(y) {}

    inline double operator*(const Point &p) const { return x * p.y - y * p.x; }

    inline Point operator+(const Point &p) const {
        return Point(x + p.x, y + p.y);
    }

    inline void operator+=(const Point &p) { x += p.x, y += p.y; }

    inline void operator/=(const int i) { x /= i, y /= i; }

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline void read() {
        static int t;
        io >> t, x = t, io >> t, y = t;
    }

    inline double dis(const Point &p) const {
        return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
    }
};

const double EPS = 1e-2;
const double DROP = 0.97;
const double PI = acos(-1);
const double PI2 = PI * 2;

struct SimulatedAnnealing {
    Point p[MAXN + 1];
    Point min;
    int n;
    double ans;

    inline double check(const Point &p) {
        register double ret = 0;
        for (register int i = 0; i < n; i++) ret += p.dis(this->p[i]);
        if (ret < ans) ans = ret, min = p;
        return ret;
    }

    inline void simulatedAnnealing() {
        ans = DBL_MAX;
        Point now;
        for (register int i = 0; i < n; i++) now += p[i];
        now /= n;
        for (double t = 10000.0; t > EPS; t *= DROP) {
            Point tmp(now.x + t * sin(nextUint()), now.y + t * cos(nextUint()));
            register double delta = check(now) - check(tmp);
            if (delta >= 0 || exp(delta / t) * UINT_MAX >= nextUint())
                now = tmp;
        }
        for (register int i = 1; i <= 1000; i++) {
            check(Point(min.x + 0.01 * cos(nextUint()),
                        min.y + 0.01 * sin(nextUint())));
        }
    }

    inline void solve() {
        while (IO::read(n)) {
            for (register int i = 0; i < n; i++) p[i].read();
            simulatedAnnealing();
            io << (int)(ans + 0.5);
        }
    }
} task;
}

int main() {
    task.solve();
    return 0;
}