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
 * 「BZOJ 5290」道路 08-05-2018
 * 网络流
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 20000 * 2 + 9;
const long long INF = 0x3f3f3f3f3f3f3f3fll;

int n, ch[MAXN][2], type[MAXN], fa[MAXN];
long long a[MAXN], b[MAXN], c[MAXN], ans;

struct Maxflow {
    struct Node {
        int v, i;
        long long f;
        Node(int v, long long f, int i) : v(v), i(i), f(f) {}
    };

    std::vector<Node> g[MAXN];
    int s, t, n, gap[MAXN], h[MAXN], iter[MAXN];

    void addEdge(int u, int v, long long f) {
        g[u].push_back(Node(v, f, g[v].size()));
        g[v].push_back(Node(u, 0, g[u].size() - 1));
    }

    long long dfs(int u, long long flow) {
        if (u == t) return flow;
        long long ret = 0;
        for (int &i = iter[u]; i < (int)g[u].size(); i++) {
            Node &p = g[u][i];
            if (p.f > 0 && h[u] > h[p.v]) {
                long long df = dfs(p.v, std::min(flow - ret, p.f));
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

    long long maxflow(int s, int t) {
        this->s = s;
        this->t = t;
        long long ret = 0;
        for (gap[0] = n; h[s] < n;) {
            memset(iter, 0, sizeof(int) * n);
            ret += dfs(s, INF);
        }
        return ret;
    }
} g;

int main() {
    // freopen("road.in", "r", stdin);
    // freopen("road.out", "w", stdout);
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    std::cin >> n;
    for (int i = 1; i < n; i++) {
        std::cin >> ch[i][0] >> ch[i][1];
        if (ch[i][0] < 0) ch[i][0] = n - ch[i][0];
        if (ch[i][1] < 0) ch[i][1] = n - ch[i][1];
        type[ch[i][1]] = 1;
        fa[ch[i][0]] = fa[ch[i][1]] = i;
    }
    for (int i = n + 1; i <= 2 * n; i++) {
        std::cin >> a[i] >> b[i] >> c[i];
        ans += a[i] * b[i] * c[i];
        a[i] *= c[i];
        b[i] *= c[i];
    }
    for (int i = n - 1; i; i--) {
        a[i] = a[ch[i][0]] + a[ch[i][1]];
        b[i] = b[ch[i][0]] + b[ch[i][1]];
        c[i] = c[ch[i][0]] + c[ch[i][1]];
    }
    const int S = 0, T = 2 * n + 1;
    for (int i = 2; i <= 2 * n; i++) {
        if (i != n) {
            if (type[i]) {
                g.addEdge(S, i, a[i]);
            } else {
                g.addEdge(i, T, b[i]);
            }
        }
    }
    for (int i = 1; i <= n - 1; i++) g.addEdge(ch[i][1], ch[i][0], INF);
    for (int i = 2, u; i <= 2 * n; i++) {
        if (i != n) {
            for (u = i; u != 1; u = fa[u]) {
                if (type[u] != type[i]) {
                    if (type[u]) {
                        g.addEdge(i, u, c[i]);
                    } else {
                        g.addEdge(u, i, c[i]);
                    }
                }
            }
        }
    }
    g.n = T + 1;
    std::cout << g.maxflow(S, T) + ans;
    return 0;
}