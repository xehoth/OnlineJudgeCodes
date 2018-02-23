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
 * 「POJ 2187」Beauty Contest 23-02-2018
 * 计算几何-旋转卡壳
 * @author xehoth
 */
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <iterator>
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

    inline double dis2(const Point &p) const {
        return (x - p.x) * (x - p.x) + (y - p.y) * (y - p.y);
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

inline double rotatingCalispers(std::vector<Point> &p) {
    int n = p.size(), top = 1;
    p.push_back(p[0]);
    double ans = 0;
    for (int i = 0; i < n; i++) {
        while (sign((p[i + 1] - p[i]) * (p[top] - p[i]) -
                    (p[i + 1] - p[i]) * (p[top + 1] - p[i])) < 0)
            if (++top == n) top = 0;
        ans = std::max(ans,
                       std::max(p[i].dis2(p[top]), p[i + 1].dis2(p[top + 1])));
    }
    p.pop_back();
    return ans;
}

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
    std::cout << (int)(rotatingCalispers(c) + 0.5);
    return 0;
}