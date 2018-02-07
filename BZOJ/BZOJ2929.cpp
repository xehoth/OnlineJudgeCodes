/*
 * created by xehoth
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
}  // namespace IO

namespace Maxflow {

const int MAXN = 210;

struct Node {
    int v, f, index;

    Node(const int v, const int f, const int index)
        : v(v), f(f), index(index) {}
};

std::vector<Node> edge[MAXN];

inline void addEdge(const int u, const int v, const int f) {
    edge[u].push_back(Node(v, f, edge[v].size()));
    edge[v].push_back(Node(u, 0, edge[u].size() - 1));
}

int h[MAXN], gap[MAXN];

inline int sap(int v, int flow, int s, int t, int n) {
    if (v == t) return flow;
    static int iter[MAXN];
    register int rec = 0;
    for (register int i = iter[v]; i < edge[v].size(); i++) {
        Node *p = &edge[v][i];
        if (p->f > 0 && h[v] == h[p->v] + 1) {
            register int ret = sap(p->v, std::min(flow - rec, p->f), s, t, n);
            p->f -= ret, edge[p->v][p->index].f += ret, iter[v] = i;
            if ((rec += ret) == flow) return flow;
        }
    }
    if (!(--gap[h[v]])) h[s] = n;
    gap[++h[v]]++, iter[v] = 0;
    return rec;
}

const int INF = INT_MAX >> 1;

inline int sap(int s, int t, int n) {
    register int ret = 0;
    gap[0] = n;
    while (h[s] < n) ret += sap(s, INF, s, t, n);
    return ret;
}

inline void solve() {
    using namespace IO;
    register int n;
    read(n);
    for (register int i = 1, u, v; i < n; i++) {
        read(u);
        while (u--)
            read(v), i == 1 || v == n ? addEdge(i, v, 1) : addEdge(i, v, INF);
    }
    std::cout << sap(1, n, n);
}
}  // namespace Maxflow

int main() {
    // freopen("in.in", "r", stdin);
    Maxflow::solve();
    return 0;
}
