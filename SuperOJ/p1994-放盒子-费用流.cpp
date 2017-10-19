/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 1994」放盒子 19-10-2017
 * 费用流
 * @author xehoth
 */
#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>

using namespace __gnu_pbds;

namespace {

inline bool relax(int &x, int v) { return v < x ? (x = v, true) : false; }

const int MAXN = 410;
const int INF = 0x3f3f3f3f;

struct Node {
    int v, f, w, index;

    Node(int v, int f, int w, int index) : v(v), f(f), w(w), index(index) {}
};

struct Graph {
    typedef std::vector<Node> Vector;
    Vector edge[MAXN + 1];

    inline void addEdge(const int u, const int v, const int f, const int w) {
        edge[u].push_back(Node(v, f, w, edge[v].size()));
        edge[v].push_back(Node(u, 0, -w, edge[u].size() - 1));
    }

    inline Vector &operator[](const int i) { return edge[i]; }
};

struct PrimalDual : Graph {
    int h[MAXN + 1], d[MAXN + 1];
    bool vis[MAXN + 1];

    typedef Vector::iterator Iterator;

    typedef std::pair<int, int> Pair;
    typedef __gnu_pbds::priority_queue<Pair, std::greater<Pair> > PriorityQueue;

    inline void bellmanFord(const int s, const int n) {
        memset(h, 0x3f, sizeof(int) * (n + 1));
        static std::queue<int> q;
        q.push(s), h[s] = 0;
        for (register int u; !q.empty();) {
            vis[u = q.front()] = false, q.pop();
            for (Iterator p = edge[u].begin(); p != edge[u].end(); p++)
                if (p->f > 0 && relax(h[p->v], h[u] + p->w) && !vis[p->v])
                    q.push(p->v), vis[p->v] = true;
        }
    }

    inline void dijkstra(const int s, const int n) {
        memset(vis, 0, sizeof(bool) * (n + 1));
        static PriorityQueue::point_iterator id[MAXN + 1];
        static PriorityQueue q;
        memset(id, 0, sizeof(PriorityQueue::point_iterator) * (n + 1));
        memset(d, 0x3f, sizeof(int) * (n + 1));
        id[s] = q.push(Pair(d[s] = 0, s));
        for (register int u; !q.empty();) {
            register Pair now = q.top();
            q.pop(), u = now.second;
            if (vis[u] || d[u] < now.first) continue;
            vis[u] = true;
            for (Iterator p = edge[u].begin(); p != edge[u].end(); p++) {
                if (p->f > 0 && relax(d[p->v], d[u] + p->w + h[u] - h[p->v])) {
                    if (id[p->v] != NULL)
                        q.modify(id[p->v], Pair(d[p->v], p->v));
                    else
                        id[p->v] = q.push(Pair(d[p->v], p->v));
                }
            }
        }
    }

    int iter[MAXN + 1];

    int dfs(int v, int flow, int s, int t, int &cost) {
        if (v == t) return cost += h[t] * flow, flow;
        vis[v] = true;
        register int rec = 0;
        for (register int i = iter[v]; i < edge[v].size(); i++) {
            Node *p = &edge[v][i];
            if (!vis[p->v] && p->f > 0 && h[v] == h[p->v] - p->w) {
                register int ret =
                    dfs(p->v, std::min(flow - rec, p->f), s, t, cost);
                p->f -= ret, edge[p->v][p->index].f += ret, iter[v] = i;
                if ((rec += ret) == flow) return rec;
            }
        }
        return rec;
    }

    inline void primalDual(int s, int t, int n, int &flow, int &cost,
                           int f = INF) {
        for (bellmanFord(s, n), cost = 0, flow = 0; f > 0;) {
            dijkstra(s, n);
            if (d[t] == INF) break;
            for (register int i = 0; i <= n; i++)
                h[i] = std::min(INF, h[i] + d[i]);
            memset(iter, 0, sizeof(int) * (n + 1));
            memset(vis, 0, sizeof(bool) * (n + 1));
            flow += dfs(s, INF, s, t, cost);
        }
    }
} g;

struct Data {
    int l, w, s;

    inline bool operator<(const Data &p) const {
        return l < p.l || (l == p.l && w < p.w);
    }

    inline bool operator==(const Data &p) const { return l == p.l && w == p.w; }
} d[MAXN + 1];

inline void solve() {
    register int n, ans = 0, flow = 0, cost = 0;
    std::cin >> n;
    for (register int i = 1; i <= n; i++)
        std::cin >> d[i].l >> d[i].w, d[i].s = d[i].l * d[i].w;
    std::sort(d + 1, d + n + 1);
    register int cnt = std::unique(d + 1, d + n + 1) - d - 1;
    n = cnt;
    register const int S = 0, T = n << 1 | 1;
    for (register int i = 1; i <= n; i++) {
        g.addEdge(S, i, 1, 0), g.addEdge(i + n, T, 1, 0);
        for (register int j = i + 1; j <= n; j++)
            if (d[i].l <= d[j].l && d[i].w <= d[j].w)
                g.addEdge(j, i + n, 1, -d[i].s);
        ans += d[i].s;
    }
    g.primalDual(S, T, T + 1, flow, cost);
    std::cout << ans + cost;
}
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    solve();
    return 0;
}