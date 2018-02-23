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
 * 「POJ 3348」Cows 23-02-2018
 * 计算几何-凸包
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

    inline bool operator<(const Point &p) const {
        return sign(x - p.x) < 0 || (sign(x - p.x) == 0 && sign(y - p.y) < 0);
    }
};

inline void andrew(std::vector<Point> &p, std::vector<Point> &c) {
    std::sort(p.begin(), p.end());
    c.resize(2 * p.size());
    int top = 0, k;
    for (int i = 0; i < (int)p.size(); i++) {
        while (top > 1 &&
               sign((c[top - 1] - c[top - 2]) * (p[i] - c[top - 2])) <= 0)
            top--;
        c[top++] = p[i];
    }
    k = top;
    for (int i = (int)p.size() - 2; i >= 0; i--) {
        while (top > k &&
               sign((c[top - 1] - c[top - 2]) * (p[i] - c[top - 2])) <= 0)
            top--;
        c[top++] = p[i];
    }
    c.resize((int)p.size() > 1 ? top - 1 : top);
}

struct Polygon {
    std::vector<Point> p;
    int n;

    inline double area() {
        double ret = 0;
        for (int i = 0; i < n; i++) ret += p[i] * p[i + 1];
        return ret / 2;
    }

    inline Polygon &fix() {
        p[n] = p[0];
        if (area() < 0) std::reverse(p.begin(), p.begin() + n);
        p[n] = p[0];
        return *this;
    }

    Polygon() {}

    Polygon(const int n) : p(n + 1), n(n) {}

    Polygon(const std::vector<Point> &v) : p(v), n(v.size()) {
        p.resize(n + 1);
    }
};

inline std::istream &operator>>(std::istream &is, Point &p) {
    is >> p.x >> p.y;
    return is;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    int n;
    std::cin >> n;
    std::vector<Point> p(n), c;
    for (int i = 0; i < n; i++) std::cin >> p[i];
    andrew(p, c);
    std::cout << (int)(Polygon(c).fix().area() / 50);
    return 0;
}