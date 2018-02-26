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
 * 「POJ 3608」Bridge Across Islands 26-02-2018
 * 凸包间最短距离
 * @author xehoth
 */
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <vector>

const double EPS = 1e-8;
const double INF = 1e16;

inline int sign(const double x) { return (x > EPS) - (x < -EPS); }

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

    inline Point operator*(double p) const { return Point(x * p, y * p); }

    inline double operator*(const Point &p) const { return x * p.y - y * p.x; }

    inline double operator^(const Point &p) const { return x * p.x + y * p.y; }

    inline double dis(const Point &p) const {
        return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
    }

    inline bool operator==(const Point &p) const {
        return sign(x - p.x) == 0 && sign(y - p.y) == 0;
    }
};

struct Segment {
    Point s, t;

    Segment(const Point &s, const Point &t) : s(s), t(t) {}

    inline double dis(const Point &p) const {
        if (s == t) return s.dis(p);
        // 投影到 t-s 延长线上
        if (((t - s) ^ (p - s)) < -EPS) return s.dis(p);
        if (((s - t) ^ (p - t)) < -EPS) return s.dis(p);
        return std::abs(((t - s) * (p - s)) / s.dis(t));
    }

    inline double dis(const Segment &l) const {
        return std::min(std::min(dis(l.s), dis(l.t)),
                        std::min(l.dis(s), l.dis(t)));
    }
};

inline void fix(std::vector<Point> &p) {
    int n = p.size() - 1;
    p[n] = p[0];
    double ret = 0;
    for (int i = 0; i < n; i++) ret += p[i] * p[i + 1];
    if (sign(ret) < 0) std::reverse(p.begin(), p.begin() + n);
    p[n] = p[0];
}

inline double cross(const Point &a, const Point &b, const Point &c) {
    return (b - a) * (c - a);
}

int main() {
    int n, m;
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    std::vector<Point> p, q;
    while (std::cin >> n >> m, n && m) {
        p.resize(n + 1);
        q.resize(m + 1);
        for (int i = 0; i < n; i++) std::cin >> p[i].x >> p[i].y;
        for (int i = 0; i < m; i++) std::cin >> q[i].x >> q[i].y;
        fix(p);
        fix(q);
        int minP = 0, maxQ = 0;
        for (int i = 1; i < n; i++)
            if (sign(p[minP].y - p[i].y) > 0) minP = i;
        for (int i = 1; i < m; i++)
            if (sign(q[maxQ].y - q[i].y) < 0) maxQ = i;
        double ans = INF;
        for (int i = 0; i < n; i++) {
            while (cross(p[minP + 1], q[maxQ + 1], p[minP]) -
                       cross(p[minP + 1], q[maxQ], p[minP]) >
                   EPS) {
                if (++maxQ == m) maxQ = 0;
            }
            ans = std::min(ans, Segment(p[minP], p[minP + 1])
                                    .dis(Segment(q[maxQ], q[maxQ + 1])));
            if (++minP == n) minP = 0;
        }
        std::cout << std::fixed << std::setprecision(5) << ans << '\n';
    }
    return 0;
}