/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3631」松鼠的新家 27-09-2017
 * 树上差分
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
    for (iosig = false, c = read(); !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
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

inline void read(double &x) {
    static char buf[30];
    read(buf), x = atof(buf);
}

const int OUT_LEN = 10000000;

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

inline void print(double x) {
    static char buf[30];
    sprintf(buf, "%.1f", x);
    print((const char *)buf);
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

#define long long long

using IO::io;

const int MAXN = 300000;

std::vector<int> edge[MAXN + 1];

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

typedef std::vector<int>::iterator Iterator;

int dep[MAXN + 1], top[MAXN + 1], sz[MAXN + 1], fa[MAXN + 1], son[MAXN + 1];
bool vis[MAXN + 1];

inline void dfs1(const int u) {
    vis[u] = true, sz[u] = 1, dep[u] = dep[fa[u]] + 1;
    for (Iterator v = edge[u].begin(); v != edge[u].end(); v++) {
        if (!vis[*v]) {
            fa[*v] = u, dfs1(*v), sz[u] += sz[*v];
            sz[*v] > sz[son[u]] ? son[u] = *v : 0;
        }
    }
}

inline void dfs2(const int u) {
    vis[u] = false, top[u] = (u == son[fa[u]] ? top[fa[u]] : u);
    for (Iterator v = edge[u].begin(); v != edge[u].end(); v++)
        if (*v == son[u]) dfs2(*v);
    for (Iterator v = edge[u].begin(); v != edge[u].end(); v++)
        if (vis[*v]) dfs2(*v);
}

inline int lca(int u, int v) {
    while (top[u] != top[v])
        dep[top[u]] < dep[top[v]] ? v = fa[top[v]] : u = fa[top[u]];
    return dep[u] < dep[v] ? u : v;
}

int a[MAXN + 1], sum[MAXN + 1];

inline int dp(const int u) {
    vis[u] = true;
    for (Iterator v = edge[u].begin(); v != edge[u].end(); v++)
        if (!vis[*v]) dp(*v), sum[u] += dp(*v);
    return sum[u];
}

inline void solve() {
    register int n;
    io >> n;
    for (register int i = 1; i <= n; i++) io >> a[i];
    for (register int i = 1, u, v; i < n; i++) io >> u >> v, addEdge(u, v);
    dfs1(1), dfs2(1);
    for (register int i = 1, l; i < n; i++) {
        l = lca(a[i], a[i + 1]);
        sum[a[i]]++, sum[a[i + 1]]++;
        sum[l]--, sum[fa[l]]--;
    }
    dp(1);
    for (register int i = 2; i <= n; i++) sum[a[i]]--;
    for (register int i = 1; i <= n; i++) io << sum[i] << '\n';
}

#undef long
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}
