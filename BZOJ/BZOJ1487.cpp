/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1487」无归岛 28-10-2017
 * DP
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

struct InputOutputStream {
    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }
} io;
}  // namespace IO

namespace {

using IO::io;

const int MAXN = 100005;

std::vector<int> edge[MAXN + 1];
typedef std::vector<int>::iterator Iterator;

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

int dfn[MAXN + 1], idx, fa[MAXN + 1], w[MAXN + 1];

int f[MAXN + 1], g[MAXN + 1], n, m;

inline void dp(const int u, const int v) {
    register int u0 = 0, u1 = 0, v0 = 0, v1 = 0;
    for (register int o = v; o != u; o = fa[o])
        v0 = u1 + g[o], v1 = u0 + f[o], u0 = v0, u1 = std::max(v0, v1);
    g[u] += u1, u0 = INT_MIN / 2, u1 = 0;
    for (register int o = v; o != u; o = fa[o])
        v0 = u1 + g[o], v1 = u0 + f[o], u0 = v0, u1 = std::max(v0, v1);
    f[u] += u0;
}

void dfs(const int u) {
    dfn[u] = ++idx;
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); p++)
        if (!dfn[*p]) fa[*p] = u, dfs(*p);
    f[u] = w[u];
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); p++)
        if (dfn[u] < dfn[*p] && u != fa[*p]) dp(u, *p);
}

inline void solve() {
    io >> n >> m;
    for (register int i = 1, u, v; i <= m; i++) io >> u >> v, addEdge(u, v);
    for (register int i = 1; i <= n; i++) io >> w[i];
    dfs(1);
    std::cout << std::max(f[1], g[1]);
}
}  // namespace

int main() {
    solve();
    return 0;
}