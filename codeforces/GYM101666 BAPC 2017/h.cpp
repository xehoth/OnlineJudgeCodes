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
 * 「CF GYM 101666 H」25-02-2018
 * 欧拉定理 + 并查集
 * @author xehoth
 */
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

inline int sign(long long x) { return (x > 0) - (x < 0); }

struct Point {
    int x, y;

    Point() = default;

    Point(const int x, const int y) : x(x), y(y) {}

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline int operator*(const Point &p) const {
        return sign((long long)x * p.y - (long long)y * p.x);
    }
};

struct Line {
    Point s, t;

    Line() = default;

    Line(const Point &s, const Point &t) : s(s), t(t) {}
    inline bool isIntersect(const Line &l) const {
        return std::max(s.x, t.x) >= std::min(l.s.x, l.t.x) &&
               std::max(s.y, t.y) >= std::min(l.s.y, l.t.y) &&
               std::max(l.s.x, l.t.x) >= std::min(s.x, t.x) &&
               std::max(l.s.y, l.t.y) >= std::min(s.y, t.y) &&
               ((l.t - l.s) * (s - l.s)) * ((l.t - l.s) * (t - l.s)) <= 0 &&
               ((t - s) * (l.s - s)) * ((t - s) * (l.t - s)) <= 0;
    }
};

inline std::istream &operator>>(std::istream &is, Point &p) {
    is >> p.x >> p.y;
    return is;
}

inline std::istream &operator>>(std::istream &is, Line &p) {
    is >> p.s >> p.t;
    return is;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<Line> l(std::istream_iterator<Line>(std::cin),
                        std::istream_iterator<Line>());
    std::vector<int> fa(n + 1);
    std::iota(fa.begin(), fa.end(), 0);
    auto get = [&](int x) {
        while (x != fa[x]) x = fa[x] = fa[fa[x]];
        return x;
    };
    int c = n;
    auto put = [&](int u, int v) {
        if ((u = get(u)) != (v = get(v))) {
            c--;
            fa[u] = v;
        }
    };
    int e = n, p = 2 * n;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (l[i].isIntersect(l[j])) {
                e += 2;
                p++;
                put(i, j);
            }
        }
    }
    std::cout << e - p + c;
}
