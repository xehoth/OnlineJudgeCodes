/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3694」 25-11-2017
 * 树链剖分 + 线段树
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

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

const int OUT_LEN = 100000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
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

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
    ~InputOutputStream() { flush(); }

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

const int MAXN = 4000 + 9;
const int MAXM = 100000 + 9;
const int INF = 0x3f3f3f3f;

struct Node {
    int v, w;

    Node(int v, int w) : v(v), w(w) {}
};

std::vector<Node> edge[MAXN];

inline void addEdge(const int u, const int v, const int w) {
    edge[u].push_back(Node(v, w));
    edge[v].push_back(Node(u, w));
}

int dep[MAXN], dis[MAXN], top[MAXN], fa[MAXN];
int sz[MAXN], son[MAXN], pos[MAXN], idx;
bool vis[MAXN];

typedef std::vector<Node>::iterator Iterator;

void dfs1(const int u) {
    vis[u] = true, sz[u] = 1, dep[u] = dep[fa[u]] + 1;
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); ++p) {
        if (!vis[p->v]) {
            fa[p->v] = u, dis[p->v] = dis[u] + p->w, dfs1(p->v);
            sz[u] += sz[p->v];
            sz[p->v] > sz[son[u]] ? son[u] = p->v : 0;
        }
    }
}

void dfs2(const int u) {
    vis[u] = false, pos[u] = ++idx;
    top[u] = (u == son[fa[u]] ? top[fa[u]] : u);
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); ++p)
        if (p->v == son[u]) dfs2(p->v);
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); ++p)
        if (vis[p->v]) dfs2(p->v);
}

namespace SegmentTree {

struct Node {
    Node *lc, *rc;
    int min;

    Node();

    inline void *operator new(size_t);

    inline void cover(int);

    inline void pushDown() {
        if (min != INF) {
            lc->cover(min), rc->cover(min);
            min = INF;
        }
    }
};

const int NODE_SIZE = sizeof(Node);

char pool[MAXN * NODE_SIZE * 4], *cur = pool;

Node *null = (Node *)pool, *root = null;

Node::Node() : lc(null), rc(null), min(INF) {}

inline void *Node::operator new(size_t) { return cur += NODE_SIZE; }

inline void Node::cover(int v) {
    if (this == null) return;
    min = std::min(min, v);
}

inline void build(Node *&p, int l, int r) {
    p = new Node();
    if (l == r) return;
    register int mid = l + r >> 1;
    build(p->lc, l, mid), build(p->rc, mid + 1, r);
}

inline void modify(Node *p, int l, int r, int s, int t, int v) {
    if (s <= l && t >= r) return (void)(p->cover(v));
    register int mid = l + r >> 1;
    if (s <= mid) modify(p->lc, l, mid, s, t, v);
    if (t > mid) modify(p->rc, mid + 1, r, s, t, v);
}

inline int query(Node *p, int l, int r, int x) {
    if (l == r) return p->min;
    p->pushDown();
    register int mid = l + r >> 1;
    return x <= mid ? query(p->lc, l, mid, x) : query(p->rc, mid + 1, r, x);
}
}  // namespace SegmentTree

using SegmentTree::build;
using SegmentTree::modify;
using SegmentTree::null;
using SegmentTree::query;
using SegmentTree::root;

int n, m, cnt;

struct Edge {
    int u, v, w;

    Edge(int u, int v, int w) : u(u), v(v), w(w) {}

    Edge() {}
} edges[MAXM];

inline void modify(register int u, register int v, const int w) {
    while (top[u] != top[v]) {
        dep[top[u]] < dep[top[v]] ? std::swap(u, v) : (void)0;
        modify(root, 1, n, pos[top[u]], pos[u], w);
        u = fa[top[u]];
    }
    if (u == v) return;
    dep[u] < dep[v] ? std::swap(u, v) : (void)0;
    modify(root, 1, n, pos[son[v]], pos[u], w);
}

inline void solve() {
    null->lc = null->rc = null, null->min = INF;
    io >> n >> m;
    for (register int i = 1, u, v, w, t; i <= m; i++) {
        io >> u >> v >> w >> t;
        if (t == 0) {
            edges[++cnt] = Edge(u, v, w);
        } else {
            addEdge(u, v, w);
        }
    }
    dfs1(1), dfs2(1);
    build(root, 1, n);
    for (register int i = 1; i <= cnt; i++) {
        modify(edges[i].u, edges[i].v,
               edges[i].w + dis[edges[i].u] + dis[edges[i].v]);
    }
    for (register int i = 2, ans; i <= n; i++) {
        ans = query(root, 1, n, pos[i]);
        if (ans != INF)
            io << ans - dis[i] << ' ';
        else
            io << "-1 ";
    }
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}