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
 * 「HDU 1756」Cupid's Arrow 23-02-2018
 * 计算几何-判断点在多边形内
 * @author xehoth
 */
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>

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
};

inline bool isPointOnSegment(const Point &a, const Point &b, const Point &p) {
    return sign((a - p) * (b - p)) == 0 &&
           sign(std::min(a.x, b.x) - p.x) <= 0 &&
           sign(std::min(a.y, b.y) - p.y) <= 0 &&
           sign(std::max(a.x, b.x) - p.x) >= 0 &&
           sign(std::max(a.y, b.y) - p.y) >= 0;
}

struct Polygon {
    std::vector<Point> p;
    int n;

    Polygon() : p(), n() {}
    Polygon(const int n) : p(n + 1), n(n) {}

    inline double area() {
        double ret = 0;
        for (int i = 0; i < n; i++) ret += p[i] * p[i + 1];
        return ret / 2;
    }

    inline void fix() {
        p[n] = p[0];
        if (area() < 0) std::reverse(p.begin(), p.begin() + n);
        p[n] = p[0];
    }

    inline bool contains(const Point &q) {
        int cnt = 0;
        for (int i = 0; i < n; i++) {
            if (isPointOnSegment(p[i], p[i + 1], q)) return true;
            double det = (p[i] - q) * (p[i + 1] - q);
            double d1 = p[i].y - q.y, d2 = p[i + 1].y - q.y;
            if ((sign(det) >= 0 && sign(d1) < 0 && sign(d2) >= 0) ||
                (sign(det) <= 0 && sign(d1) >= 0 && sign(d2) < 0))
                cnt++;
        }
        return cnt & 1;
    }

    inline Point &operator[](int i) { return p[i]; }

    inline const Point &operator[](int i) const { return p[i]; }
};

inline std::istream &operator>>(std::istream &is, Point &p) {
    is >> p.x >> p.y;
    return is;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    int n, m;
    Polygon p;
    while (std::cin >> n) {
        p.p.resize(n + 1);
        p.n = n;
        for (int i = 0; i < n; i++) std::cin >> p[i];
        p.fix();
        std::cin >> m;
        for (Point q; m--;) {
            std::cin >> q;
            std::cout << (p.contains(q) ? "Yes\n" : "No\n");
        }
    }
    return 0;
}