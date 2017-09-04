/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3876」支线剧情 04-09-2017
 * 上下界费用流 - 最小费用可行流
 * @author xehoth
 */
#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>

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

const int MAXN = 300;
const int MAX_NODE = 300 + 2;
const int INF = 0x3f3f3f3f;

struct Node {
    int v, f, w, index;

    Node(int v, int f, int w, int index) : v(v), f(f), w(w), index(index) {}
};

struct Graph {
    typedef std::vector<Node> Vector;
    Vector edge[MAX_NODE + 1];

    inline void addEdge(const int u, const int v, const int f, const int w) {
        edge[u].push_back(Node(v, f, w, edge[v].size()));
        edge[v].push_back(Node(u, 0, -w, edge[u].size() - 1));
    }

    inline Vector &operator[](const int i) { return edge[i]; }
};

struct PrimalDual {
    Graph g;

    int h[MAX_NODE + 1], d[MAX_NODE + 1];
    bool vis[MAX_NODE + 1];
    int prev[MAX_NODE + 1], pree[MAX_NODE + 1];

    typedef Graph::Vector::iterator Iterator;

    inline void bellmanFord(const int s, const int n) {
        memset(h, 0x3f, sizeof(int) * (n + 1));
        static std::queue<int> q;
        q.push(s), h[s] = 0;
        while (!q.empty()) {
            register int u = q.front();
            q.pop(), vis[u] = false;
            for (Iterator p = g[u].begin(); p != g[u].end(); p++) {
                if (p->f > 0 && h[u] + p->w < h[p->v]) {
                    h[p->v] = h[u] + p->w;
                    if (!vis[p->v]) q.push(p->v), vis[p->v] = true;
                }
            }
        }
    }

    typedef std::pair<int, int> Pair;
    typedef __gnu_pbds::priority_queue<Pair, std::greater<Pair> > PriorityQueue;

    inline void dijkstra(const int s, const int n) {
        static PriorityQueue::point_iterator id[MAX_NODE + 1];
        static PriorityQueue q;
        memset(vis, 0, sizeof(bool) * (n + 1));
        memset(id, 0, sizeof(PriorityQueue::point_iterator) * (n + 1));
        memset(d, 0x3f, sizeof(int) * (n + 1));
        id[s] = q.push(Pair(d[s] = 0, s));
        while (!q.empty()) {
            register Pair now = q.top();
            q.pop();
            register int u = now.second;
            if (vis[u] || d[u] < now.first) continue;
            vis[u] = true;
            for (register int i = 0; i < g[u].size(); i++) {
                Node *p = &g[u][i];
                register int w = d[u] + p->w + h[u] - h[p->v];
                if (p->f > 0 && w < d[p->v]) {
                    d[p->v] = w, prev[p->v] = u, pree[p->v] = i;
                    if (id[p->v] != NULL)
                        q.modify(id[p->v], Pair(d[p->v], p->v));
                    else
                        id[p->v] = q.push(Pair(d[p->v], p->v));
                }
            }
        }
    }

    inline Pair primalDual(const int s, const int t, const int n, int f = INF) {
        Pair ans(0, 0);
        for (bellmanFord(s, n); f > 0;) {
            dijkstra(s, n);
            if (d[t] == INF) break;
            for (register int i = 0; i <= n; i++)
                h[i] = std::min(INF, h[i] + d[i]);
            register int flow = f;
            for (register int i = t; i != s; i = prev[i])
                flow = std::min(flow, g[prev[i]][pree[i]].f);
            f -= flow, ans.first += flow, ans.second += flow * h[t];
            for (register int i = t; i != s; i = prev[i]) {
                Node *p = &g[prev[i]][pree[i]];
                p->f -= flow, g[p->v][p->index].f += flow;
            }
        }
        return ans;
    }

    int extra[MAX_NODE + 1], ans;

    inline void addEdge(int u, int v, int lower, int upper, int w) {
        extra[v] += lower, extra[u] -= lower;
        g.addEdge(u, v, upper - lower, w);
    }

    inline void solve() {
        register int n, ans = 0;
        io >> n;
        const int S = 0, T = n + 1;
        for (register int i = 2; i <= n; i++) g.addEdge(i, 1, INF, 0);
        for (register int i = 1, m; i <= n; i++) {
            io >> m;
            for (register int j = 1, b, t; j <= m; j++) {
                io >> b >> t;
                addEdge(i, b, 1, INF, t), ans += t;
            }
        }
        for (register int i = 1; i <= n; i++) {
            if (extra[i] > 0) g.addEdge(S, i, extra[i], 0);
            if (extra[i] < 0) g.addEdge(i, T, -extra[i], 0);
        }
        io << ans + primalDual(S, T, T + 1).second;
    }
} task;
}

int main() {
    task.solve();
    return 0;
}