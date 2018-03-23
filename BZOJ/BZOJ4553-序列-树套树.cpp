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
 * 「BZOJ 4553」序列 22-03-2018
 * 树套树
 * @author xehoth
 */

#include <cctype>
#include <cstdio>
#include <iostream>

char pool[1024 * 1024 * 65];

inline void *operator new(size_t size) {
    static char *s = pool;
    char *t = s;
    s += size;
    return t;
}

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 * 2 };
    char ibuf[SIZE], *s;

    InputOutputStream() : s(ibuf) { fread(ibuf, 1, SIZE, stdin); }

    inline InputOutputStream &operator>>(int &x) {
        while (!isdigit(*s)) s++;
        for (x = 0; isdigit(*s); s++) x = x * 10 + (*s ^ '0');
        return *this;
    }
} io;

const int MAXN = 100000 + 1;

int n, m, l, a[MAXN], min[MAXN], max[MAXN];

struct Node *null;

struct Node {
    Node *lc, *rc;
    int max;

    Node() : lc(null), rc(null) {}
};

Node *d[MAXN];

void insert(Node *&p, int l, int r, int x, int v) {
    if (p == null) p = new Node;
    p->max = std::max(p->max, v);
    if (l == r) return;
    int mid = (l + r) >> 1;
    x <= mid ? insert(p->lc, l, mid, x, v) : insert(p->rc, mid + 1, r, x, v);
}

int query(Node *p, int l, int r, int x) {
    if (p == null) return 0;
    if (l == r) return p->max;
    int mid = (l + r) >> 1;
    return x <= mid ? query(p->lc, l, mid, x)
                    : std::max(p->lc->max, query(p->rc, mid + 1, r, x));
}

inline void modify(int k, int x, int v) {
    for (; k <= n; k += k & -k) insert(d[k], 1, l, x, v);
}

inline int query(int k, int v) {
    int ret = 0;
    for (; k; k ^= k & -k) ret = std::max(ret, query(d[k], 1, l, v));
    return ret;
}

int main() {
    null = new Node;
    null->lc = null->rc = null;
    io >> n >> m;
    for (int i = 1; i <= n; i++) {
        io >> a[i];
        min[i] = max[i] = a[i];
        l = std::max(l, a[i]);
    }
    for (int i = 1, x, y; i <= n; i++) {
        io >> x >> y;
        min[x] = std::min(min[x], y);
        max[x] = std::max(max[x], y);
        l = std::max(l, y);
    }
    for (int i = 0; i <= n; i++) d[i] = null;
    int ans = 0;
    for (int i = 1, t; i <= n; i++) {
        t = query(min[i], a[i]);
        modify(a[i], max[i], t + 1);
        ans = std::max(ans, t + 1);
    }
    std::cout << ans;
    return 0;
}