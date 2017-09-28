/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1131」Sta 28-09-2017
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

const int MAXN = 1000000;

std::vector<int> edge[MAXN + 1];
typedef std::vector<int>::iterator Iterator;

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

int n;
long f[MAXN + 1];
int sz[MAXN + 1], dep[MAXN + 1];
bool vis[MAXN + 1];

inline void dfs(const int u) {
    vis[u] = true, sz[u] = 1, f[u] = dep[u];
    for (Iterator v = edge[u].begin(); v != edge[u].end(); v++)
        if (!vis[*v])
            dep[*v] = dep[u] + 1, dfs(*v), sz[u] += sz[*v], f[u] += f[*v];
}

inline void dp(const int u) {
    vis[u] = false;
    for (Iterator v = edge[u].begin(); v != edge[u].end(); v++)
        if (vis[*v]) f[*v] = f[u] + n - 2 * sz[*v], dp(*v);
}

inline void solve() {
    io >> n;
    for (register int i = 1, u, v; i < n; i++) io >> u >> v, addEdge(u, v);
    dfs(1), dp(1);
    io << std::max_element(f + 1, f + n + 1) - f;
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}
