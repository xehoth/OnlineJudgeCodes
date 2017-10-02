/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4154」Generating Synergy 02-10-2017
 * K-D 树
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 100000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
}

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = 0;
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = 0;
    return s;
}

inline void read(char &x) {
    while (x = read(), isspace(x) && x != -1)
        ;
}

const int OUT_LEN = 100000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, oh - obuf, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    ~InputOutputStream() { flush(); }
} io;
}

namespace {

#define long long long
const int MOD = 1e9 + 7;
const int MAXN = 100010;

struct IdPoint {
    int x, y, id;

    IdPoint(int x = 0, int y = 0, int id = 0) : x(x), y(y), id(id) {}

} p[MAXN + 1];

struct Point {
    int x, y;

    Point(int x = 0, int y = 0) : x(x), y(y) {}

    Point(const IdPoint &p) : x(p.x), y(p.y) {}
};

struct Node {
    Node *c[2], *fa;

    Point p, min, max;
    int cov, color;

    Node();
    Node(const IdPoint &);

    inline void maintain() {
        min.x = std::min(std::min(c[0]->min.x, c[1]->min.x), min.x);
        min.y = std::min(std::min(c[0]->min.y, c[1]->min.y), min.y);
        max.x = std::max(std::max(c[0]->max.x, c[1]->max.x), max.x);
        max.y = std::max(std::max(c[0]->max.y, c[1]->max.y), max.y);
    }

    inline void cover(int);

    inline void pushDown() {
        if (cov) c[0]->cover(cov), c[1]->cover(cov), cov = 0;
    }

    inline void *operator new(size_t);
} pool[MAXN + 1], *cur = pool + 1, *null = pool, *id[MAXN + 1];

Node::Node() : cov(0), color(0) {
    c[0] = c[1] = fa = null;
    min.x = min.y = INT_MAX, max.x = max.y = INT_MIN;
}

Node::Node(const IdPoint &p)
    : p(p), min(p), max(p), cov(0), color(1), fa(null) {
    c[0] = c[1] = null;
}

inline void *Node::operator new(size_t) { return cur++; }

inline void Node::cover(int cov) {
    if (this == null) return;
    this->color = this->cov = cov;
}

bool flag;

inline bool cmp(const IdPoint &p1, const IdPoint &p2) {
    return flag ? (p1.y < p2.y || (p1.y == p2.y && p1.x < p2.x))
                : (p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y));
}

template <typename T>
inline T square(const T &x) {
    return x * x;
}

inline bool getSplit(int l, int r) {
    register double vx = 0, vy = 0, ax = 0, ay = 0;
    for (register int i = l; i <= r; i++) ax += p[i].x, ay += p[i].y;
    ax /= r - l + 1, ay /= r - l + 1;
    for (register int i = l; i <= r; i++)
        vx += square(p[i].x - ax), vy += square(p[i].y - ay);
    return vx < vy;
}

inline Node *build(int l, int r) {
    if (l > r) return null;
    register int mid = l + r >> 1;
    flag = getSplit(l, r), std::nth_element(p + l, p + mid, p + r + 1, cmp);
    register Node *o = new Node(p[mid]);
    id[p[mid].id] = o;
    o->c[0] = build(l, mid - 1), o->c[1] = build(mid + 1, r);
    if (o->c[0] != null) o->c[0]->fa = o;
    if (o->c[1] != null) o->c[1]->fa = o;
    return o->maintain(), o;
}

std::vector<int> edge[MAXN + 1];
typedef std::vector<int>::iterator Iterator;

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

int dfn[MAXN + 1], sz[MAXN + 1], dep[MAXN + 1], fa[MAXN + 1], idx;
bool vis[MAXN + 1];

inline void dfs(const int u) {
    vis[u] = true, dep[u] = dep[fa[u]] + 1, sz[u] = 1, dfn[u] = ++idx;
    for (Iterator v = edge[u].begin(); v != edge[u].end(); v++)
        if (!vis[*v]) fa[*v] = u, dfs(*v), sz[u] += sz[*v];
}

inline void init(const int n) {
    for (register int i = 0; i <= n; i++) edge[i].clear();
    idx = 0;
    memset(vis, 0, sizeof(bool) * (n + 1)), fa[1] = 0;
    cur = pool + 1;
}

inline void modify(Node *p, const Point &l, const Point &r, int color) {
    if (p == null || p->min.x > r.x || p->max.x < l.x || p->min.y > r.y ||
        p->max.y < l.y)
        return;
    p->pushDown();
    if (p->max.x <= r.x && p->min.x >= l.x && p->max.y <= r.y &&
        p->min.y >= l.y) {
        p->cov = p->color = color;
        return;
    }
    if (p->p.x <= r.x && p->p.x >= l.x && p->p.y <= r.y && p->p.y >= l.y)
        p->color = color;
    modify(p->c[0], l, r, color), modify(p->c[1], l, r, color);
}

inline int query(Node *p) {
    static Node *st[MAXN + 1];
    register int top = 0;
    for (; p != null; p = p->fa) st[++top] = p;
    while (top) st[top]->pushDown(), top--;
    return st[1]->color;
}

using IO::io;

inline void solve() {
    register int T;
    io >> T;
    while (T--) {
        register int n, c, q;
        io >> n >> c >> q;
        init(n);
        for (register int i = 2, v; i <= n; i++) io >> v, addEdge(i, v);
        dfs(1);
        for (register int i = 1; i <= n; i++)
            p[i].x = dfn[i], p[i].y = dep[i], p[i].id = i;
        register Node *root = build(1, n);
        register int ans = 0;
        for (register int x, y, cmd, i = 1; i <= q; i++) {
            io >> x >> y >> cmd;
            switch (cmd) {
                case 0:
                    ans = (ans + (long)query(id[x]) * i) % MOD;
                    break;
                default:
                    modify(root, Point(dfn[x], dep[x]),
                           Point(dfn[x] + sz[x] - 1, dep[x] + y), cmd);
                    break;
            }
        }
        io << ans << '\n';
    }
}
#undef long
}

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    solve();
    return 0;
}