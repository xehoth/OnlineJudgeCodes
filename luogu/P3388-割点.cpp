/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「洛谷 3388」割点 02-09-2017
 * tarjan
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

const int MAXN = 100010;

struct Graph {
    typedef std::vector<int> Vector;

    Vector edge[MAXN + 1];

    inline void addEdge(const int u, const int v) {
        edge[u].push_back(v), edge[v].push_back(u);
    }

    inline Vector &operator[](const int i) { return edge[i]; }
};

struct Tarjan {
    Graph g;

    typedef Graph::Vector::iterator Iterator;

    int dfn[MAXN + 1], low[MAXN + 1], idx, fa[MAXN + 1];

    inline void tarjan(const int u) {
        dfn[u] = low[u] = ++idx;
        for (Iterator v = g[u].begin(); v != g[u].end(); v++) {
            if (!dfn[*v])
                fa[*v] = u, tarjan(*v), low[u] = std::min(low[u], low[*v]);
            else
                low[u] = std::min(low[u], dfn[*v]);
        }
    }

    int son[MAXN + 1], cut[MAXN + 1];

    inline void findCut(const int min, const int max) {
        for (register int i = min, v; i <= max; i++)
            if (low[i] >= dfn[v = fa[i]]) fa[v] == 0 ? son[v]++ : cut[v] = 1;
        for (register int i = min; i <= max; i++)
            if (son[i] == 1) cut[i] = 0;
        register int ans = 0;
        for (register int i = min; i <= max; i++)
            if (cut[i]) ans++;
        io << ans << '\n';
        for (register int i = min; i <= max; i++)
            if (cut[i]) io << i << ' ';
    }

    inline void solve() {
        register int n, m, max = 0, min = INT_MAX;
        io >> n >> m;
        for (register int i = 1, u, v; i <= m; i++) {
            io >> u >> v, g.addEdge(u, v);
            max = std::max(max, std::max(u, v));
            min = std::min(min, std::min(u, v));
        }
        for (register int i = min; i <= max; i++)
            if (!dfn[i]) tarjan(i);
        findCut(min, max);
    }
} task;
}

int main() {
    task.solve();
    return 0;
}