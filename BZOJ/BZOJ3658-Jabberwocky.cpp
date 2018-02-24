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
 * 「BZOJ 3658」Jabberwocky 24-02-2018
 * 链表
 * @author xehoth
 */
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

struct InputStream {
    enum { SIZE = 1 << 18 | 1 };

    char ibuf[SIZE], *s, *t;

    InputStream() : s(), t() {}

    inline char read() {
        return (s == t) && (t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin)),
               s == t ? -1 : *s++;
    }

    template <typename T>
    inline InputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        iosig && (x = -x);
        return *this;
    }
} io;

const int MAXN = 100000 + 3;

struct Point {
    int x, y, color, id;
} pool[MAXN];

Point *p[MAXN];

int n, k, buc[MAXN], w[MAXN], ans, pre[MAXN], l[MAXN], r[MAXN];

int d[MAXN];

inline void add(int k, int v) {
    for (; k <= n + 1; k += k & -k) d[k] += v;
}

inline int query(int k) {
    int ret = 0;
    for (; k; k ^= k & -k) ret += d[k];
    return ret;
}

inline void update(int l, int r) {
    if (l <= r) ans = std::max(ans, query(r) - query(l - 1));
}

inline bool cmpX(const Point *a, const Point *b) { return a->x < b->x; }

inline bool cmpY(const Point *a, const Point *b) { return a->y < b->y; }

inline void solveSide() {
    memset(d, 0, sizeof(int) * (n + 2));
    memset(pre, 0, sizeof(int) * (n + 1));
    w[0] = 0;
    w[n + 1] = n + 1;
    std::sort(p + 1, p + n + 1, cmpX);
    for (int i = 1; i <= n; i++) add(p[i]->x, 1);
    for (int i = 1, x, y; i <= n; i++) {
        x = p[i]->id;
        y = pre[p[i]->color];
        l[x] = y;
        r[x] = n + 1;
        if (y) r[y] = x;
        update(w[y] + 1, w[x] - 1);
        pre[p[i]->color] = x;
    }
    for (int i = 1; i <= k; i++) update(w[pre[i]] + 1, n + 1);
    std::sort(p + 1, p + n + 1, cmpY);
    for (int i = 1, j = 1, x; i <= n; i++) {
        x = p[i]->id;
        while (j <= n && p[j]->y == p[i]->y) add(p[j++]->x, -1);
        l[r[x]] = l[x];
        r[l[x]] = r[x];
        update(w[l[x]] + 1, w[r[x]] - 1);
    }
}

inline void solveCase() {
    ans = 0;
    io >> n >> k;
    p[0] = pool;
    for (int i = 1; i <= n; i++) {
        io >> pool[i].x >> pool[i].y >> pool[i].color;
        pool[i].id = i;
        p[i] = pool + i;
        buc[i] = pool[i].x;
    }
    std::sort(buc + 1, buc + n + 1);
    int *end = std::unique(buc + 1, buc + n + 1);
    for (int i = 1; i <= n; i++)
        w[i] = pool[i].x = std::lower_bound(buc + 1, end, pool[i].x) - buc;
    solveSide();
    for (int i = 1; i <= n; i++) pool[i].y = -pool[i].y;
    solveSide();
    printf("%d\n", ans);
}

int main() {
    int T;
    io >> T;
    while (T--) solveCase();
    return 0;
}
