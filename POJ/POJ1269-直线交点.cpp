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
 * 「POJ 1269」Intersecting Lines 23-02-2018
 * 计算几何-直线交点
 * @author xehoth
 */
#include <algorithm>
#include <cstdio>
#include <iomanip>
#include <iostream>

const double EPS = 1e-8;

inline int sign(const double x) { return (x > EPS) - (x < -EPS); }

struct Point {
    double x, y;

    Point() {}

    Point(const double x, const double y) : x(x), y(y) {}

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline double operator*(const Point &p) const { return x * p.y - y * p.x; }

    inline Point operator*(double p) const { return Point(x * p, y * p); }

    inline Point operator+(const Point &p) const {
        return Point(x + p.x, y + p.y);
    }
};

inline std::istream &operator>>(std::istream &is, Point &p) {
    is >> p.x >> p.y;
    return is;
}

inline std::ostream &operator<<(std::ostream &os, const Point &p) {
    os << std::fixed << std::setprecision(2) << p.x << ' ' << p.y << '\n';
    return os;
}

struct Line {
    Point s, t;

    Line() {}

    Line(const Point &s, const Point &t) : s(s), t(t) {}

    inline bool isParallel(const Line &l) const {
        return sign((t - s) * (l.t - l.s)) == 0;
    }

    inline bool isCollinear(const Line &l) const {
        return isParallel(l) && sign((t - s) * (l.t - s)) == 0 &&
               sign((l.t - l.s) * (t - l.s)) == 0;
    }

    inline Point intersect(const Line &l) const {
        return s +
               (t - s) * (((l.s - s) * (l.t - s)) / ((t - s) * (l.t - l.s)));
    }
};

inline std::istream &operator>>(std::istream &is, Line &l) {
    is >> l.s >> l.t;
    return is;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    int n;
    std::cin >> n;
    std::cout << "INTERSECTING LINES OUTPUT\n";
    Line a, b;
    for (int i = 0; i < n; i++) {
        std::cin >> a >> b;
        if (a.isParallel(b)) {
            std::cout << (a.isCollinear(b) ? "LINE\n" : "NONE\n");
        } else {
            std::cout << "POINT " << a.intersect(b);
        }
    }
    std::cout << "END OF OUTPUT\n";
    return 0;
}