/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3306」树 01-09-2017
 * 线段树 + dfs 序
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

const int MAXN = 100000;
const int MAXM = 100000 * 4;
const int MAX_LOG = 18;

using IO::io;

template <typename T>
inline const T &min(const T &x, const T &y) {
    return x < y ? x : y;
}

struct Graph {
    typedef std::vector<int> Vector;

    Vector edge[MAXN + 1];

    inline void addEdge(const int u, const int v) {
        edge[u].push_back(v), edge[v].push_back(u);
    }

    inline Vector &operator[](const int i) { return edge[i]; }
};

struct SegmentTree {
    int d[MAXM + 1], M;

    inline void maintain(int k) { d[k] = min(d[k << 1], d[k << 1 | 1]); }

    inline void build(const int n, const int *a, const int *pos) {
        for (M = 1; M < n + 2; M <<= 1)
            ;
        for (register int i = 1; i <= n; i++) d[pos[i] + M] = a[i];
        for (register int i = M - 1; i; i--) maintain(i);
    }

    inline void modify(int x, int v) {
        for (d[x += M] = v; x;) maintain(x >>= 1);
    }

    inline int query(int s, int t) {
        register int ret = INT_MAX;
        for (s = s + M - 1, t = t + M + 1; s ^ t ^ 1; s >>= 1, t >>= 1) {
            (~s & 1) ? ret = min(ret, d[s ^ 1]) : 0;
            (t & 1) ? ret = min(ret, d[t ^ 1]) : 0;
        }
        return ret;
    }
};

struct Task {
    Graph g;

    typedef Graph::Vector::iterator Iterator;

    SegmentTree segmentTree;

    int n, q, w[MAXN + 1], root;
    int pos[MAXN + 1], idx, fa[MAXN + 1][MAX_LOG], dep[MAXN + 1], sz[MAXN + 1];
    bool vis[MAXN + 1];

    inline void dfs(const int u) {
        vis[u] = true, pos[u] = ++idx, dep[u] = dep[fa[u][0]] + 1, sz[u] = 1;
        for (register int i = 1; i < MAX_LOG; i++) {
            if (dep[u] < (1 << i)) break;
            fa[u][i] = fa[fa[u][i - 1]][i - 1];
        }
        for (Iterator v = g[u].begin(); v != g[u].end(); v++)
            if (!vis[*v]) fa[*v][0] = u, dfs(*v), sz[u] += sz[*v];
    }

    inline void query(int x) {
        if (root == x) {
            io << segmentTree.query(pos[1], pos[1] + sz[1] - 1) << '\n';
        } else if (pos[x] <= pos[root] &&
                   pos[x] + sz[x] >= pos[root] + sz[root]) {
            register int y = root, d = dep[y] - dep[x] - 1;
            for (register int i = 0; i < MAX_LOG; i++)
                if (d & (1 << i)) y = fa[y][i];
            io << min(segmentTree.query(1, pos[y] - 1),
                      segmentTree.query(pos[y] + sz[y], n))
               << '\n';
        } else {
            io << segmentTree.query(pos[x], pos[x] + sz[x] - 1) << '\n';
        }
    }

    inline void solve() {
        io >> n >> q;
        io >> w[1] >> w[1];
        for (register int i = 2, fa; i <= n; i++) {
            io >> fa >> w[i], g.addEdge(fa, i);
        }
        dfs(root = 1);
        segmentTree.build(n, w, pos);
        register char cmd;
        for (register int x, y; q--;) {
            io >> cmd;
            switch (cmd) {
                case 'V':
                    io >> x >> y, segmentTree.modify(pos[x], y);
                    break;
                case 'E':
                    io >> x, root = x;
                    break;
                case 'Q':
                    io >> x, query(x);
                    break;
            }
        }
    }
} task;
}

int main() {
    task.solve();
    return 0;
}