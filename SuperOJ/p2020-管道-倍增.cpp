/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 2020」管道 30-10-2017
 * 倍增
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

const int MAXN = 100000;
const int MAXM = MAXN * 2;

namespace UnionFindSet {

int fa[MAXN + 1];

inline int get(const register int x) {
    return x == fa[x] ? x : fa[x] = get(fa[x]);
}
}

namespace {

using IO::io;
using UnionFindSet::get;
const int MAX_LOG = 17;

bool vis[MAXM + 1];

struct Edge {
    int u, v, w, id;

    inline bool operator<(const Edge &p) const { return w < p.w; }
} edges[MAXM + 1];

struct Node {
    int v, w;

    Node(int v, int w) : v(v), w(w) {}
};

std::vector<Node> edge[MAXN + 1];

typedef std::vector<Node>::iterator Iterator;

inline void addEdge(const int u, const int v, const int w) {
    edge[u].push_back(Node(v, w)), edge[v].push_back(Node(u, w));
}

int fa[MAX_LOG][MAXN + 1], max[MAX_LOG][MAXN + 1], dep[MAXN + 1];
long long ans[MAXM + 1];

inline int optMax(const register int a, const register int b) {
    return a > b ? a : b;
}

void dfs(const int u) {
    dep[u] = dep[fa[0][u]] + 1;
    for (register int i = 1; (1 << i) <= dep[u]; i++) {
        fa[i][u] = fa[i - 1][fa[i - 1][u]];
        max[i][u] = optMax(max[i - 1][u], max[i - 1][fa[i - 1][u]]);
    }
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); p++)
        if (p->v != fa[0][u]) fa[0][p->v] = u, max[0][p->v] = p->w, dfs(p->v);
}

inline int getMax(register int u, register int v) {
    register int ret = INT_MIN;
    dep[u] < dep[v] ? std::swap(u, v) : (void)0;
    register int delta = dep[u] - dep[v];
    for (register int i = 0; (1 << i) <= delta; i++)
        if (delta & (1 << i)) ret = optMax(ret, max[i][u]), u = fa[i][u];
    if (u == v) return ret;
    for (register int i = MAX_LOG - 1; i >= 0; i--) {
        if (fa[i][u] != fa[i][v]) {
            ret = optMax(ret, max[i][u]), u = fa[i][u];
            ret = optMax(ret, max[i][v]), v = fa[i][v];
        }
    }
    return optMax(ret, optMax(max[0][u], max[0][v]));
}

inline void solve() {
    register int n, m;
    io >> n >> m;
    for (register int i = 1; i <= n; i++) UnionFindSet::fa[i] = i;
    for (register int i = 1; i <= m; i++)
        io >> edges[i].u >> edges[i].v >> edges[i].w, edges[i].id = i;
    std::sort(edges + 1, edges + m + 1);
    register long long cost = 0;
    for (register int i = 1, cnt = 0, u, v; i <= m; i++) {
        if ((u = get(edges[i].u)) != (v = get(edges[i].v))) {
            UnionFindSet::fa[u] = v, cost += edges[i].w;
            addEdge(edges[i].u, edges[i].v, edges[i].w);
            vis[i] = true;
            if (++cnt == n - 1) break;
        }
    }
    dfs(1);
    for (register int i = 1; i <= m; i++) {
        if (vis[i]) {
            ans[edges[i].id] = cost;
            continue;
        }
        ans[edges[i].id] = cost - getMax(edges[i].u, edges[i].v) + edges[i].w;
    }
    for (register int i = 1; i <= m; i++) io << ans[i] << '\n';
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}