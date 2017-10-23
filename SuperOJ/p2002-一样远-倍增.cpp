/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 2002」一样远 23-10-2017
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

namespace {

using IO::io;

const int MAXN = 100000;
const int MAX_LOG = 18;

std::vector<int> edge[MAXN + 1];

typedef std::vector<int>::iterator Iterator;

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

int in[MAXN + 1], out[MAXN + 1], fa[MAX_LOG][MAXN + 1], sz[MAXN + 1];
int n, m, dep[MAXN + 1], idx;
bool vis[MAXN + 1];

inline bool isAncestor(const int u, const int v) {
    return in[u] <= in[v] && out[u] >= out[v];
}

void dfs(const int u) {
    vis[u] = true, dep[u] = dep[fa[0][u]] + 1, sz[u] = 1, in[u] = idx++;
    for (register int i = 1; i < MAX_LOG; i++)
        fa[i][u] = fa[i - 1][fa[i - 1][u]];
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); p++)
        if (!vis[*p]) fa[0][*p] = u, dfs(*p), sz[u] += sz[*p];
    out[u] = idx++;
}

inline int bitUp(register int u, register int v) {
    for (register int i = MAX_LOG - 1; i >= 0; i--)
        !isAncestor(fa[i][u], v) ? u = fa[i][u] : 0;
    return u;
}

inline int lca(const int u, const int v) {
    return isAncestor(u, v) ? u : (isAncestor(v, u) ? v : fa[0][bitUp(u, v)]);
}

inline void query(int u, int v) {
    if (u == v) {
        io << n << '\n';
        return;
    }
    register int l = lca(u, v);
    if (dep[u] - dep[l] == dep[v] - dep[l]) {
        io << n - sz[bitUp(u, l)] - sz[bitUp(v, l)] << '\n';
        return;
    }
    dep[u] < dep[v] ? std::swap(u, v) : (void)0;
    register int dis = dep[u] + dep[v] - 2 * dep[l];
    if (dis & 1) {
        io << "0\n";
        return;
    }
    dis /= 2;
    register int mid = u;
    for (register int i = MAX_LOG - 1; i >= 0; i--)
        dep[u] - dep[fa[i][mid]] < dis ? mid = fa[i][mid] : 0;
    io << sz[fa[0][mid]] - sz[mid] << '\n';
}

inline void solve() {
    io >> n, fa[0][1] = 1;
    for (register int i = 1, u, v; i < n; i++) io >> u >> v, addEdge(u, v);
    dfs(1), io >> m;
    for (register int u, v; m--;) io >> u >> v, query(u, v);
}
}

int main() {
    solve();
    return 0;
}