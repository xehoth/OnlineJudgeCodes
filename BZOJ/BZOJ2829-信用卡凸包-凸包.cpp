/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2829」信用卡凸包 23-11-2017
 * 凸包
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

inline void read(double &t) {
    static char c;
    static bool iosig;
    register int x = 0;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    if (c == '.') {
        register long long y = 0, cnt = 1;
        for (c = read(); isdigit(c); c = read())
            y = y * 10 + (c ^ '0'), cnt *= 10;
        t = x + (double)y / cnt;
    } else {
        t = x;
    }
    iosig ? t = -t : 0;
}
}  // namespace IO

namespace {

using namespace IO;

int n;
double a, b, r;

const int MAXN = 100000;
const double PI = acos(-1);

struct Point {
    double x, y;

    Point() {}

    Point(const double x, const double y) : x(x), y(y) {}

    inline Point operator+(const Point &p) const {
        return Point(x + p.x, y + p.y);
    }

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline double operator*(const Point &p) const { return x * p.y - y * p.x; }

    inline bool operator<(const Point &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }

    inline Point rotate(double a) const {
        return Point(x * cos(a) - y * sin(a), x * sin(a) + y * cos(a));
    }

    inline double dis(const Point &p) const {
        return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
    }
} p[MAXN], con[MAXN];

inline int andrew(Point *p, const int n, Point *c) {
    std::sort(p, p + n);
    register int top = 0, k;
    for (register int i = 0; i < n; i++) {
        while (top > 1 && (c[top - 1] - c[top - 2]) * (p[i] - c[top - 2]) <= 0)
            top--;
        c[top++] = p[i];
    }
    k = top;
    for (register int i = n - 2; i >= 0; i--) {
        while (top > k && (c[top - 1] - c[top - 2]) * (p[i] - c[top - 2]) <= 0)
            top--;
        c[top++] = p[i];
    }
    return n > 1 ? --top : top;
}

inline void solve() {
    read(n), read(a), read(b), read(r);
    a -= 2 * r, b -= 2 * r;
    register double x, y, rad;
    register int cnt = 0;
    for (register int i = 1; i <= n; i++) {
        read(x), read(y), read(rad);
        const Point t = Point(x, y);
        p[cnt++] = t + (Point(x - b / 2, y + a / 2) - t).rotate(rad);
        p[cnt++] = t + (Point(x + b / 2, y + a / 2) - t).rotate(rad);
        p[cnt++] = t + (Point(x - b / 2, y - a / 2) - t).rotate(rad);
        p[cnt++] = t + (Point(x + b / 2, y - a / 2) - t).rotate(rad);
    }
    register int top = andrew(p, cnt, con);
    con[top] = con[0];
    register double ans = 0;
    for (register int i = 0; i < top; i++) ans += con[i].dis(con[i + 1]);
    printf("%.2f", ans + 2 * PI * r);
}
}  // namespace

int main() {
    solve();
    return 0;
}