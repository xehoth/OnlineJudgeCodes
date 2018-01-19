/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 4012」开店 15-01-2018
 * 可持久化点分树
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

inline char read() {
    static const int IN_LEN = 1 << 18 | 1;
    static char buf[IN_LEN], *s, *t;
    return (s == t) && (t = (s = buf) + fread(buf, 1, IN_LEN, stdin)),
           s == t ? -1 : *s++;
}

const int OUT_LEN = 1 << 18 | 1;
char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    (oh == obuf + OUT_LEN) && (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf);
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[21], cnt;
    if (x != 0) {
        (x < 0) && (print('-'), x = -x);
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    } else {
        print('0');
    }
}

struct InputOutputStream {
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
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

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 150000 + 9;
const int MAX_LOG = 19;
const int MAX_DEG = 3;

struct Edge {
    int v, w;

    Edge(int v, int w) : v(v), w(w) {}
};

std::vector<Edge> edge[MAXN];

typedef std::vector<Edge>::iterator Iterator;

inline void addEdge(const int u, const int v, const int w) {
    edge[u].push_back(Edge(v, w));
    edge[v].push_back(Edge(u, w));
}

bool vis[MAXN];
int sz[MAXN], dep[MAXN], idx[MAXN][MAX_LOG];
long long dis[MAXN][MAX_LOG];

void dfsSize(const int u, const int pre) {
    sz[u] = 1;
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); ++p) {
        if (!vis[p->v] && p->v != pre) {
            dfsSize(p->v, u);
            sz[u] += sz[p->v];
        }
    }
}

int getCenter(const int u, const int pre, const int size) {
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); ++p)
        if (!vis[p->v] && p->v != pre && sz[p->v] > size)
            return getCenter(p->v, u, size);
    return u;
}

void dfsDis(const int u, const int pre, const int d) {
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); ++p) {
        if (!vis[p->v] && p->v != pre) {
            idx[p->v][d] = idx[u][d];
            dis[p->v][d] = dis[u][d] + p->w;
            dfsDis(p->v, u, d);
        }
    }
}

int build(int u, int d) {
    dfsSize(u, 0);
    register int root = getCenter(u, 0, sz[u] / 2), c = 0;
    vis[root] = true;
    dep[root] = d;
    for (register Iterator p = edge[root].begin(); p != edge[root].end(); ++p) {
        if (!vis[p->v]) {
            dis[p->v][d] = p->w;
            idx[p->v][d] = c++;
            dfsDis(p->v, root, d);
            build(p->v, d + 1);
        }
    }
    return root;
}

std::pair<int, int> x[MAXN];

char *cur;

struct Node *null;

struct Node {
    static const int NODE_SIZE;
    Node *c[MAX_DEG];
    long long s, sf, size;

    inline void *operator new(size_t) { return cur += NODE_SIZE; }

    Node() { c[0] = c[1] = c[2] = null; }
};

const int Node::NODE_SIZE = sizeof(Node);

char pool[MAXN * MAX_LOG * Node::NODE_SIZE];

Node *root[MAXN];

inline void insert(Node *&p, Node *pre, int u, int d) {
    p = new Node(*pre);
    p->s += dis[u][d];
    p->sf += dis[u][d - 1];
    p->size++;
    if (d != dep[u]) insert(p->c[idx[u][d]], pre->c[idx[u][d]], u, d + 1);
}

inline long long query(Node *p, int u, int d) {
    return (d == dep[u] ? 0 : query(p->c[idx[u][d]], u, d + 1)) + p->s - p->sf +
           p->size * (dis[u][d] - dis[u][d - 1]);
}

int n, q, p;

inline int id(int a) {
    register int l = 0, r = n + 1;
    for (register int mid = 0; r - l > 1;
         x[mid = (l + r) >> 1].first >= a ? r = mid : l = mid)
        ;
    return l;
}

inline void solve() {
    cur = pool;
    null = (Node *)pool;
    null->c[0] = null->c[1] = null->c[2] = null;
    root[0] = null;
    io >> n >> q >> p;
    for (register int i = 1, t; i <= n; i++) {
        io >> t;
        x[i] = std::make_pair(t, i);
    }
    std::sort(x + 1, x + n + 1);
    for (register int i = 1, u, v, w; i < n; i++) {
        io >> u >> v >> w;
        addEdge(u, v, w);
    }
    register int center = build(1, 1);
    for (register int i = 1; i <= n; i++)
        insert(root[i], root[i - 1], x[i].second, 1);
    register long long ans = 0;
    for (register int u, a, b; q--;) {
        io >> u >> a >> b;
        a = (a + ans) % p;
        b = (b + ans) % p;
        if (a > b) std::swap(a, b);
        ans = query(root[id(b + 1)], u, 1) - query(root[id(a)], u, 1);
        io << ans << '\n';
    }
}
}  // namespace

int main() {
    // freopen("in.in", "r", stdin);
    solve();
    return 0;
}