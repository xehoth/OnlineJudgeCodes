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
 * 「BZOJ 4561」圆的异或并 12-03-2018
 * 扫描线
 * @author xehoth
 */
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <set>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 * 30 };

    char ibuf[SIZE], *s;

    InputOutputStream() : s(ibuf) { fread(ibuf, 1, SIZE, stdin); }

    inline InputOutputStream &operator>>(int &x) {
        static char c;
        static bool iosig;
        for (c = *s++, iosig = false; !isdigit(c); c = *s++) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = *s++) x = x * 10 + (c ^ '0');
        iosig && (x = -x);
        return *this;
    }
} io;

const int MAXN = 200000 + 1;

struct Circle {
    int x, y, r;
} c[MAXN];

struct Point {
    int x, id, f;

    inline void init(int x, int id, int f) {
        this->x = x;
        this->id = id;
        this->f = f;
    }

    inline bool operator<(const Point &p) const { return x < p.x; }
} d[MAXN * 2 + 1];

inline long long square(int x) { return (long long)x * x; }

int nowX;

struct Node {
    int id, f;

    Node(int id, int f) : id(id), f(f) {}

    inline bool operator<(const Node &p) const {
        double y1 =
            c[id].y + f * sqrt(square(c[id].r) - square(c[id].x - nowX));
        double y2 = c[p.id].y +
                    p.f * sqrt(square(c[p.id].r) - square(c[p.id].x - nowX));
        return y1 != y2 ? y1 < y2 : f < p.f;
    }
};

std::set<Node> set;
int f[MAXN], n, tot;

int main() {
    io >> n;
    for (int i = 1; i <= n; i++) io >> c[i].x >> c[i].y >> c[i].r;
    for (int i = 1; i <= n; i++) {
        d[++tot].init(c[i].x - c[i].r, i, 1);
        d[++tot].init(c[i].x + c[i].r, i, -1);
    }
    std::sort(d + 1, d + tot + 1);
    for (int i = 1; i <= tot; i++) {
        nowX = d[i].x;
        if (d[i].f == 1) {
            std::set<Node>::iterator it = set.upper_bound(Node(d[i].id, -1));
            if (it == set.end()) {
                f[d[i].id] = 1;
            } else {
                f[d[i].id] = (it->f == 1 ? -f[it->id] : f[it->id]);
            }
            set.insert(Node(d[i].id, -1));
            set.insert(Node(d[i].id, 1));
        } else {
            set.erase(Node(d[i].id, -1));
            set.erase(Node(d[i].id, 1));
        }
    }
    long long ret = 0;
    for (int i = 1; i <= n; i++) ret += square(c[i].r) * f[i];
    std::cout << ret;
    return 0;
}
