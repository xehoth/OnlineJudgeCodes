/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「洛谷 1137」旅行计划 02-09-2017
 * 拓扑排序 + dp
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

const int MAXN = 100000;

struct Graph {
    typedef std::vector<int> Vector;

    Vector edge[MAXN + 1];

    inline void addEdge(const int u, const int v) { edge[u].push_back(v); }

    inline Vector &operator[](const int i) { return edge[i]; }
};

struct Task {
    Graph g;

    typedef Graph::Vector::iterator Iterator;

    int f[MAXN + 1], in[MAXN + 1];

    std::vector<int> topo;

    inline void topoSort(const int n) {
        static std::vector<int> st;
        st.reserve(n + 1), topo.reserve(n + 1);
        for (register int i = 1; i <= n; i++)
            if (in[i] == 0) st.push_back(i), f[i] = 1;
        while (!st.empty()) {
            register int u = st.back();
            st.pop_back(), topo.push_back(u);
            for (Iterator v = g[u].begin(); v != g[u].end(); v++)
                if (--in[*v] == 0) st.push_back(*v);
        }
    }

    inline void dp() {
        for (Iterator u = topo.begin(); u != topo.end(); u++)
            for (Iterator v = g[*u].begin(); v != g[*u].end(); v++)
                f[*v] = std::max(f[*v], f[*u] + 1);
    }

    inline void solve() {
        register int n, m;
        io >> n >> m;
        for (register int i = 1, u, v; i <= m; i++)
            io >> u >> v, g.addEdge(u, v), in[v]++;
        topoSort(n), dp();
        for (register int i = 1; i <= n; i++) io << f[i] << '\n';
    }
} task;
}

int main() {
    task.solve();
    return 0;
}