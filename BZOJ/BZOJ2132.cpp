/*
 * created by xehoth on 05-05-2017
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

const int MAXM = 105;
const int MAXN = 105 * 105 + 5;

int gap[MAXN], h[MAXN];

struct Node {
    int v, f, index;
    Node(int v, int f, int index) : v(v), f(f), index(index) {}
};

std::vector<Node> edge[MAXN];

inline void addEdge(int u, int v, int f, int rf = 0) {
    edge[u].push_back(Node(v, f, edge[v].size()));
    edge[v].push_back(Node(u, rf, edge[u].size() - 1));
}

inline int sap(int v, int flow, int s, int t, int n) {
    static int iter[MAXN];
    if (v == t) return flow;
    register int rec = 0;
    for (register int i = iter[v]; i < edge[v].size(); i++) {
        Node *p = &edge[v][i];
        if (h[v] == h[p->v] + 1) {
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
    while (h[s] < n) ret += sap(s, INT_MAX, s, t, n);
    return ret;
}

int n, m;

inline int id(const int i, const int j) { return (i - 1) * m + j; }

inline void solve() {
    using namespace IO;
    read(n), read(m);

    register int sum = 0;
    const int s = 0, t = n * m + 1;
    for (register int i = 1, x; i <= n; i++) {
        for (register int j = 1; j <= m; j++) {
            read(x), sum += x;
            if ((i + j & 1) == 0)
                addEdge(s, id(i, j), x);
            else
                addEdge(id(i, j), t, x);
        }
    }

    for (register int i = 1, x; i <= n; i++) {
        for (register int j = 1; j <= m; j++) {
            read(x), sum += x;
            if ((i + j & 1) != 0)
                addEdge(s, id(i, j), x);
            else
                addEdge(id(i, j), t, x);
        }
    }

    for (register int i = 1, x; i <= n; i++) {
        for (register int j = 1; j <= m; j++) {
            read(x);
            if (i != 1) addEdge(id(i - 1, j), id(i, j), x, x), sum += x;
            if (i != n) addEdge(id(i + 1, j), id(i, j), x, x), sum += x;
            if (j != 1) addEdge(id(i, j - 1), id(i, j), x, x), sum += x;
            if (j != m) addEdge(id(i, j + 1), id(i, j), x, x), sum += x;
        }
    }

    std::cout << sum - sap(s, t, t + 1);
}
}  // namespace Maxflow

int main() {
    // freopen("in.in", "r", stdin);
    Maxflow::solve();
    return 0;
}
