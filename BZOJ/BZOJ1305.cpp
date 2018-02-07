/*
 * created by xehoth on 19-02-2017
 */
#include <bits/stdc++.h>

const int MAXN = 495;

struct Node {
    int v, f, index;
    Node(int v, int f, int index) : v(v), f(f), index(index) {}
};

std::vector<Node> edge[MAXN];
std::vector<Node> tmp[MAXN];

inline void addEdge(int u, int v, int f) {
    edge[u].push_back(Node(v, f, edge[v].size()));
    edge[v].push_back(Node(u, 0, edge[u].size() - 1));
    tmp[u].push_back(Node(v, f, tmp[v].size()));
    tmp[v].push_back(Node(u, 0, tmp[u].size() - 1));
}

int gap[MAXN], h[MAXN];

inline int sap(int v, int flow, int s, int t, int n) {
    if (v == t) return flow;
    register int rec = 0;
    static int iter[MAXN];
    for (register int i = iter[v]; i < edge[v].size(); i++) {
        Node *p = &edge[v][i];
        if (h[v] == h[p->v] + 1) {
            register int ret = sap(p->v, std::min(flow - rec, p->f), s, t, n);
            p->f -= ret, edge[p->v][p->index].f += ret, iter[v] = i;
            if ((rec += ret) == flow) return flow;
        }
    }
    iter[v] = 0;
    if (!(--gap[h[v]])) h[s] = n;
    gap[++h[v]]++;
    return rec;
}

inline int sap(int s, int t, int n) {
    register int ret = 0;
    memset(gap, 0, sizeof(int) * (n + 1));
    memset(h, 0, sizeof(int) * (n + 1));
    gap[0] = n;
    while (h[s] < n) ret += sap(s, INT_MAX, s, t, n);
    return ret;
}

int S, T;

inline void build(int n, int k) {
    S = 0, T = n << 2 | 1;
    for (register int i = 1; i <= n; i++) addEdge(i, i + n, k);
    for (register int i = n << 1 | 1, range = 3 * n; i <= range; i++)
        addEdge(i, i + n, k);
    for (register int i = 1; i <= n; i++) {
        static char str[MAXN];
        scanf("%s", str + 1);
        for (register int j = 1; j <= n; j++) {
            if (str[j] == 'Y')
                addEdge(i, 3 * n + j, 1);
            else
                addEdge(n + i, 2 * n + j, 1);
        }
    }
    for (register int i = 1; i <= n; i++) addEdge(S, i, 0);
    for (register int i = 3 * n + 1; i <= 4 * n; i++) addEdge(i, T, 0);
}

inline void solve(int n) {
    int l = 0, r = n;
    while (l + 1 < r) {
        for (register int i = S; i <= T; i++)
            std::copy(tmp[i].begin(), tmp[i].end(), edge[i].begin());
        register int mid = l + r >> 1;
        for (register int i = 0; i < n; i++) edge[S][i].f = mid;
        for (register int i = 3 * n + 1; i <= 4 * n; i++)
            edge[i][edge[i].size() - 1].f = mid;
        register int d = sap(S, T, T + 1);
        if (d == mid * n)
            l = mid;
        else
            r = mid;
    }
    for (register int i = S; i <= T; i++)
        std::copy(tmp[i].begin(), tmp[i].end(), edge[i].begin());
    for (register int i = 0; i < n; i++) edge[S][i].f = r;
    for (register int i = 3 * n + 1; i <= 4 * n; i++)
        edge[i][edge[i].size() - 1].f = r;
    register int d = sap(S, T, T + 1);
    printf("%d", (d == r * n) ? r : l);
}

int main() {
    register int n, k;
    scanf("%d %d", &n, &k);
    build(n, k);
    solve(n);
    return 0;
}