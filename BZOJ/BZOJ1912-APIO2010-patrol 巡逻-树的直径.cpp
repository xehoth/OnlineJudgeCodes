/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1912」patrol 巡逻 08-10-2017
 * 树的直径
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

const int OUT_LEN = 100000;

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
    ~InputOutputStream() { flush(); }

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
} io;
}

namespace {

const int MAXN = 100000;

struct Node {
    int v, w, index;

    Node(int v, int w, int index) : v(v), w(w), index(index) {}
};

std::vector<Node> edge[MAXN + 1];

typedef std::vector<Node>::iterator Iterator;

inline void addEdge(const int u, const int v, int w = 1) {
    edge[u].push_back(Node(v, w, edge[v].size()));
    edge[v].push_back(Node(u, w, edge[u].size() - 1));
}

int max, dis[MAXN + 1], fa[MAXN + 1];

inline void dfs(const int u) {
    if (dis[u] > dis[max]) max = u;
    for (Iterator p = edge[u].begin(); p != edge[u].end(); p++)
        if (p->v != fa[u]) fa[p->v] = u, dis[p->v] = dis[u] + p->w, dfs(p->v);
}

Iterator path[MAXN + 1];

inline void getDiameter(const int u) {
    if (dis[u] > dis[max]) max = u;
    for (Iterator p = edge[u].begin(); p != edge[u].end(); p++) {
        if (p->v != fa[u]) {
            path[p->v] = p, fa[p->v] = u;
            dis[p->v] = dis[u] + p->w, getDiameter(p->v);
        }
    }
}

int len;

inline void dp(const int u) {
    dis[u] = 0;
    for (Iterator p = edge[u].begin(); p != edge[u].end(); p++) {
        if (p->v != fa[u]) {
            fa[p->v] = u, dp(p->v);
            len = std::max(len, dis[u] + dis[p->v] + p->w);
            dis[u] = std::max(dis[u], dis[p->v] + p->w);
        }
    }
}

using IO::io;

inline void solve() {
    register int n, k;
    io >> n >> k;
    for (register int i = 1, u, v; i < n; i++) io >> u >> v, addEdge(u, v);
    dfs(1);
    register int now = max;
    fa[now] = dis[now] = 0, getDiameter(now);
    register int ans = (n - 1) * 2 - dis[max] + 1;
    if (k == 1) {
        io << ans;
    } else {
        for (register int i = max; i != now; i = fa[i])
            path[i]->w = -1, edge[path[i]->v][path[i]->index].w = -1;
        fa[1] = 0, dp(1);
        io << ans - len + 1;
    }
}
}

int main() {
    solve();
    return 0;
}