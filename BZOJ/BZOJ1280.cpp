/*
 * created by xehoth on 27-04-2017
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

namespace MaxFlow {

const int MAXN = 1500;

const int INF = INT_MAX >> 1;

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

int gap[MAXN], h[MAXN];

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

inline int sap(int s, int t, int n) {
    register int ret = 0;
    gap[0] = n;
    while (h[s] < n) ret += sap(s, INF, s, t, n);
    return ret;
}

inline void solve() {
    using namespace IO;
    register int n, m;
    read(m), read(n);
    static int buc[MAXN];
    register int s = 0, t = n * 2 + m + 1;
    for (register int i = 1, a; i <= m; i++)
        read(a), addEdge(buc[i] = i + n, t, a);
    for (register int i = 1; i <= n; i++) {
        register int q, a;
        read(q);
        while (q--)
            read(a), addEdge(i + n + m, buc[a], INF), buc[a] = i + n + m;
        addEdge(i, i + n + m, INF);
        read(a), addEdge(s, i, a);
    }
    printf("%d", sap(s, t, t + 1));
}
}  // namespace MaxFlow

int main() {
    // freopen("in.in", "r", stdin);
    MaxFlow::solve();
    return 0;
}
