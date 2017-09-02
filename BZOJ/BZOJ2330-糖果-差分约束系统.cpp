/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2330」糖果 02-09-2017
 * 差分约束系统
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

struct Node {
    int v, w;

    Node(int v, int w) : v(v), w(w) {}
};

struct Graph {
    typedef std::vector<Node> Vector;

    Vector edge[MAXN + 1];

    inline void addEdge(const int u, const int v, const int w) {
        edge[u].push_back(Node(v, w));
    }

    inline Vector &operator[](const int i) { return edge[i]; }
};

struct DifferenceConstraintsSystems {
    Graph g;

#define long long long

    typedef Graph::Vector::iterator Iterator;

    int d[MAXN + 1], in[MAXN + 1];
    bool vis[MAXN + 1];

    std::deque<int> q;

    inline bool bellmanFord(const int s, const int n) {
        while (!q.empty()) {
            register int u = q.front();
            q.pop_front();
            vis[u] = false, in[u]++;
            if (in[u] == n) return false;
            for (Iterator p = g[u].begin(); p != g[u].end(); p++) {
                if (d[u] + p->w > d[p->v]) {
                    d[p->v] = d[u] + p->w;
                    if (!vis[p->v]) q.push_back(p->v), vis[p->v] = true;
                }
            }
        }
        return true;
    }

    inline void addEdge(const int x, const int a, const int b) {
        switch (x) {
            case 1:
                g.addEdge(a, b, 0), g.addEdge(b, a, 0);
                break;
            case 2:
                if (a == b) {
                    io << "-1";
                    exit(0);
                }
                g.addEdge(a, b, 1);
                break;
            case 3:
                g.addEdge(b, a, 0);
                break;
            case 4:
                if (a == b) {
                    io << "-1";
                    exit(0);
                }
                g.addEdge(b, a, 1);
                break;
            case 5:
                g.addEdge(a, b, 0);
                break;
        }
    }

    inline void solve() {
        register int n, k;
        io >> n >> k;
        for (register int i = 1, x, a, b; i <= k; i++)
            io >> x >> a >> b, addEdge(x, a, b);
        for (register int i = 1; i <= n; i++)
            vis[i] = true, q.push_back(i), d[i] = 1, in[i]++;
        if (bellmanFord(1, n)) {
            register long ans = 0;
            for (register int i = 1; i <= n; i++) ans += d[i];
            io << ans;
        } else {
            io << "-1";
        }
    }

#undef long
} task;
}

int main() {
    task.solve();
    return 0;
}
