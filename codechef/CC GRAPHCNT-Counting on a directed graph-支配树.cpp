/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「CC GRAPHCNT」Counting on a directed graph 06-09-2017
 * 支配树
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

struct Graph {
    typedef std::vector<int> Vector;
    Vector edge[MAXN + 1];

    inline void addEdge(const int u, const int v) { edge[u].push_back(v); }

    inline Vector &operator[](const int i) { return edge[i]; }
};

inline bool cmp(const int, const int);

struct DominatorTree {
#define long long long
    Graph cur, rev;

    typedef Graph::Vector::iterator Iterator;

    int ufs[MAXN + 1], val[MAXN + 1];  // 并查集
    int fa[MAXN + 1];                  // dfs 树上的父亲
    int dfn[MAXN + 1];                 // dfs 序
    int id[MAXN + 1];                  // dfs 序对应的节点编号
    int idx;
    int sdom[MAXN + 1];  // 半支配点
    int idom[MAXN + 1];  // 最近支配点

    Graph dom;  // 过程中先储存半支配点为 u 的点集，最后为支配树

    inline void addEdge(const int u, const int v) {
        cur.addEdge(u, v), rev.addEdge(v, u);
    }

    inline void dfs(const int u) {
        id[dfn[u] = ++idx] = u;
        for (Iterator v = cur[u].begin(); v != cur[u].end(); v++)
            if (!dfn[*v]) fa[*v] = u, dfs(*v);
    }

    inline int get(int x) {
        if (x == ufs[x]) return x;
        register int y = get(ufs[x]);
        if (cmp(sdom[val[ufs[x]]], sdom[val[x]])) val[x] = val[ufs[x]];
        return ufs[x] = y;
    }

    inline void build(const int s, const int n) {
        for (register int i = 1; i <= n; i++) ufs[i] = val[i] = sdom[i] = i;
        dfs(s);
        for (register int i = idx, u; u = id[i], i > 1; i--) {
            for (Iterator v = rev[u].begin(); v != rev[u].end(); v++)
                if (dfn[*v])
                    get(*v), sdom[u] = std::min(sdom[u], sdom[val[*v]], cmp);
            dom.addEdge(sdom[u], u);
            register int x = (ufs[u] = fa[u]);
            for (Iterator v = dom[x].begin(); v != dom[x].end(); v++)
                get(*v), idom[*v] = cmp(sdom[val[*v]], fa[u]) ? val[*v] : fa[u];
            dom[x].clear();
        }
        for (register int i = 2, u; u = id[i], i <= idx; i++) {
            idom[u] != sdom[u] ? idom[u] = idom[idom[u]] : 0;
            dom.addEdge(idom[u], u);
        }
    }

    int sz[MAXN + 1];

    inline void getSize(const int u) {
        sz[u] = 1;
        for (Iterator v = dom[u].begin(); v != dom[u].end(); v++)
            getSize(*v), sz[u] += sz[*v];
    }

    inline void solve() {
        register int n, m;
        io >> n >> m;
        for (register int i = 0, u, v; i < m; i++) {
            io >> u >> v, addEdge(u, v);
        }
        build(1, n);
        getSize(1);

        register long tmp = 1, ans = 0;
        for (Iterator v = dom[1].begin(); v != dom[1].end(); v++)
            ans += (long)sz[*v] * tmp, tmp += sz[*v];
        io << ans;
    }

} task;

inline bool cmp(const int u, const int v) { return task.dfn[u] < task.dfn[v]; }

#undef long
}

int main() {
    task.solve();
    return 0;
}
