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
 * 「BZOJ 2402」陶陶的难题II 24-02-2018
 * 二分 + 树链剖分 + 线段树 + 凸包
 * @author xehoth
 */
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>

inline char read() {
    static const int IN_LEN = 1 << 18 | 1;
    static char buf[IN_LEN], *s, *t;
    return (s == t) && (t = (s = buf) + fread(buf, 1, IN_LEN, stdin)),
           s == t ? -1 : *s++;
}

template <typename T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        iosig |= c == '-';
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig && (x = -x);
}

inline void read(double &t) {
    static char c;
    static bool iosig;
    int x = 0;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    if (c == '.') {
        long long y = 0, cnt = 1;
        for (c = read(); isdigit(c); c = read())
            y = y * 10 + (c ^ '0'), cnt *= 10;
        t = x + (double)y / cnt;
    } else {
        t = x;
    }
    iosig ? t = -t : 0;
}

const int OUT_LEN = 1 << 18 | 1;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    (oh == obuf + OUT_LEN) && (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf);
    *oh++ = c;
}

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

const double EPS = 1e-9;

inline int sign(const double x) { return (x > EPS) - (x < -EPS); }

inline void print(double x) {
    static char tmpBuf[30];
    sprintf(tmpBuf, "%.4f", x);
    print((const char *)tmpBuf);
}

struct InputOutputStream {
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

struct Point {
    double x, y;

    Point() {}

    Point(const double x, const double y) : x(x), y(y) {}

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline double operator*(const Point &p) const { return x * p.y - y * p.x; }
};

struct Node *null;

char *cur;

struct Node {
    static const int NODE_SIZE;

    Node *lc, *rc;
    std::vector<const Point *> c;

    Node() : lc(null), rc(null), c() {}

    inline void insert(const Point *p) {
        while (c.size() > 1 &&
               (*c[(int)c.size() - 1] - *p) * (*c[(int)c.size() - 2] - *p) <
                   -EPS)
            c.pop_back();
        c.push_back(p);
    }

    inline void maintain() {
        size_t sl = lc->c.size(), sr = rc->c.size(), pl = 0, pr = 0;
        while (pl < sl || pr < sr) {
            if (pr == sr || (pl < sl && lc->c[pl]->x < rc->c[pr]->x))
                insert(lc->c[pl++]);
            else
                insert(rc->c[pr++]);
        }
    }

    inline void *operator new(size_t) { return cur += NODE_SIZE; }
};

const int MAXN = 30000 + 9;
const int MAXM = MAXN * 4 * 2 + 1;
const double INF = 1e16;

const int Node::NODE_SIZE = sizeof(Node);

char pool[Node::NODE_SIZE * MAXM];

int n;

class SegmentTree {
   public:
    const Point *pts[MAXN];
    Node *root;

    inline void init() { root = null; }

    inline void build(int n) { this->build(root, 1, n); }

    inline double query(int l, int r, double k) {
        return this->query(root, 1, n, l, r, k);
    }

   private:
    void build(Node *&p, int l, int r) {
        p = new Node();
        if (l == r) {
            p->c.push_back(pts[l]);
            return;
        }
        int mid = (l + r) >> 1;
        build(p->lc, l, mid);
        build(p->rc, mid + 1, r);
        p->maintain();
    }

    inline double binary(const std::vector<const Point *> &c, const double k) {
        int l = 0, r = c.size();
        for (int mid; r - l > 1;) {
            mid = (l + r) >> 1;
            if (c[mid]->y - k * c[mid]->x - EPS >
                c[mid - 1]->y - k * c[mid - 1]->x)
                l = mid;
            else
                r = mid;
        }
        return c[l]->y - k * c[l]->x;
    }

    double query(Node *p, int l, int r, int s, int t, double k) {
        if (s <= l && t >= r) return binary(p->c, k);
        int mid = (l + r) >> 1;
        double ans = -INF;
        if (s <= mid) ans = std::max(ans, query(p->lc, l, mid, s, t, k));
        if (t > mid) ans = std::max(ans, query(p->rc, mid + 1, r, s, t, k));
        return ans;
    }
} segA, segB;

Point vxy[MAXN], vpq[MAXN];

std::vector<int> g[MAXN];

int fa[MAXN], sz[MAXN], dep[MAXN], son[MAXN], top[MAXN], pos[MAXN], idx;
bool vis[MAXN];

void dfs1(const int u) {
    vis[u] = true;
    sz[u] = 1;
    dep[u] = dep[fa[u]] + 1;
    for (int i = 0, v; i < (int)g[u].size(); i++) {
        if (!vis[v = g[u][i]]) {
            fa[v] = u;
            dfs1(v);
            sz[u] += sz[v];
            (sz[v] > sz[son[u]]) && (son[u] = v);
        }
    }
}

void dfs2(const int u) {
    vis[u] = false;
    pos[u] = ++idx;
    top[u] = (u == son[fa[u]] ? top[fa[u]] : u);
    segA.pts[idx] = vxy + u;
    segB.pts[idx] = vpq + u;
    if (son[u]) dfs2(son[u]);
    for (int i = 0, v; i < (int)g[u].size(); i++)
        if (vis[v = g[u][i]]) dfs2(v);
}

inline double solve(int u, int v, double mid) {
    double ans1 = -INF, ans2 = -INF;
    while (top[u] != top[v]) {
        if (dep[top[u]] < dep[top[v]]) std::swap(u, v);
        ans1 = std::max(ans1, segA.query(pos[top[u]], pos[u], mid));
        ans2 = std::max(ans2, segB.query(pos[top[u]], pos[u], mid));
        u = fa[top[u]];
    }
    if (dep[u] > dep[v]) std::swap(u, v);
    ans1 = std::max(ans1, segA.query(pos[u], pos[v], mid));
    ans2 = std::max(ans2, segB.query(pos[u], pos[v], mid));
    return ans1 + ans2;
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    cur = pool;
    null = (Node *)pool;
    null->lc = null;
    null->rc = null;
    null->c = std::vector<const Point *>();
    io >> n;
    segA.init();
    segB.init();
    for (int i = 1; i <= n; i++) io >> vxy[i].x;
    for (int i = 1; i <= n; i++) io >> vxy[i].y;
    for (int i = 1; i <= n; i++) io >> vpq[i].x;
    for (int i = 1; i <= n; i++) io >> vpq[i].y;
    for (int i = 1, u, v; i < n; i++) {
        io >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs1(1);
    dfs2(1);
    segA.build(n);
    segB.build(n);
    int m;
    io >> m;
    for (double l, r, mid; m--;) {
        l = 0, r = 1e8;
        int x, y;
        io >> x >> y;
        while (r - l > 1e-5) {
            mid = (l + r) / 2;
            if (solve(x, y, mid) > EPS)
                l = mid;
            else
                r = mid;
        }
        io << l << '\n';
    }
    return 0;
}