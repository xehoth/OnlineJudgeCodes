/*
 * created by xehoth on 28-04-2017
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        if (x < 0) print('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace IO

namespace Maxflow {

const int MAXN = 310;

struct Node {
    int v, f, index;

    Node(const int v, const int f, const int index)
        : v(v), f(f), index(index) {}
};

std::vector<Node> edge[MAXN];

int h[MAXN], gap[MAXN];

const int INF = INT_MAX >> 1;

inline void addEdge(const int u, const int v, const int f) {
    edge[u].push_back(Node(v, f, edge[v].size()));
    edge[v].push_back(Node(u, 0, edge[u].size() - 1));
}

inline int sap(int v, int flow, int s, int t, int n) {
    if (v == t) return flow;
    register int rec = 0;
    static int iter[MAXN];
    for (register int i = iter[v]; i < edge[v].size(); i++) {
        Node *p = &edge[v][i];
        if (p->f > 0 && h[v] == h[p->v] + 1) {
            register int ret = sap(p->v, std::min(flow - rec, p->f), s, t, n);
            p->f -= ret, edge[p->v][p->index].f += ret, iter[v] = i;
            if ((rec += ret) == flow || h[s] >= n) return rec;
        }
    }
    if (!(--gap[h[v]])) h[s] = n;
    gap[++h[v]]++, iter[v] = 0;
    return rec;
}

inline int sap(int s, int t, int n) {
    register int ret = 0;
    gap[0] = n;
    while (h[s] < n) ret += sap(s, INF, s, t, n);
    return ret;
}

bool vis[MAXN];

int n;

inline void dfs(int v) {
    IO::print(v = v > n ? v - n : v), IO::print(' ');
    vis[v] = true;
    for (register int i = 0; i < edge[v].size(); i++) {
        Node *p = &edge[v][i];
        if (!p->f && p->v != 0 && !vis[p->v]) {
            dfs(p->v);
            break;
        }
    }
}

inline void solve() {
    using namespace IO;
    register int m;
    read(n), read(m);
    register int s = 0, t = n << 1 | 1;
    for (register int i = 1; i <= n; i++)
        addEdge(s, i, 1), addEdge(i + n, t, 1);
    for (register int i = 0, u, v; i < m; i++)
        read(u), read(v), addEdge(u, v + n, 1);
    register int ans = sap(s, t, t + 1);
    for (register int i = 1; i <= n; i++) {
        if (!vis[i]) {
            dfs(i), print('\n');
        }
    }
    print(n - ans), print('\n');
}
}  // namespace Maxflow

int main() {
    Maxflow::solve();
    IO::flush();
    return 0;
}