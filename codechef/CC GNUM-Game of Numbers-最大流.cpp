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
 * 「CC GNUM」Game of Numbers 02-05-2018
 * 最大流
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 400;
const int MAXG = MAXN * MAXN * 2 + 9;
const int INF = 0x3f3f3f3f;

struct Maxflow {
    struct Node {
        int v, f, i;
    };

    std::vector<Node> g[MAXG];
    int s, t, n, gap[MAXG], h[MAXG], iter[MAXG];

    void addEdge(int u, int v, int f) {
        g[u].push_back({v, f, (int)g[v].size()});
        g[v].push_back({u, 0, (int)g[u].size() - 1});
    }

    void clear() {
        for (int i = 0; i <= n; i++) g[i].clear();
        memset(gap, 0, sizeof(int) * n);
        memset(h, 0, sizeof(int) * n);
    }

    int dfs(int u, int flow) {
        if (u == t) return flow;
        int ret = 0;
        for (int &i = iter[u]; i < (int)g[u].size(); i++) {
            Node &p = g[u][i];
            if (p.f > 0 && h[u] > h[p.v]) {
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

    int maxflow(int s, int t) {
        this->s = s;
        this->t = t;
        int ret = 0;
        for (gap[0] = n; h[s] < n;) {
            memset(iter, 0, sizeof(int) * n);
            ret += dfs(s, INF);
        }
        return ret;
    }
} g;

std::unordered_map<int, int> lm, rm;

int a[MAXN + 1], b[MAXN + 1];

inline void solve() {
    g.clear();
    lm.clear();
    rm.clear();
    int n;
    std::cin >> n;
    for (int i = 1; i <= n; i++) std::cin >> a[i];
    for (int i = 1; i <= n; i++) std::cin >> b[i];
    for (int i = 1, g; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (a[i] == b[j]) continue;
            g = std::__gcd(a[i], b[j]);
            if (g == 1) continue;
            if (a[i] < b[j]) {
                lm[g]++;
            } else {
                rm[g]++;
            }
        }
    }
    const int S = 0, T = lm.size() + rm.size() + 1;
    int lId = 1, rId;
    for (auto itL = lm.begin(); itL != lm.end(); ++itL, lId++) {
        rId = lm.size() + 1;
        for (auto itR = rm.begin(); itR != rm.end(); ++itR, rId++) {
            if (std::__gcd(itL->first, itR->first) != 1) {
                g.addEdge(lId, rId, INF);
            }
        }
    }
    lId = 1;
    for (auto itL = lm.begin(); itL != lm.end(); ++itL, lId++) {
        g.addEdge(S, lId, itL->second);
    }
    rId = lm.size() + 1;
    for (auto itR = rm.begin(); itR != rm.end(); ++itR, rId++) {
        g.addEdge(rId, T, itR->second);
    }
    g.n = T + 1;
    std::cout << g.maxflow(S, T) << '\n';
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int T;
    std::cin >> T;
    while (T--) {
        solve();
    }
    return 0;
}