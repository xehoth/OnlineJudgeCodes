/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「ARC 085E」MUL 06-12-2017
 * 最小割
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

struct Node {
    int v, index;
    long long f;

    Node(int v, long long f, int index) : v(v), f(f), index(index) {}
};

const int MAXN = 100 + 9;
const long long INF = LLONG_MAX / 2;

std::vector<Node> edge[MAXN];

inline void addEdge(const int u, const int v, const long long f) {
    edge[u].push_back(Node(v, f, edge[v].size()));
    edge[v].push_back(Node(u, 0, edge[u].size() - 1));
}

int h[MAXN], gap[MAXN];

inline long long sap(int v, long long flow, int s, int t, int n) {
    if (v == t) return flow;
    register long long rec = 0;
    static int iter[MAXN];
    for (register int &i = iter[v]; i < edge[v].size(); i++) {
        register Node *p = &edge[v][i];
        if (h[v] == h[p->v] + 1) {
            register long long ret =
                sap(p->v, std::min(flow - rec, p->f), s, t, n);
            p->f -= ret, edge[p->v][p->index].f += ret;
            if ((rec += ret) == flow || h[s] >= n) return iter[v] = 0, rec;
        }
    }
    if (--gap[h[v]] == 0) h[s] = n;
    gap[++h[v]]++, iter[v] = 0;
    return rec;
}

inline long long sap(int s, int t, int n) {
    register long long ret = 0;
    for (gap[0] = n; h[s] < n;) ret += sap(s, INF, s, t, n);
    return ret;
}

inline void solve() {
    register int n;
    std::cin >> n;
    const int S = 0, T = n + 1;
    register long long sum = 0;
    for (register int i = 1, a; i <= n; i++) {
        std::cin >> a;
        if (a <= 0)
            addEdge(S, i, -a);
        else
            addEdge(i, T, a), sum += a;
    }
    for (register int i = 1; i <= n; i++)
        for (register int j = i + i; j <= n; j += i) addEdge(i, j, INF);
    std::cout << sum - sap(S, T, T + 1);
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}