/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3123」森林 12-10-2017
 * 可持久化线段树 + 倍增 + 启发式合并
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
inline bool read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return false;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
    return true;
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

inline void print(const char *s) {
    for (; *s; s++) print(*s);
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
const int MAX_LOG = 15;

int n, m, T;

struct Node {
    Node *lc, *rc;
    int size;

    Node();

    inline void *operator new(size_t);
};
Node *root[MAXN + 1], *null = new Node();

const int NODE_SIZE = sizeof(Node);

char pool[MAXN * 85 * NODE_SIZE], *cur = pool;

Node::Node() : lc(null), rc(null), size(0) {}

inline void *Node::operator new(size_t) { return cur += NODE_SIZE; }

void insert(Node *&p, Node *pre, int l, int r, int v) {
    p = new Node(*pre), p->size++;
    if (l == r) return;
    register int mid = l + r >> 1;
    v <= mid ? insert(p->lc, pre->lc, l, mid, v)
             : insert(p->rc, pre->rc, mid + 1, r, v);
}

inline int query(Node *u, Node *v, Node *lca, Node *fa, int l, int r, int k) {
    for (register int mid, now; l != r;) {
        mid = l + r >> 1;
        now = u->lc->size + v->lc->size - lca->lc->size - fa->lc->size;
        if (k <= now) {
            r = mid, u = u->lc, v = v->lc, lca = lca->lc, fa = fa->lc;
        } else {
            l = mid + 1, u = u->rc, v = v->rc, lca = lca->rc, fa = fa->rc;
            k -= now;
        }
    }
    return l;
}

int a[MAXN + 1], bound, tmp[MAXN + 1];

inline void init() {
    null->lc = null, null->rc = null;
    for (register int i = 1; i <= n; i++) io >> a[i];
    memcpy(tmp + 1, a + 1, sizeof(int) * n);
    std::sort(tmp + 1, tmp + n + 1);
    register int *end = std::unique(tmp + 1, tmp + n + 1);
    for (register int i = 1; i <= n; i++)
        a[i] = std::lower_bound(tmp + 1, tmp + n + 1, a[i]) - tmp;
    bound = end - tmp - 1;
    for (register int i = 0; i <= n; i++) root[i] = null;
}

std::vector<int> edge[MAXN + 1];

typedef std::vector<int>::iterator Iterator;

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

int id[MAXN + 1], pos[MAXN + 1], idx, sz[MAXN + 1];
int dep[MAXN + 1], fa[MAXN + 1][MAX_LOG + 1];
bool vis[MAXN + 1];

void dfs(const int u, const int pre) {
    vis[u] = true, sz[u] = 1, id[u] = idx;
    for (register int i = 1; i <= MAX_LOG; i++)
        fa[u][i] = fa[fa[u][i - 1]][i - 1];
    insert(root[u], root[pre], 1, bound, a[u]);
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); p++)
        if (*p != pre)
            fa[*p][0] = u, dep[*p] = dep[u] + 1, dfs(*p, u), sz[u] += sz[*p];
}

inline int lca(int u, int v) {
    dep[u] < dep[v] ? std::swap(u, v) : (void)0;
    register int d = dep[u] - dep[v];
    for (register int i = 0; i <= MAX_LOG; i++)
        if (d & (1 << i)) u = fa[u][i];
    for (register int i = MAX_LOG; i >= 0; i--)
        if (fa[u][i] != fa[v][i]) u = fa[u][i], v = fa[v][i];
    return u == v ? u : fa[u][0];
}

inline void solve() {
    io >> n >> n >> m >> T;
    init();
    for (register int i = 1, u, v; i <= m; i++) io >> u >> v, addEdge(u, v);
    for (register int i = 1; i <= n; ++i)
        if (!vis[i]) idx++, dfs(i, 0), pos[idx] = i;
    register char cmd;
    register int ans = 0;
    for (register int x, y, k; T--;) {
        io >> cmd >> x >> y, x ^= ans, y ^= ans;
        switch (cmd) {
            case 'Q': {
                io >> k, k ^= ans;
                register int f = lca(x, y);
                ans = tmp[query(root[x], root[y], root[f], root[fa[f][0]], 1,
                                bound, k)];
                io << ans << '\n';
                break;
            }
            case 'L': {
                register int u = pos[id[x]], v = pos[id[y]];
                sz[u] > sz[v] ? (std::swap(u, v), std::swap(x, y)) : (void)0;
                addEdge(y, x);
                fa[x][0] = y, sz[v] += sz[u], dep[x] = dep[y] + 1;
                idx = id[y], dfs(x, y);
                break;
            }
        }
    }
}
}

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    solve();
    return 0;
}