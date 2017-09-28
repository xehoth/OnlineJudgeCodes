/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2427」软件安装 28-09-2017
 * 缩点 + 树形 DP
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

#define long long long

const int MAXN = 110;
const int MAXV = 510;

int n, m;

struct Graph {
    typedef std::vector<int> Vector;
    Vector edge[MAXN + 1];

    inline void addEdge(const int u, const int v) { edge[u].push_back(v); }

    inline Vector &operator[](const int i) { return edge[i]; }
};

typedef Graph::Vector::iterator Iterator;

Graph g, newG;

int W[MAXN + 1], V[MAXN + 1], nW[MAXN + 1], nV[MAXN + 1];
int dfn[MAXN + 1], low[MAXN + 1], idx, scc[MAXN + 1], cnt;
bool vis[MAXN + 1];
int f[MAXN + 1][MAXV + 1], in[MAXN + 1];

inline void tarjan(const int u) {
    static std::vector<int> st;
    st.push_back(u), vis[u] = true, dfn[u] = low[u] = ++idx;
    register int v;
    for (Iterator p = g[u].begin(); p != g[u].end(); p++) {
        if (!dfn[v = *p])
            tarjan(v), low[u] = std::min(low[u], low[v]);
        else if (vis[v])
            low[u] = std::min(low[u], dfn[v]);
    }
    if (low[u] == dfn[u]) {
        cnt++;
        do {
            vis[v = st.back()] = false, st.pop_back(), scc[v] = cnt;
            nW[cnt] += W[v], nV[cnt] += V[v];
        } while (u != v);
    }
}

inline void dfs(const int u) {
    for (Iterator v = newG[u].begin(); v != newG[u].end(); v++) {
        dfs(*v);
        for (register int j = m - nW[u]; j >= 0; j--)
            for (register int k = 0; k <= j; k++)
                f[u][j] = std::max(f[u][j], f[u][k] + f[*v][j - k]);
    }
    for (register int j = m; j >= nW[u]; j--) f[u][j] = f[u][j - nW[u]] + nV[u];
    for (register int j = nW[u] - 1; j >= 0; j--) f[u][j] = 0;
}

inline void solve() {
    io >> n >> m;
    for (register int i = 1; i <= n; i++) io >> W[i];
    for (register int i = 1; i <= n; i++) io >> V[i];
    for (register int i = 1, d; i <= n; i++) {
        io >> d;
        if (d != 0) g.addEdge(d, i);
    }
    for (register int i = 1; i <= n; i++)
        if (!dfn[i]) tarjan(i);
    for (register int i = 1; i <= n; i++)
        for (Iterator v = g[i].begin(); v != g[i].end(); v++)
            if (scc[i] != scc[*v]) newG.addEdge(scc[i], scc[*v]), in[scc[*v]]++;
    for (register int i = 1; i <= cnt; i++)
        if (in[i] == 0) newG.addEdge(cnt + 1, i);

    dfs(cnt + 1);
    io << f[cnt + 1][m];
}

#undef long
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}
