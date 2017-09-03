/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4205」卡牌配对 03-09-2017
 * 最大流
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

const int MAXN = 200;
const int MAX_NODE = 66500;
const int INF = INT_MAX >> 1;

struct Node {
    int v, f, index;

    Node(int v, int f, int index) : v(v), f(f), index(index) {}
};

struct Graph {
    typedef std::vector<Node> Vector;
    Vector edge[MAX_NODE + 1];

    inline void addEdge(const int u, const int v, const int f) {
        edge[u].push_back(Node(v, f, edge[v].size()));
        edge[v].push_back(Node(u, 0, edge[u].size() - 1));
    }

    inline Vector &operator[](const int i) { return edge[i]; }
};

struct ImprovedShortestAugmentPath {
    Graph g;
    typedef Graph::Vector::iterator Iterator;
    int h[MAX_NODE + 1], gap[MAX_NODE + 1];

    inline void bfs(const int t) {
        static std::queue<int> q;
        static bool vis[MAX_NODE + 1];
        q.push(t), gap[0]++, vis[t] = true;
        while (!q.empty()) {
            register int u = q.front();
            q.pop();
            for (Iterator p = g[u].begin(); p != g[u].end(); p++)
                if (!vis[p->v])
                    gap[h[p->v] = h[u] + 1]++, vis[p->v] = true, q.push(p->v);
        }
    }

    inline int sap(int v, int flow, int s, int t, int n) {
        if (v == t) return flow;
        register int rec = 0;
        static int iter[MAX_NODE + 1];
        for (register int i = iter[v]; i < g[v].size(); i++) {
            Node *p = &g[v][i];
            if (p->f > 0 && h[v] == h[p->v] + 1) {
                register int ret =
                    sap(p->v, std::min(flow - rec, p->f), s, t, n);
                p->f -= ret, g[p->v][p->index].f += ret, iter[v] = i;
                if ((rec += ret) == flow || h[s] >= n) return iter[v] = 0, rec;
            }
        }
        if (!(--gap[h[v]])) h[s] = n;
        gap[++h[v]]++, iter[v] = 0;
        return rec;
    }

    inline int sap(int s, int t, int n) {
        register int ret = 0;
        for (bfs(t); h[s] < n;) ret += sap(s, INF, s, t, n);
        return ret;
    }

    int fac[MAXN + 1][4], pos, cnt;
    int id12[MAXN + 1][MAXN + 1];
    int id13[MAXN + 1][MAXN + 1];
    int id23[MAXN + 1][MAXN + 1];
    bool vis[MAXN + 1];

    inline void init() {
        for (register int i = 2; i <= MAXN; i++) {
            if (!vis[i]) {
                cnt++;
                for (register int j = i; j <= MAXN; j += i)
                    vis[j] = true, fac[j][++fac[j][0]] = cnt;
            }
        }
    }

    inline void solve() {
        init();
        for (register int i = 1; i <= cnt; i++)
            for (register int j = 1; j <= cnt; j++)
                id12[i][j] = ++pos, id13[i][j] = ++pos, id23[i][j] = ++pos;
        register int n, m;
        io >> n >> m;
        const int S = 0, T = n + m + pos + 1;
        for (register int x, y, z; n--;) {
            g.addEdge(S, ++pos, 1);
            io >> x >> y >> z;
            for (register int i = 1; i <= fac[x][0]; i++)
                for (register int j = 1; j <= fac[y][0]; j++)
                    g.addEdge(pos, id12[fac[x][i]][fac[y][j]], 1);
            for (register int i = 1; i <= fac[x][0]; i++)
                for (register int j = 1; j <= fac[z][0]; j++)
                    g.addEdge(pos, id13[fac[x][i]][fac[z][j]], 1);
            for (register int i = 1; i <= fac[y][0]; i++)
                for (register int j = 1; j <= fac[z][0]; j++)
                    g.addEdge(pos, id23[fac[y][i]][fac[z][j]], 1);
        }
        for (register int x, y, z; m--;) {
            g.addEdge(++pos, T, 1);
            io >> x >> y >> z;
            for (register int i = 1; i <= fac[x][0]; i++)
                for (register int j = 1; j <= fac[y][0]; j++)
                    g.addEdge(id12[fac[x][i]][fac[y][j]], pos, 1);
            for (register int i = 1; i <= fac[x][0]; i++)
                for (register int j = 1; j <= fac[z][0]; j++)
                    g.addEdge(id13[fac[x][i]][fac[z][j]], pos, 1);
            for (register int i = 1; i <= fac[y][0]; i++)
                for (register int j = 1; j <= fac[z][0]; j++)
                    g.addEdge(id23[fac[y][i]][fac[z][j]], pos, 1);
        }
        io << sap(S, T, T + 1);
    }
} task;
}

int main() {
    task.solve();
    return 0;
}