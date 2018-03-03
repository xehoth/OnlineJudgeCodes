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
 * 「BZOJ 2718」毕业旅行 03-03-2018
 * Dilworth 定理
 * @author xehoth
 */
#include <bitset>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <queue>
#include <vector>

template <size_t MAXN, int INF = 0x3f3f3f3f>
struct Maxflow {
    struct Node {
        int v, f, i;

        Node(int v, int f, int i) : v(v), f(f), i(i) {}
    };

    std::vector<Node> g[MAXN];

    inline void addEdge(int u, int v, int f) {
        g[u].push_back(Node(v, f, g[v].size()));
        g[v].push_back(Node(u, 0, g[u].size() - 1));
    }

    typedef typename std::vector<Node>::iterator Iterator;

    int s, t, n;
    bool vis[MAXN];
    int h[MAXN], gap[MAXN], iter[MAXN];

    inline void bfs() {
        static std::queue<int> q;
        q.push(t);
        vis[t] = true;
        gap[0]++;
        for (int u; !q.empty();) {
            u = q.front();
            q.pop();
            for (Iterator p = g[u].begin(); p != g[u].end(); ++p) {
                if (!vis[p->v]) {
                    gap[h[p->v] = h[u] + 1]++;
                    vis[p->v] = true;
                    q.push(p->v);
                }
            }
        }
    }

    int dfs(int u, int flow) {
        if (u == t) return flow;
        int ret = 0;
        for (int &i = iter[u]; i < (int)g[u].size(); i++) {
            Node &p = g[u][i];
            if (p.f > 0 && h[u] == h[p.v] + 1) {
                int t = dfs(p.v, std::min(flow - ret, p.f));
                p.f -= t;
                g[p.v][p.i].f += t;
                if ((ret += t) == flow || h[s] >= n) return ret;
            }
        }
        if (--gap[h[u]] == 0) h[s] = n;
        gap[++h[u]]++;
        iter[u] = 0;
        return ret;
    }

    inline int maxflow(int s, int t, int n) {
        this->s = s;
        this->t = t;
        this->n = n;
        int ret = 0;
        for (bfs(); h[s] < n;) {
            memset(iter, 0, sizeof(int) * n);
            ret += dfs(s, INF);
        }
        return ret;
    }
};

const int MAXN = 200 + 1;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    int n, m;
    std::cin >> n >> m;
    static std::bitset<MAXN> f[MAXN];
    static Maxflow<MAXN * 2 + 1> g;
    for (int i = 0, u, v; i < m; i++) {
        std::cin >> u >> v;
        f[u].set(v);
    }
    for (int k = 1; k <= n; k++)
        for (int i = 1; i <= n; i++)
            if (f[i].test(k)) f[i] |= f[k];
    const int S = 0, T = 2 * n + 1;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (f[i].test(j)) g.addEdge(i, j + n, 1);
    for (int i = 1; i <= n; i++) {
        g.addEdge(S, i, 1);
        g.addEdge(i + n, T, 1);
    }
    std::cout << n - g.maxflow(S, T, T + 1);
    return 0;
}