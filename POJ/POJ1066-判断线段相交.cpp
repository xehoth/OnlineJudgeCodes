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
 * 「POJ 1066」Treasure Hunt 23-02-2018
 * 计算几何-判断线段相交
 * @author xehoth
 */
#include <cstdio>
#include <iostream>
#include <vector>

const double EPS = 1e-9;

inline int sign(const double x) { return (x > EPS) - (x < -EPS); }

struct Point {
    double x, y;

    Point() {}

    Point(const double x, const double y) : x(x), y(y) {}

    inline double operator*(const Point &p) const { return x * p.y - y * p.x; }

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }
};

struct Segment {
    Point s, t;

    Segment() {}

    Segment(const Point &s, const Point &t) : s(s), t(t) {}

    inline bool isIntersect(const Segment &l) const {
        return sign(std::max(s.x, t.x) - std::min(l.s.x, l.t.x)) >= 0 &&
               sign(std::max(s.y, t.y) - std::min(l.s.y, l.t.y)) >= 0 &&
               sign(std::max(l.s.x, l.t.x) - std::min(s.x, t.x)) >= 0 &&
               sign(std::max(l.s.y, l.t.y) - std::min(s.y, t.y)) >= 0 &&
               sign(((l.t - l.s) * (s - l.s)) * ((l.t - l.s) * (t - l.s))) <
                   0 &&
               sign(((t - s) * (l.s - s)) * ((t - s) * (l.t - s))) < 0;
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
    std::vector<Point> p;
    std::vector<Segment> l;
    Segment tmp;
    Point des;
    while (std::cin >> n) {
        p.clear();
        l.resize(n);
        for (int i = 0; i < n; i++) {
            std::cin >> des;
            p.push_back(l[i].s = des);
            std::cin >> des;
            p.push_back(l[i].t = des);
        }
        p.push_back(Point(0, 0));
        p.push_back(Point(0, 100));
        p.push_back(Point(100, 0));
        p.push_back(Point(100, 100));
        std::cin >> des;
        int ans = 1000, cnt;
        for (int i = 0; i < (int)p.size(); i++) {
            tmp.s = des;
            tmp.t = p[i];
            cnt = 0;
            for (int j = 0; j < (int)l.size(); j++)
                if (tmp.isIntersect(l[j])) cnt++;
            ans = std::min(ans, cnt);
        }
        std::cout << "Number of doors = " << ans + 1 << '\n';
    }
    return 0;
}