/**
 * Copyright (c) 2016-2018, xehoth
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * 「BZOJ 1137」Wsp 岛屿 24-02-2018
 * 单调栈
 * @author xehoth
 */
#include <cctype>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>

struct InputStream {
    enum { SIZE = 1 << 18 | 1 };
    char ibuf[SIZE], *s, *t;

    InputStream() : s(), t() {}

    inline char read() {
        return (s == t) && (t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin)),
               s == t ? -1 : *s++;
    }

    template <typename T>
    inline InputStream &operator>>(T &x) {
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

struct Point {
    double x, y;

    Point() {}

    Point(const double x, const double y) : x(x), y(y) {}

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline Point operator+(const Point &p) const {
        return Point(x + p.x, y + p.y);
    }

    inline double operator*(const Point &p) const { return x * p.y - y * p.x; }

    inline Point operator*(double p) const { return Point(x * p, y * p); }

    inline void read() {
        static int v;
        io >> v;
        x = v;
        io >> v;
        y = v;
    }

    inline double norm() const { return sqrt(x * x + y * y); }
};

struct Line {
    Point s, t;

    Line() {}

    Line(const Point &s, const Point &t) : s(s), t(t) {}

    inline Point intersect(const Line &l) const {
        return s +
               (t - s) * (((l.s - s) * (l.t - s)) / ((t - s) * (l.t - l.s)));
    }
};

std::vector<Point> p;
std::vector<Line> q;

inline void add(int i, int j) {
    Line tmp(p[i], p[j]);
    while ((int)q.size() > 1 && (p[j] - p[i]) * (q[(int)q.size() - 1].intersect(
                                                     q[(int)q.size() - 2]) -
                                                 p[i]) >
                                    0)
        q.pop_back();
    q.push_back(tmp);
}

int main() {
    int n, m;
    io >> n >> m;
    std::vector<std::vector<int> > g(n + 1);
    p.resize(n + 1);
    for (int i = 1; i <= n; i++) p[i].read();
    for (int i = 0, u, v; i < m; i++) {
        io >> u >> v;
        if (u < v)
            g[u].push_back(v);
        else
            g[v].push_back(u);
    }
    std::vector<int> vis(n + 1);
    int now = 0;
    for (int i = 1, j; i < n; i++) {
        for (j = 0; j < (int)g[i].size(); j++) vis[g[i][j]] = i;
        for (j = n; j > 0 && vis[j] == i; j--)
            ;
        if (j > now) add(i, now = j);
    }
    Point tmp, last = p[1];
    double ans = 0;
    for (int i = 0; i < (int)q.size() - 1; i++) {
        tmp = q[i].intersect(q[i + 1]);
        ans += (tmp - last).norm();
        last = tmp;
    }
    ans += (p[n] - last).norm();
    printf("%.9f", ans);
    return 0;
}