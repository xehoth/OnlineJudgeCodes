/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「COGS 896」圈奶牛 19-09-2017
 * 凸包
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

struct Point {
    double x, y;

    Point(double x = 0, double y = 0) : x(x), y(y) {}

    inline bool operator<(const Point &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline double dis(const Point &p) const {
        return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
    }

    inline void read() { std::cin >> x >> y; }

    inline double operator*(const Point &p) const { return x * p.y - y * p.x; }
};

const int MAXN = 10000;

struct Task {
    inline int andrew(Point *p, Point *con, const int n) {
        std::sort(p, p + n);
        register int top = 0, k;
        for (register int i = 0; i < n; i++) {
            while (top > 1 &&
                   (con[top - 1] - con[top - 2]) * (p[i] - con[top - 2]) <= 0)
                top--;
            con[top++] = p[i];
        }
        k = top;
        for (register int i = n - 2; i >= 0; i--) {
            while (top > k &&
                   (con[top - 1] - con[top - 2]) * (p[i] - con[top - 2]) <= 0)
                top--;
            con[top++] = p[i];
        }
        return n > 1 ? --top : top;
    }

    Point p[MAXN + 1], con[MAXN + 1];

    inline void solve() {
        register int n;
        std::cin >> n;
        for (register int i = 0; i < n; i++) p[i].read();
        n = andrew(p, con, n);
        register double ans = 0;
        con[n] = con[0];
        for (register int i = 0; i < n; i++) ans += con[i].dis(con[i + 1]);
        std::cout << std::fixed << std::setprecision(2) << ans;
    }
} task;
}

int main() {
    freopen("fc.in", "r", stdin);
    freopen("fc.out", "w", stdout);
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    task.solve();
    return 0;
}