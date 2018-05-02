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
 * 「CC GEOCHEAT」Bear and Shuffled Points 02-05-2018
 * 凸包
 * @author xehoth
 */
#include <bits/stdc++.h>

struct Point {
    int x, y;

    long long operator*(const Point &p) const {
        return (long long)x * p.y - (long long)y * p.x;
    }

    Point operator-(const Point &p) const { return {x - p.x, y - p.y}; }

    long long dis2(const Point &p) const {
        return (long long)(x - p.x) * (x - p.x) +
               (long long)(y - p.y) * (y - p.y);
    }

    bool operator<(const Point &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
};

const int MAXN = 750000;

int x[MAXN + 1], y[MAXN + 1];

std::tuple<Point, Point, long long> solve(int n) {
    static std::vector<Point> p, c;
    p.clear();
    p.resize(n);
    for (int i = 0; i < n; i++) {
        p[i].x = x[i];
        p[i].y = y[i];
    }
    if (n == 2) {
        return std::make_tuple(p[0], p[1], p[0].dis2(p[1]));
    }

    std::sort(p.begin(), p.end());
    c.clear();
    c.resize(n * 2);
    int top = 0, k;
    for (int i = 0; i < n; i++) {
        while (top > 1 && (c[top - 1] - c[top - 2]) * (p[i] - c[top - 2]) < 0)
            top--;
        c[top++] = p[i];
    }
    k = top;
    for (int i = n - 2; i >= 0; i--) {
        while (top > k && (c[top - 1] - c[top - 2]) * (p[i] - c[top - 2]) < 0)
            top--;
        c[top++] = p[i];
    }
    top--;
    c.resize(top + 1);
    c[top] = c[0];

    Point ret1, ret2;
    long long ret = 0;
    for (int i = 0, j = 0; i < top; i++) {
        while (c[i].dis2(c[j + 1]) >= c[i].dis2(c[j])) {
            if (++j == top) j = 0;
        }
        long long d = c[i].dis2(c[j]);
        if (d > ret) {
            ret = d;
            ret1 = c[i];
            ret2 = c[j];
        }
    }
    return std::make_tuple(ret1, ret2, ret);
}

long long ans[MAXN];

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n;
    std::cin >> n;
    for (int i = 0; i < n; i++) std::cin >> x[i] >> y[i];
    for (int k = n; k > 1;) {
        Point p, q;
        long long d;
        std::tie(p, q, d) = solve(k);
        ans[--k] = d;
        while (k > 1 && (x[k] != p.x || y[k] != p.y) &&
               (x[k] != q.x || y[k] != q.y))
            ans[--k] = d;
    }
    for (int i = 0; i < n; i++) std::cout << ans[i] << ' ';
    return 0;
}