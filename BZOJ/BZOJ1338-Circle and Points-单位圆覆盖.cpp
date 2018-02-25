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
 * 「BZOJ 1338」Circle and Points 25-02-2018
 * 单位圆覆盖
 * @author xehoth
 */
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>

const int MAXN = 300 + 1;

struct Point {
    double x, y;

    Point() {}

    Point(double x, double y) : x(x), y(y) {}

    inline double dis(const Point &p) const {
        return sqrt((x - p.x) * (x - p.x) + (y - p.y) * (y - p.y));
    }
} pts[MAXN];

struct PolarAngle {
    double arg;
    int f;

    inline bool operator<(const PolarAngle &other) const {
        return arg < other.arg;
    }
} pa[MAXN];

inline int solve(const int n) {
    int result = 1;
    for (int i = 0; i < n; i++) {
        int m = 0;
        double d;
        for (int j = 0; j < n; j++) {
            if (i != j && (d = pts[i].dis(pts[j])) <= 2) {
                double phi = acos(d / 2);
                double theta = atan2(pts[j].y - pts[i].y, pts[j].x - pts[i].x);
                pa[m].arg = theta - phi;
                pa[m++].f = 1;
                pa[m].arg = theta + phi;
                pa[m++].f = -1;
            }
        }
        std::sort(pa, pa + m);
        for (int sum = 1, j = 0; j < m; j++)
            result = std::max(result, sum += pa[j].f);
    }
    return result;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    int n;
    while (std::cin >> n, n) {
        for (int i = 0; i < n; i++) std::cin >> pts[i].x >> pts[i].y;
        std::cout << solve(n) << '\n';
    }
    return 0;
}