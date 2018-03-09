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
 * 「LOJ 2276」新型城市化 07-03-2018
 * 网络流 + Tarjan
 * @author xehoth
 */
#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>

const int MAXN = 10000 + 9;
const int INF = 0x3f3f3f3f;

int n, m;

struct Maxflow {
    struct Node {
        int v, f, i, t;

        Node(int v, int f, int i, int t) : v(v), f(f), i(i), t(t) {}
    };

    int s, t, n, iter[MAXN], gap[MAXN], h[MAXN];

    std::vector<Node> g[MAXN];

    typedef std::vector<Node>::iterator Iterator;

    inline void addEdge(int u, int v, int f) {
        g[u].push_back(Node(v, f, g[v].size(), 0));
        g[v].push_back(Node(u, 0, g[u].size() - 1, 1));
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
        for (gap[0] = n; h[s] < n;) {
            memset(iter, 0, sizeof(int) * n);
            ret += dfs(s, INF);
        }
        return ret;
    }

    int low[MAXN], dfn[MAXN], idx, scc[MAXN], cnt, sz[MAXN];
    bool vis[MAXN];

    inline void tarjan(int u) {
        static std::vector<int> st;
        st.push_back(u);
        vis[u] = true;
        low[u] = dfn[u] = ++idx;
        for (Iterator p = g[u].begin(); p != g[u].end(); ++p) {
            if (p->f > 0) {
                if (!dfn[p->v]) {
                    tarjan(p->v);
                    low[u] = std::min(low[u], low[p->v]);
                } else if (vis[p->v]) {
                    low[u] = std::min(low[u], dfn[p->v]);
                }
            }
        }
        if (dfn[u] == low[u]) {
            cnt++;
            int v;
            do {
                vis[v = st.back()] = false;
                st.pop_back();
                sz[cnt]++;
                scc[v] = cnt;
            } while (u != v);
        }
    }
} flow;

std::vector<int> g[MAXN];

int color[MAXN];

void dfs(int u, int c) {
    color[u] = c;
    for (int i = 0, v; i < (int)g[u].size(); i++)
        if (!color[v = g[u][i]]) dfs(v, -c);
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    std::cin >> n >> m;
    for (int i = 1, u, v; i <= m; i++) {
        std::cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for (int i = 1; i <= n; i++)
        if (!color[i]) dfs(i, 1);
    const int S = 0, T = n + 1;
    for (int i = 1; i <= n; i++) {
        if (color[i] == 1) {
            flow.addEdge(S, i, 1);
            for (int j = 0; j < (int)g[i].size(); j++)
                flow.addEdge(i, g[i][j], 1);
        } else {
            flow.addEdge(i, T, 1);
        }
    }
    flow.maxflow(S, T, T + 1);
    for (int i = 1; i <= n; i++)
        if (!flow.dfn[i]) flow.tarjan(i);
    int *scc = flow.scc;
    std::vector<std::pair<int, int> > ans;
    for (int i = 1; i <= n; i++) {
        for (Maxflow::Iterator p = flow.g[i].begin(); p != flow.g[i].end();
             ++p) {
            if (p->f == 0 && p->t == 0) {
                if (flow.sz[scc[p->v]] == 1) {
                    ans.push_back(
                        std::make_pair(std::min(i, p->v), std::max(i, p->v)));
                }
            }
        }
    }
    std::cout << ans.size() << '\n';
    std::sort(ans.begin(), ans.end());
    for (int i = 0; i < (int)ans.size(); i++)
        std::cout << ans[i].first << ' ' << ans[i].second << '\n';
    return 0;
}