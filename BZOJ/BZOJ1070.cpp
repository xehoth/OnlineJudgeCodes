/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1070」03-05-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>

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

namespace PrimalDual {

const int MAXN = 60 + 60 * 9 + 10;

struct Node {
    int v, f, w, index;

    Node(int v, int f, int w, int index) : v(v), f(f), w(w), index(index) {}
};

std::vector<Node> edge[MAXN];

inline void addEdge(const int u, const int v, const int f, const int w) {
    edge[u].push_back(Node(v, f, w, edge[v].size()));
    edge[v].push_back(Node(u, 0, -w, edge[u].size() - 1));
}

typedef std::pair<int, int> Pair;

// typedef std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair> >
// PriorityQueue;
typedef __gnu_pbds::priority_queue<Pair, std::greater<Pair> > PriorityQueue;
int pree[MAXN], prev[MAXN];
PriorityQueue::point_iterator id[MAXN];

const int INF = 0x3f3f3f3f;

inline Pair minCostFlow(const int s, const int t, const int n, int f) {
    Pair ans(0, 0);
    static int h[MAXN], d[MAXN];
    while (f > 0) {
        static bool vis[MAXN];
        memset(vis, 0, n + 1);
        memset(d, 0x3f, sizeof(int) * (n + 1));
        memset(id, 0, sizeof(PriorityQueue::point_iterator) * (n + 1));
        static PriorityQueue q;
        d[s] = 0, id[s] = q.push(Pair(0, s));
        while (!q.empty()) {
            Pair now = q.top();
            q.pop();
            register int v = now.second;
            if (vis[v] || d[v] < now.first) continue;
            vis[v] = true;
            for (register int i = 0; i < edge[v].size(); i++) {
                Node *p = &edge[v][i];
                register int w = d[v] + p->w + h[v] - h[p->v];
                if (p->f > 0 && d[p->v] > w) {
                    d[p->v] = w, prev[p->v] = v, pree[p->v] = i;
                    if (id[p->v] != NULL)
                        q.modify(id[p->v], Pair(d[p->v], p->v));
                    else
                        id[p->v] = q.push(Pair(d[p->v], p->v));
                }
            }
        }
        if (d[t] == INF) break;
        for (register int i = 0; i <= n; i++) h[i] += d[i];
        register int flow = f;
        for (register int i = t; i != s; i = prev[i])
            flow = std::min(flow, edge[prev[i]][pree[i]].f);
        f -= flow, ans.first += flow, ans.second += flow * h[t];
        for (register int i = t; i != s; i = prev[i]) {
            Node *p = &edge[prev[i]][pree[i]];
            p->f -= flow, edge[p->v][p->index].f += flow;
        }
    }
    return ans;
}

int n, m, time[9][60];

inline int get(int j, int k) { return n + (j - 1) * n + k; }

inline void solve() {
    using namespace IO;
    read(m), read(n);
    for (register int i = 0; i < n; i++)
        for (register int j = 0; j < m; j++) read(time[j][i]);

    const int s = 0, t = n + n * m + 1;

    for (register int i = 1; i <= n; i++) addEdge(s, i, 1, 0);

    for (register int j = 1; j <= m; j++) {
        for (register int k = 1; k <= n; k++) {
            addEdge(get(j, k), t, 1, 0);
            for (register int i = 1; i <= n; i++)
                addEdge(i, get(j, k), 1, time[j - 1][i - 1] * k);
        }
    }
    std::cout << std::fixed << std::setprecision(2)
              << (double)minCostFlow(s, t, t + 1, INF).second / n;
}
}  // namespace PrimalDual

int main() {
    // freopen("in.in", "r", stdin);
    PrimalDual::solve();
    return 0;
}