#include <bits/stdc++.h>

const int MAXN = 101000, INF = ~0U >> 2;
const int MAXM = 500100;

int n, m, s, t, h[MAXN], gap[MAXN];

struct Node {
    int v, f, index;
    Node(int v, int f, int index) : v(v), f(f), index(index) {}
};

std::vector<Node> edge[MAXN];

inline void addEdge(int u, int v, int f) {
    edge[u].push_back(Node(v, f, edge[v].size()));
    edge[v].push_back(Node(u, 0, edge[u].size() - 1));
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
            if ((rec += ret) == flow || h[s] >= n) return rec;
        }
    }
    iter[v] = 0;
    if (!(--gap[h[v]])) h[s] = n;
    gap[++h[v]]++;
    return rec;
}

inline void bfs(int t) {
    std::queue<int> q;
    static bool vis[MAXN];
    q.push(t), vis[t] = true, gap[0]++;
    while (!q.empty()) {
        register int u = q.front();
        q.pop();
        for (register int i = 0, v; i < edge[u].size(); i++) {
            if (vis[v = edge[u][i].v]) continue;
            gap[h[v] = h[u] + 1]++, vis[v] = true, q.push(v);
        }
    }
}

inline int sap(int s, int t, int n) {
    register int ret = 0;
    bfs(t);
    while (h[s] < n) ret += sap(s, INT_MAX, s, t, n);
    return ret;
}

const int IO_LEN = 1048576;

char ioc;
bool iosig;
inline char read() {
    static char buf1[IO_LEN], buf2[IO_LEN + 1], *s1, *t;
    if (s1 == t) {
        t = (s1 = buf1) + fread(buf1, 1, IO_LEN, stdin);
        if (s1 == t) return -1;
    }
    return *s1++;
}

inline bool read(int &x) {
    iosig = false;
    for (ioc = read(); !isdigit(ioc); ioc = read()) {
        if (ioc == -1) return false;
        if (ioc == '-') iosig = true;
    }
    x = 0;
    while (ioc == '0') ioc = read();
    for (;; ioc = read()) {
        if (!isdigit(ioc)) break;
        x = (x << 1) + (x << 3) + (ioc ^ '0');
    }
    if (iosig) x = -x;
    return true;
}

int tot, ans;
int main() {
#ifndef ONLINE_JUDGE
// freopen("snow.in", "r", stdin);
#endif
    read(n), read(m);
    s = tot = ans = 0, t = n << 1 | 1;
    for (register int i = 1, w; i <= n; i++)
        read(w), ans += w, addEdge(s, i, w), addEdge(i + n, t, w);
    for (register int i = 0, u, v, w; i < m; i++)
        read(u), read(v), read(w), addEdge(u, v + n, w);
    n = t - s + 1;
    std::cout << ans - sap(s, t, t + 1);
    return 0;
}