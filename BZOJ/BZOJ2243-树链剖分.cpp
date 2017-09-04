/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2243」染色 05-09-2017
 * 树链剖分
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
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

inline void read(char &c) {
    while (c = read(), isspace(c) && c != -1)
        ;
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

const int OUT_LEN = 1000000;

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

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
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

    ~InputOutputStream() { flush(); }
} io;
}

namespace {

using IO::io;

const int MAXN = 100000;
const int MAXM = MAXN * 4;

struct Graph {
    typedef std::vector<int> Vector;

    Vector edge[MAXN + 1];

    inline void addEdge(const int u, const int v) {
        edge[u].push_back(v), edge[v].push_back(u);
    }

    inline Vector &operator[](const int i) { return edge[i]; }
};

struct Node {
    Node *lc, *rc;
    int lColor, rColor;
    int tag, sum;

    Node();

    inline void *operator new(size_t);

    inline void cover(int c) { lColor = rColor = c, sum = 1, tag = c; }

    inline void pushDown(int l, int r);

    inline void maintain();
} pool[MAXM + 1], *cur = pool + 1, *null = pool;

Node::Node() : lc(null), rc(null), lColor(0), rColor(0), sum(1), tag(-1) {}

inline void *Node::operator new(size_t) { return cur++; }

inline void Node::pushDown(int l, int r) {
    if (tag == -1 || l == r) return;
    lc->cover(tag), rc->cover(tag), tag = -1;
    maintain();
}

inline void Node::maintain() {
    if (this == null) return;
    sum = lc->sum + rc->sum - (rc->lColor == lc->rColor);
    lColor = lc->lColor, rColor = rc->rColor;
}

class SegmentTree {
   private:
    Node *root;
    int n;

    inline void build(Node *&p, int l, int r, const int *a, const int *id) {
        p = new Node();
        if (l == r) {
            p->lColor = p->rColor = a[id[l]];
            return;
        }
        register int mid = l + r >> 1;
        build(p->lc, l, mid, a, id), build(p->rc, mid + 1, r, a, id);
        p->maintain();
    }

    inline int query(Node *p, int l, int r, int s, int t) {
        if (s <= l && t >= r) return p->sum;
        p->pushDown(l, r);
        register int mid = l + r >> 1, ret = 0;
        register int tmp =
            (p->lc->rColor == p->rc->lColor) && s <= mid && t > mid;
        if (s <= mid) ret += query(p->lc, l, mid, s, t);
        if (t > mid) ret += query(p->rc, mid + 1, r, s, t);
        return ret - tmp;
    }

    inline void modify(Node *p, int l, int r, int s, int t, int c) {
        if (s <= l && t >= r) {
            p->cover(c);
            return;
        }
        p->pushDown(l, r);
        register int mid = l + r >> 1;
        if (s <= mid) modify(p->lc, l, mid, s, t, c);
        if (t > mid) modify(p->rc, mid + 1, r, s, t, c);
        p->maintain();
    }

   public:
    inline void init() {
        null->lc = null->rc = null, null->sum = 0;
        null->tag = -1, null->lColor = null->rColor = 0;
    }

    inline void build(const int l, const int r, const int *a, const int *id) {
        this->n = r;
        build(root, l, r, a, id);
    }

    inline int query(int l, int r) { return query(root, 1, n, l, r); }

    inline void modify(int l, int r, int c) { modify(root, 1, n, l, r, c); }

    inline int getColor(int pos) {
        register int l = 1, r = n, mid;
        Node *p = root;
        for (; l != r;) {
            p->pushDown(l, r);
            mid = l + r >> 1;
            if (pos <= mid)
                p = p->lc, r = mid;
            else
                p = p->rc, l = mid + 1;
        }
        return p->lColor;
    }
};

struct HeavyLightChainDecomposition {
    Graph g;
    SegmentTree segmentTree;
    typedef Graph::Vector::iterator Iterator;

    int sz[MAXN + 1], dep[MAXN + 1], fa[MAXN + 1], idx;
    int top[MAXN + 1], son[MAXN + 1], pos[MAXN + 1], id[MAXN + 1];
    bool vis[MAXN + 1];

    inline void dfs1(const int u) {
        vis[u] = true, sz[u] = 1, dep[u] = dep[fa[u]] + 1;
        for (Iterator v = g[u].begin(); v != g[u].end(); v++) {
            if (!vis[*v]) {
                fa[*v] = u, dfs1(*v), sz[u] += sz[*v];
                sz[*v] > sz[son[u]] ? son[u] = *v : 0;
            }
        }
    }

    inline void dfs2(const int u) {
        vis[u] = false, pos[u] = ++idx, id[idx] = u,
        top[u] = (u == son[fa[u]] ? top[fa[u]] : u);
        for (Iterator v = g[u].begin(); v != g[u].end(); v++)
            if (*v == son[u]) dfs2(*v);
        for (Iterator v = g[u].begin(); v != g[u].end(); v++)
            if (vis[*v]) dfs2(*v);
    }

    inline int lca(int u, int v) {
        while (top[u] != top[v])
            dep[top[u]] < dep[top[v]] ? v = fa[top[v]] : u = fa[top[u]];
        return dep[u] < dep[v] ? u : v;
    }

    inline void cut(int root = 1) { dfs1(root), dfs2(root); }

    int color[MAXN + 1];

    inline void modify(int u, int v, int w) {
        while (top[u] != top[v]) {
            dep[top[u]] < dep[top[v]] ? std::swap(u, v) : (void)0;
            segmentTree.modify(pos[top[u]], pos[u], w);
            u = fa[top[u]];
        }
        dep[u] < dep[v] ? std::swap(u, v) : (void)0;
        segmentTree.modify(pos[v], pos[u], w);
    }

    inline int query(int u, int v) {
        register int ret = 0;
        while (top[u] != top[v]) {
            dep[top[u]] < dep[top[v]] ? std::swap(u, v) : (void)0;
            ret += segmentTree.query(pos[top[u]], pos[u]);
            if (segmentTree.getColor(pos[top[u]]) ==
                segmentTree.getColor(pos[fa[top[u]]]))
                ret--;
            u = fa[top[u]];
        }
        dep[u] < dep[v] ? std::swap(u, v) : (void)0;
        return ret + segmentTree.query(pos[v], pos[u]);
    }

    inline void solve() {
        register int n, m;
        io >> n >> m;
        for (register int i = 1; i <= n; i++) io >> color[i];
        for (register int i = 1, u, v; i < n; i++)
            io >> u >> v, g.addEdge(u, v);
        cut(), segmentTree.init();
        segmentTree.build(1, n, color, id);
        static char cmd[5];
        for (register int u, v, w, t; m--;) {
            io >> cmd;
            switch (cmd[0]) {
                case 'C':
                    io >> u >> v >> w;
                    modify(u, v, w);
                    break;
                case 'Q':
                    io >> u >> v;
                    io << query(u, v) << '\n';
                    break;
                default:
                    assert(false);
            }
        }
    }
} task;
}

int main() {
    task.solve();
    return 0;
}