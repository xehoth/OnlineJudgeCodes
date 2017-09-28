/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1060」时态同步 28-09-2017
 * 树形 DP
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

const int MAXN = 500000;

struct Node {
    int v, w;

    Node(int v, int w) : v(v), w(w) {}
};

typedef std::vector<Node>::iterator Iterator;

std::vector<Node> edge[MAXN + 1];

inline void addEdge(const int u, const int v, const int w) {
    edge[u].push_back(Node(v, w));
    edge[v].push_back(Node(u, w));
}

bool vis[MAXN + 1];
int f[MAXN + 1];
long ans;

inline void dfs(const int u) {
    vis[u] = true;
    for (Iterator p = edge[u].begin(); p != edge[u].end(); p++)
        if (!vis[p->v]) dfs(p->v), f[u] = std::max(f[u], f[p->v] + p->w);
}

inline void dp(const int u) {
    vis[u] = false;
    for (Iterator p = edge[u].begin(); p != edge[u].end(); p++)
        if (vis[p->v]) dp(p->v), ans += f[u] - f[p->v] - p->w;
}

inline void solve() {
    register int n, root;
    io >> n >> root;
    for (register int i = 1, u, v, w; i < n; i++)
        io >> u >> v >> w, addEdge(u, v, w);
    dfs(root), dp(root);
    io << ans;
}

#undef long
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}
