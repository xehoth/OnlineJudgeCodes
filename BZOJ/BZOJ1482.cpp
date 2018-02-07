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

const int MAXN = 1010;

#define long long long

const int INF = 1000000000;

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
    memset(gap, 0, sizeof(int) * (n + 1));
    memset(h, 0, sizeof(int) * (n + 1));
    gap[0] = n;
    while (h[s] < n) ret += sap(s, INF, s, t, n);
    return ret;
}

long d[MAXN][MAXN];
int n, m, s, t, sum;
int a[MAXN], b[MAXN];

inline bool check(long x) {
    s = 0, t = 2 * n + 1;
    //记得从 0 开始清空
    for (register int i = s; i <= t; i++) edge[i].clear();
    for (register int i = 1; i <= n; i++)
        addEdge(s, i, a[i]), addEdge(n + i, t, b[i]);
    for (register int i = 1; i <= n; i++)
        for (register int j = 1; j <= n; j++)
            if (d[i][j] <= x) addEdge(i, j + n, INF);
    return sap(s, t, t + 1) == sum;
}

inline void solve() {
    using namespace IO;
    read(n), read(m);
    for (register int i = 1; i <= n; i++) read(a[i]), read(b[i]), sum += a[i];
    for (register int i = 1; i <= n; i++)
        for (register int j = 1; j <= n; j++) d[i][j] = INF * 200ll;
    for (register int i = 1, u, v; i <= m; i++) {
        register long w;
        read(u), read(v), read(w);
        d[u][v] = d[v][u] = std::min(d[u][v], w);
    }
    for (register int i = 1; i <= n; i++) d[i][i] = 0;
    for (register int k = 1; k <= n; k++)
        for (register int i = 1; i <= n; i++)
            for (register int j = 1; j <= n; j++)
                d[i][j] = std::min(d[i][j], d[i][k] + d[k][j]);
    long l = 0, r = INF * 200ll - 1, ans = -1;
    while (l <= r) {
        register long mid = l + r >> 1;
        if (check(mid))
            r = mid - 1, ans = mid;
        else
            l = mid + 1;
    }
    std::cout << ans;
}
}  // namespace MaxFlow

int main() {
    // freopen("in.in", "r", stdin);
    exit(0);
    MaxFlow::solve();
    return 0;
}
