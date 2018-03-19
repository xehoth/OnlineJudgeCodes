/**
 * Copyright (c) 2016-2018, xehoth
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * 「BZOJ 4519」不同的最小割 19-03-2018
 * GomoryHuTree
 * @author xehoth
 */
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>

struct InputStream {
    enum { SIZE = 1024 * 1024 };

    char ibuf[SIZE], *s;

    InputStream() : s(ibuf) { fread(s, 1, SIZE, stdin); }

    inline InputStream &operator>>(int &x) {
        while (!isdigit(*s)) s++;
        for (x = 0; isdigit(*s); s++) x = x * 10 + (*s ^ '0');
        return *this;
    }
} io;

const int MAXN = 850 + 9;
const int INF = 0x3f3f3f3f;

struct Maxflow {
    struct Node {
        int v, f, c, i;

        Node(int v, int f, int i) : v(v), f(f), c(f), i(i) {}
    };

    std::vector<Node> g[MAXN];

    inline void addEdge(int u, int v, int f) {
        g[u].push_back(Node(v, f, g[v].size()));
        g[v].push_back(Node(u, f, g[u].size() - 1));
    }

    typedef std::vector<Node>::iterator Iterator;

    int n, s, t, iter[MAXN], h[MAXN], gap[MAXN];

    int dfs(int u, int flow) {
        if (u == t) return flow;
        int ret = 0;
        for (int &i = iter[u]; i < (int)g[u].size(); i++) {
            Node &p = g[u][i];
            if (p.f > 0 && h[u] == h[p.v] + 1) {
                int df = dfs(p.v, std::min(flow - ret, p.f));
                p.f -= df;
                g[p.v][p.i].f += df;
                if ((ret += df) == flow || h[s] >= n) return ret;
            }
        }
        if (--gap[h[u]] == 0) h[s] = n;
        gap[++h[u]]++;
        iter[u] = 0;
        return ret;
    }

    inline int maxflow(int s, int t) {
        this->s = s;
        this->t = t;
        memset(gap, 0, sizeof(int) * (n + 1));
        memset(h, 0, sizeof(int) * (n + 1));
        int ret = 0;
        for (gap[0] = n; h[s] < n;) {
            memset(iter, 0, sizeof(int) * (n + 1));
            ret += dfs(s, INF);
        }
        return ret;
    }

    inline void reset() {
        for (int i = 0; i <= n; i++)
            for (Iterator p = g[i].begin(); p != g[i].end(); ++p) p->f = p->c;
    }

    inline std::vector<Node> &operator[](int i) { return g[i]; }

    inline const std::vector<Node> &operator[](int i) const { return g[i]; }
} g;

struct GomoryHuTree {
    int fa[MAXN], flow[MAXN];
    bool vis[MAXN];

    inline bool bfs(Maxflow &g) {
        memset(vis, 0, sizeof(bool) * (g.n + 1));
        std::queue<int> q;
        q.push(g.s);
        vis[g.s] = true;
        for (int u; !q.empty();) {
            u = q.front();
            q.pop();
            for (Maxflow::Iterator p = g[u].begin(); p != g[u].end(); ++p) {
                if (p->f && !vis[p->v]) {
                    vis[p->v] = true;
                    q.push(p->v);
                }
            }
        }
    }

    inline void build(Maxflow &g) {
        for (int i = 2; i <= g.n; i++) fa[i] = 1;
        for (int u = 2, v; u <= g.n; u++) {
            v = fa[u];
            flow[u - 1] = g.maxflow(u, v);
            bfs(g);
            for (int i = u + 1; i <= g.n; i++)
                if (fa[i] == v && vis[i]) fa[i] = u;
            g.reset();
        }
    }
} d;

int main() {
    int n, m;
    io >> n >> m;
    g.n = n;
    for (int i = 0, u, v, f; i < m; i++) {
        io >> u >> v >> f;
        g.addEdge(u, v, f);
    }
    d.build(g);
    std::sort(d.flow + 1, d.flow + n);
    std::cout << std::unique(d.flow + 1, d.flow + n) - d.flow - 1;
    return 0;
}