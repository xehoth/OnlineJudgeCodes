/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 5099」Pionek 04-02-2018
 * 极角排序
 * @author xehoth
 */
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>

struct InputOutputStream {
    enum { SIZE = 1 << 18 | 1 };
    char ibuf[SIZE], *s, *t;
    InputOutputStream() : s(), t() {}

    inline char read() {
        return (s == t) && (t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin)),
               s == t ? -1 : *s++;
    }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        iosig && (x = -x);
        return *this;
    }
} io;

typedef unsigned long long ull;
const double EPS = 1e-9;
const double PI = 3.1415926535897932384626;
const double PI2 = PI * 2;

int n;

struct Point {
    int x, y;
    double arg;

    Point(int x, int y) : x(x), y(y) {}
    Point() {}

    inline bool operator<(const Point &p) const { return arg < p.arg; }
};

std::vector<Point> pts;

template <typename T>
inline void relax(T &x, const T v) {
    x < v && (x = v);
}

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    io >> n;
    pts.reserve(2 * n);
    pts.resize(n);
    for (int i = 0; i < n; i++) {
        io >> pts[i].x >> pts[i].y;
        if (pts[i].x == 0 && pts[i].y == 0) {
            i--;
            n--;
            continue;
        }
        pts[i].arg = atan2(pts[i].y, pts[i].x);
    }
    pts.resize(2 * n);
    std::sort(pts.begin(), pts.begin() + n);
    int sumX = 0, sumY = 0;
    unsigned long long ans = 0;
    for (int l = 0, r = 0; l < n; l++) {
        pts[l + n] = pts[l];
        pts[l + n].arg += PI2;
        for (; r < n + l && pts[r].arg - pts[l].arg < PI + EPS; r++) {
            sumX += pts[r].x;
            sumY += pts[r].y;
            relax(ans, (ull)sumX * sumX + (ull)sumY * sumY);
        }
        sumX -= pts[l].x;
        sumY -= pts[l].y;
        relax(ans, (ull)sumX * sumX + (ull)sumY * sumY);
    }
    std::cout << ans;
    return 0;
}