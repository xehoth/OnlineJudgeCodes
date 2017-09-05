/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2286」「SDOI 2011」消耗战 05-09-2017
 * 虚树
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

const int MAXN = 250000;

struct Node {
    int v, w;

    Node(int v, int w) : v(v), w(w) {}
};

template <typename T>
struct Graph {
    typedef std::vector<T> Vector;
    Vector edge[MAXN + 1];

    inline void addEdge(const int u, const T &v) { edge[u].push_back(v); }

    inline Vector &operator[](const int i) { return edge[i]; }

    inline void clear(const int n) {
        for (register int i = 0; i <= n; i++) edge[i].clear();
    }
};

struct HeavyLightChainDecomposition {
    Graph<Node> g;
    typedef Graph<Node>::Vector::iterator Iterator;

    int fa[MAXN + 1], dep[MAXN + 1], sz[MAXN + 1], w[MAXN + 1];
    int top[MAXN + 1], son[MAXN + 1], dfn[MAXN + 1], idx;
    bool vis[MAXN + 1];

    inline void dfs1(const int u) {
        vis[u] = true, sz[u] = 1, dep[u] = dep[fa[u]] + 1;
        for (Iterator p = g[u].begin(); p != g[u].end(); p++) {
            if (!vis[p->v]) {
                fa[p->v] = u, w[p->v] = std::min(w[u], p->w);
                dfs1(p->v);
                sz[u] += sz[p->v], sz[p->v] > sz[son[u]] ? son[u] = p->v : 0;
            }
        }
    }

    inline void dfs2(const int u) {
        vis[u] = false, dfn[u] = ++idx,
        top[u] = (u == son[fa[u]] ? top[fa[u]] : u);
        for (Iterator p = g[u].begin(); p != g[u].end(); p++)
            if (p->v == son[u]) dfs2(p->v);
        for (Iterator p = g[u].begin(); p != g[u].end(); p++)
            if (vis[p->v]) dfs2(p->v);
    }

    inline int lca(int u, int v) {
        while (top[u] != top[v])
            dep[top[u]] < dep[top[v]] ? v = fa[top[v]] : u = fa[top[u]];
        return dep[u] < dep[v] ? u : v;
    }

    inline void cut(int root = 1) { dfs1(root), dfs2(root); }

    inline void init(const int n) {
        for (register int i = 1, u, v, w; i < n; i++) {
            io >> u >> v >> w;
            g.addEdge(u, Node(v, w)), g.addEdge(v, Node(u, w));
        }
        w[1] = INT_MAX;
        cut();
    }
};

inline bool cmp(int, int);

struct VirtualTree {
#define long long long
    HeavyLightChainDecomposition hld;

    Graph<int> g;

    typedef Graph<int>::Vector::iterator Iterator;

    int *w, *dep, *dfn;

    inline void addEdge(const int u, const int v) {
        g.addEdge(u, v), g.addEdge(v, u);
    }

    inline int build(int *p, int n) {
        std::sort(p, p + n, cmp);
        register int cnt = 0;
        for (register int i = 1; i < n; i++)
            if (hld.lca(p[i], p[cnt]) != p[cnt]) p[++cnt] = p[i];
        n = ++cnt;
        static std::vector<int> st;
        st.clear(), st.reserve(n), st.push_back(1), p[cnt++] = 1;
        for (register int i = 0, u, lca; i < n; i++) {
            u = p[i], lca = hld.lca(u, st.back());
            if (lca == st.back()) {
                st.push_back(u);
            } else {
                while (st.size() >= 2 && dep[*++st.rbegin()] >= dep[lca])
                    addEdge(*++st.rbegin(), st.back()), st.pop_back();
                if (st.back() != lca)
                    addEdge(lca, st.back()), p[cnt++] = lca, st.back() = lca;
                st.push_back(u);
            }
        }
        for (register int i = 0; i < st.size() - 1; i++)
            addEdge(st[i], st[i + 1]);
        return cnt;
    }

    inline long dfs(int u, int v) {
        register long ret = 0;
        for (Iterator p = g[u].begin(); p != g[u].end(); p++)
            if (*p != v) ret += std::min(dfs(*p, u), (long)w[*p]);
        return ret ? ret : LLONG_MAX;
    }

    inline void solve() {
        register int n;
        io >> n, hld.init(n);
        w = hld.w, dfn = hld.dfn, dep = hld.dep;
        register int m, k;
        io >> m;
        static int a[MAXN + 1];
        while (m--) {
            io >> k;
            for (register int i = 0; i < k; i++) io >> a[i];
            k = build(a, k);
            io << dfs(1, 0) << '\n';
            for (register int i = 0; i < k; i++) g[a[i]].clear();
        }
    }
#undef long
} task;

inline bool cmp(const int u, const int v) { return task.dfn[u] < task.dfn[v]; }
}

int main() {
    task.solve();
    return 0;
}