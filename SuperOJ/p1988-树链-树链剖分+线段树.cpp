/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 1988」树链 16-10-2017
 * 树链剖分 + 线段树
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

std::vector<int> edge[MAXN + 1];

typedef std::vector<int>::iterator Iterator;

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

int dep[MAXN + 1], fa[MAXN + 1], sz[MAXN + 1], idx;
int son[MAXN + 1], top[MAXN + 1], pos[MAXN + 1];
bool vis[MAXN + 1];

void dfs1(const int u) {
    vis[u] = true, dep[u] = dep[fa[u]] + 1, sz[u] = 1;
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); p++) {
        if (!vis[*p]) {
            fa[*p] = u, dfs1(*p), sz[u] += sz[*p];
            sz[*p] > sz[son[u]] ? son[u] = *p : 0;
        }
    }
}

void dfs2(const int u) {
    vis[u] = false, pos[u] = ++idx, top[u] = (u == son[fa[u]] ? top[fa[u]] : u);
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); p++)
        if (*p == son[u]) dfs2(*p);
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); p++)
        if (vis[*p]) dfs2(*p);
}

inline int lca(register int u, register int v) {
    while (top[u] != top[v])
        dep[top[u]] < dep[top[v]] ? v = fa[top[v]] : u = fa[top[u]];
    return dep[u] < dep[v] ? u : v;
}

int d[MAXN * 4], M;

inline void build(const int n) {
    for (M = 1; M < n + 2; M <<= 1)
        ;
}

inline int query(register int s, register int t) {
    register int ret = 0;
    for (s = s + M - 1, t = t + M + 1; s ^ t ^ 1; s >>= 1, t >>= 1) {
        (~s & 1) ? ret += d[s ^ 1] : 0;
        (t & 1) ? ret += d[t ^ 1] : 0;
    }
    return ret;
}

inline void modify(register int k, register int v) {
    for (k += M; k; k >>= 1) d[k] += v;
}

struct Query {
    int u, v, w;

    Query(int u = 0, int v = 0, int w = 0) : u(u), v(v), w(w) {}
};

std::vector<Query> q[MAXN + 1];

int dp[MAXN + 1];

inline void query(int cur) {
    register int u, v, w;
    for (register std::vector<Query>::iterator p = q[cur].begin();
         p != q[cur].end(); p++) {
        u = p->u, v = p->v, w = p->w;
        while (top[u] != top[v]) {
            (dep[top[u]] < dep[top[v]]) ? std::swap(u, v) : (void)0;
            w += query(pos[top[u]], pos[u]), u = fa[top[u]];
        }
        (dep[u] > dep[v]) ? std::swap(u, v) : (void)0;
        w += query(pos[u], pos[v]);
        dp[cur] = std::max(w, dp[u]);
    }
    modify(pos[cur], -dp[cur]);
    (fa[cur]) ? modify(pos[fa[cur]], dp[cur]) : (void)0;
}

void dfs(int u, int fa) {
    dp[u] = 0;
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); p++)
        if (*p != fa) dfs(*p, u), dp[u] += dp[*p];
    query(u);
}

inline void solve() {
    register int n, m;
    io >> n >> m;
    for (register int i = 1, u, v; i < n; i++) io >> u >> v, addEdge(u, v);
    dfs1(1), dfs2(1), build(n);
    for (register int i = 1, u, v, w; i <= m; i++)
        io >> u >> v >> w, q[lca(u, v)].push_back(Query(u, v, w));
    dfs(1, 0);
    io << dp[1] << '\n';
}
}

int main() {
    // freopen("chain.in", "r", stdin);
    // freopen("chain.out", "w", stdout);
    solve();
    return 0;
}