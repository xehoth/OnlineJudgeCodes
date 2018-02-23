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
 * 「POJ 2194」Stacking Cylinders 23-02-2018
 * 计算几何-向量旋转缩放
 * @author xehoth
 */
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <vector>

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

    inline Point rotate(double arg) {
        double c = cos(arg), s = sin(arg);
        return Point(x * c - y * s, x * s + y * c);
    }

    inline double dis(const Point &p) const {
        return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
    }

    inline double norm() const { return sqrt(x * x + y * y); }

    inline bool operator<(const Point &p) const { return x < p.x; }
};

inline Point getMidPoint(const Point &a, const Point &b) {
    return (a + b) * 0.5;
}

inline std::ostream &operator<<(std::ostream &os, const Point &p) {
    os << std::fixed << std::setprecision(4) << p.x << ' ' << p.y << '\n';
    return os;
}

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    std::vector<Point> p;
    for (int n; std::cin >> n && n;) {
        p.resize(n);
        for (int i = 0; i < n; i++) {
            std::cin >> p[i].x;
            p[i].y = 1;
        }
        std::sort(p.begin(), p.end());
        while (p.size() != 1) {
            for (int i = 0; i < (int)p.size() - 1; i++) {
                Point mid = getMidPoint(p[i], p[i + 1]);
                double dis = mid.dis(p[i]);
                double arg = acos(dis / 2.0);
                mid = mid - p[i];
                mid = mid * (2.0 / mid.norm());
                p[i] = p[i] + mid.rotate(arg);
            }
            p.pop_back();
        }
        std::cout << p.back();
    }
    return 0;
}