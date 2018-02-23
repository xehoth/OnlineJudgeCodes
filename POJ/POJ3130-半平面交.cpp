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
 * 「POJ 3130」How I Mathematician Wonder What You Are! 23-02-2018
 * 计算几何-半平面交
 * @author xehoth
 */
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <deque>
#include <iostream>
#include <vector>

const double EPS = 1e-8;

inline int sign(const double x) { return (x > EPS) - (x < -EPS); }

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

    inline Point operator*(double p) const { return Point(x * p, y * p); }
};

struct Line {
    Point s, t;
    double arg;

    Line() {}

    Line(const Point &s, const Point &t)
        : s(s), t(t), arg(atan2(t.y - s.y, t.x - s.x)) {}

    inline friend bool onLeft(const Point &p, const Line &l) {
        return sign((l.t - l.s) * (p - l.s)) >= 0;
    }

    inline bool operator<(const Line &l) const {
        return sign(arg - l.arg) == 0 ? onLeft(s, l) : arg < l.arg;
    }

    inline Point intersect(const Line &l) const {
        return s +
               (t - s) * (((l.s - s) * (l.t - s)) / ((t - s) * (l.t - l.s)));
    }
};

typedef std::pair<Line, Point> Pair;

inline const std::deque<Pair> &halfPlaneIntersection(std::vector<Line> &l) {
    std::sort(l.begin(), l.end());
    static std::deque<Pair> q;
    q.clear();
    q.push_back(Pair(l[0], Point(0, 0)));
    for (int i = 1; i < (int)l.size(); i++) {
        if (std::abs(l[i].arg - l[i - 1].arg) < EPS) continue;
        while (q.size() > 1 && !onLeft(q.back().second, l[i])) q.pop_back();
        while (q.size() > 1 && !onLeft(q[1].second, l[i])) q.pop_front();
        q.push_back(Pair(l[i], l[i].intersect(q.back().first)));
    }
    while (q.size() > 1 && !onLeft(q.back().second, q.front().first))
        q.pop_back();
    q.front().second = q.front().first.intersect(q.back().first);
    return q;
}

inline std::istream &operator>>(std::istream &is, Point &p) {
    is >> p.x >> p.y;
    return is;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    Point a, b, o;
    std::vector<Line> l;
    for (int n; std::cin >> n && n;) {
        std::cin >> a;
        o = a;
        l.resize(n);
        for (int i = 1; i < n; i++) {
            std::cin >> b;
            l[i - 1] = Line(a, b);
            a = b;
        }
        l[n - 1] = Line(a, o);
        const std::deque<Pair> &q = halfPlaneIntersection(l);
        std::cout << (q.size() <= 2 ? "0\n" : "1\n");
    }
    return 0;
}