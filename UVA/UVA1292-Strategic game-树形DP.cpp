/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「UVA 1292」Strategic game 21-10-2017
 * 树形 DP
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 100000;
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
    return iosig ? x = -x : 0, true;
}

struct InputOutputStream {
    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        return read(x), *this;
    }
} io;
}

namespace {

using IO::io;

const int MAXN = 1500;

std::vector<int> edge[MAXN + 1];
int n;

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

typedef std::vector<int>::iterator Iterator;

int f[MAXN + 1][2];

void dfs(const int u, const int fa) {
    f[u][0] = 0, f[u][1] = 1;
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); p++) {
        if (*p != fa) {
            dfs(*p, u);
            f[u][0] += f[*p][1];
            f[u][1] += std::min(f[*p][0], f[*p][1]);
        }
    }
}

inline void solveCase() {
    for (register int i = 0; i <= n; i++) edge[i].clear();
    for (register int i = 0, u, m; i < n; i++) {
        io >> u >> m;
        for (register int j = 0, v; j < m; j++) io >> v, addEdge(u, v);
    }
    dfs(0, -1);
    std::cout << std::min(f[0][0], f[0][1]) << '\n';
}

inline void solve() {
    while (IO::read(n)) solveCase();
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}