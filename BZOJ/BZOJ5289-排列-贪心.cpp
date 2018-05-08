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
 * 「BZOJ 5289」排列 08-05-2018
 * 贪心
 * @author xehoth
 */
#include <bits/stdc++.h>

const int POOL_SIZE = 1024 * 1024 * 30;

char pool[POOL_SIZE];

void *operator new(size_t size) {
    static char *s = pool;
    char *t = s;
    s += size;
    return t;
}

void operator delete(void *) {}

void operator delete(void *, size_t) {}

const int MAXN = 500000 + 9;

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };
    char ibuf[SIZE], *s, *t;

    inline char read() {
        if (s == t) t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin);
        return s == t ? -1 : *s++;
    }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        if (iosig) x = -x;
        return *this;
    }
} io;

int a[MAXN], fa[MAXN], size[MAXN];
long long sum[MAXN];

inline int get(int x) { return x == fa[x] ? x : fa[x] = get(fa[x]); }

struct Node {
    long long v;
    int s, x;

    inline bool operator<(const Node &p) const { return v * p.s > p.v * s; }

    Node(long long v, int s, int x) : v(v), s(s), x(x) {}
};

struct PriorityQueue : public std::priority_queue<Node> {
    inline void reserve(int n) { std::priority_queue<Node>::c.reserve(n); }

    inline void push_back(const Node &p) {
        std::priority_queue<Node>::c.push_back(p);
    }

    inline void init() {
        std::make_heap(std::priority_queue<Node>::c.begin(),
                       std::priority_queue<Node>::c.end());
    }
} q;

int main() {
    // freopen("sample/1.in", "r", stdin);
    int n;
    io >> n;
    for (register int i = 1; i <= n; i++) io >> a[i];
    for (register int i = 1; i <= n; i++) io >> sum[i];

    for (int i = 1; i <= n; i++) fa[i] = i;
    for (int i = 1, u, v; i <= n; i++) {
        u = get(i);
        v = get(a[i]);
        if (u == v) {
            std::cout << "-1";
            return 0;
        }
        fa[u] = v;
    }
    long long ans = 0;
    for (int i = 1; i <= n; i++) {
        size[i] = 1;
        fa[i] = i;
    }
    size[0] = 1;
    q.reserve(2 * n);
    for (int i = 1; i <= n; i++) q.push_back(Node(sum[i], size[i], i));
    q.init();
    while (!q.empty()) {
        Node tmp = q.top();
        q.pop();
        if (tmp.s != size[tmp.x]) continue;
        int f = get(a[tmp.x]);
        ans += tmp.v * size[f];
        fa[tmp.x] = f;
        size[f] += tmp.s;
        sum[f] += tmp.v;
        if (f) q.push(Node(sum[f], size[f], f));
    }
    std::cout << ans;
}
