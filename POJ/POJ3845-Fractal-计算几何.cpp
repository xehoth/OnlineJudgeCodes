/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「POJ 3845」Fractal 20-09-2017
 * 计算几何
 * @author xehoth
 */
#include <algorithm>
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

const double EPS = 1e-8;

struct Point {
    double x, y;

    Point(double x = 0, double y = 0) : x(x), y(y) {}

    inline Point operator+(const Point &p) const {
        return Point(x + p.x, y + p.y);
    }

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline void read() { io >> x >> y; }

    inline Point operator*(const double t) const { return Point(x * t, y * t); }
    inline Point operator/(const double t) const { return Point(x / t, y / t); }

    inline double dis(const Point &p) const {
        return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
    }

    inline double len() { return sqrt(x * x + y * y); }
};

inline int sign(double x) { return (x > EPS) - (x < -EPS); }

inline double dis(const Point &a, const Point &b) { return (a - b).len(); }

inline Point rotate(const Point &p, double angle,
                    const Point &o = Point(0, 0)) {
    register Point t = p - o;
    register double x = t.x * cos(angle) - t.y * sin(angle);
    register double y = t.y * cos(angle) + t.x * sin(angle);
    return Point(x, y) + o;
}

struct Task {
    Point p[MAXN], ans;
    double f[15], sum[MAXN];
    double len;
    int c, n, d;

    inline void dfs(const Point &a, const Point &b, double len, int dep) {
        if (dep == 0) {
            ans = (b - a) * len / dis(a, b) + a;
        } else {
            for (int i = 1; i < n; ++i) {
                if (sign(sum[i] * dis(a, b) / dis(p[0], p[n - 1]) * f[dep] -
                         len) < 0)
                    continue;
                double angle1 = atan2(p[n - 1].y - p[0].y, p[n - 1].x - p[0].x);
                double angle2 = atan2(b.y - a.y, b.x - a.x);
                Point o = rotate(p[i - 1], angle2 - angle1, p[0]);
                o = (o - p[0]) * dis(a, b) / dis(p[0], p[n - 1]) + a;
                Point t = rotate(p[i], angle2 - angle1, p[0]);
                t = (t - p[0]) * dis(a, b) / dis(p[0], p[n - 1]) + a;
                dfs(o, t,
                    len - sum[i - 1] * f[dep] * dis(a, b) / dis(p[0], p[n - 1]),
                    dep - 1);
                return;
            }
        }
    }

    inline void solve() {
        io >> c;
        while (c--) {
            io >> n;
            for (register int i = 0; i < n; i++) p[i].read();
            io >> d >> len;
            sum[0] = 0;
            for (register int i = 1; i < n; i++)
                sum[i] = sum[i - 1] + dis(p[i - 1], p[i]);
            f[1] = 1;
            register double tmp = sum[n - 1] / dis(p[0], p[n - 1]);
            for (register int i = 2; i <= d; i++) f[i] = f[i - 1] * tmp;
            dfs(p[0], p[n - 1], len * sum[n - 1] * f[d], d);

            io << '(' << ans.x << ',' << ans.y << ")\n";
        }
    }
} task;
}

int main() {
    task.solve();
    return 0;
}