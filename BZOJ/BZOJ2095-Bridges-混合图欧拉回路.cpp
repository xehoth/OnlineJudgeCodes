/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2095」 02-09-2017
 * 二分 + 最大流
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

const int MAXN = 10100;
const int MAXM = 20000;
const int INF = INT_MAX >> 1;

struct Node {
    int v, f, index;

    Node(int v, int f, int index) : v(v), f(f), index(index) {}
};

struct Graph {
    typedef std::vector<Node> Vector;

    Vector edge[MAXN + 1];

    inline void addEdge(const int u, const int v, const int f) {
        edge[u].push_back(Node(v, f, edge[v].size()));
        edge[v].push_back(Node(u, 0, edge[u].size() - 1));
    }

    inline void clear(const int n) {
        for (register int i = 0; i <= n; i++) edge[i].clear();
    }

    inline Vector &operator[](const int i) { return edge[i]; }
};

struct ImprovedShortestAugmentPath {
    Graph g;

    int h[MAXN + 1], gap[MAXN + 1];

    inline int sap(int v, int flow, int s, int t, int n) {
        if (v == t) return flow;
        register int rec = 0;
        static int iter[MAXN + 1];
        for (register int i = iter[v]; i < g[v].size(); i++) {
            Node *p = &g[v][i];
            if (p->f > 0 && h[v] == h[p->v] + 1) {
                register int ret =
                    sap(p->v, std::min(p->f, flow - rec), s, t, n);
                p->f -= ret, g[p->v][p->index].f += ret, iter[v] = i;
                if ((rec += ret) == flow || h[v] >= n) return iter[v] = 0, rec;
            }
        }
        if (!(--gap[h[v]])) h[s] = n;
        gap[++h[v]]++, iter[v] = 0;
        return rec;
    }

    inline int sap(int s, int t, int n) {
        register int ret = 0;
        memset(gap, 0, sizeof(int) * (n + 1));
        memset(h, 0, sizeof(int) * (n + 1));
        for (gap[0] = n; h[s] < n;) ret += sap(s, INF, s, t, n);
        return ret;
    }

    int n, m, s, t;

    int u[MAXM + 1], v[MAXM + 1], c[MAXM + 1], d[MAXM + 1];
    int in[MAXN + 1], out[MAXN + 1];

    inline bool check(int mid) {
        register int sum = 0;
        g.clear(n + 1);
        memset(in, 0, sizeof(int) * (n + 1));
        memset(out, 0, sizeof(int) * (n + 1));
        for (register int i = 1; i <= m; i++) {
            if (c[i] <= mid) out[u[i]]++, in[v[i]]++;
            if (d[i] <= mid) g.addEdge(v[i], u[i], 1);
        }
        for (register int i = 1; i <= n; i++)
            if (std::abs(in[i] - out[i]) & 1) return false;
        for (register int i = 1, x; i <= n; i++) {
            x = (in[i] - out[i]) / 2;
            if (x > 0) g.addEdge(s, i, x), sum += x;
            if (x < 0) g.addEdge(i, t, -x);
        }
        register int tmp = sap(s, t, t + 1);
        return tmp == sum;
    }

    inline void solve() {
        io >> n >> m;
        register int l = INT_MAX, r = INT_MIN;
        s = 0, t = n + 1;
        for (register int i = 1; i <= m; i++) {
            io >> u[i] >> v[i] >> c[i] >> d[i];
            if (c[i] > d[i]) std::swap(c[i], d[i]), std::swap(u[i], v[i]);
            l = std::min(l, c[i]), r = std::max(r, d[i]);
        }
        l--, r++;
        while (r - l > 1) {
            register int mid = l + r >> 1;
            if (check(mid))
                r = mid;
            else
                l = mid;
        }
        if (!check(r))
            io << "NIE";
        else
            io << r;
    }
} task;
}

int main() {
    task.solve();
    return 0;
}