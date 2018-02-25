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
 * 「CF GYM 101666 E」25-02-2018
 * 二分 + 二分图匹配
 * @author xehoth
 */
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <list>
#include <queue>
#include <vector>

const int INF = 0x3f3f3f3f;

struct Maxflow {
    struct Node {
        int v, f, i;

        Node(int v, int f, int i) : v(v), f(f), i(i) {}
    };

    std::vector<std::vector<Node> > g;
    std::vector<int> h, gap, iter;

    int n, s, t;

    inline void addEdge(const int u, const int v, const int f) {
        g[u].push_back(Node(v, f, g[v].size()));
        g[v].push_back(Node(u, 0, g[u].size() - 1));
    }

    inline void bfs() {
        h.assign(n, n);
        gap.assign(n + 1, 0);
        gap[0]++;
        h[t] = 0;
        std::queue<int> q;
        q.push(t);
        for (int u; !q.empty();) {
            u = q.front();
            q.pop();
            for (const auto &p : g[u]) {
                if (h[p.v] == n && g[p.v][p.i].f > 0) {
                    gap[h[p.v] = h[u] + 1]++;
                    q.push(p.v);
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
                int t = dfs(p.v, std::min(p.f, flow - ret));
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

    inline void init(const int n) {
        g.clear();
        g.resize(n);
        gap.assign(n + 1, 0);
        h.assign(n, 0);
    }

    inline int maxflow(int s, int t, int n) {
        this->s = s;
        this->t = t;
        this->n = n;
        int ret = 0;
        for (bfs(); iter.assign(n, 0), h[s] < n;) ret += dfs(s, INF);
        return ret;
    }
};

Maxflow g;

const int MAXN = 250 + 9;

struct Point {
    int x, y;

    Point() {}

    Point(const int x, const int y) : x(x), y(y) {}

    inline int dis(const Point &p) const {
        return (x - p.x) * (x - p.x) + (y - p.y) * (y - p.y);
    }
} bp[MAXN], rp[MAXN];

int n, b, r;

int dis[MAXN][MAXN];

inline bool check(double x) {
    x *= x;
    const int S = 0, T = b + r + 1;
    g.init(T + 1);
    for (int i = 1; i <= b; i++)
        for (int j = 1; j <= r; j++)
            if (dis[i][j] < x) g.addEdge(i, j + b, 1);
    for (int i = 1; i <= b; i++) g.addEdge(S, i, 1);
    for (int i = 1; i <= r; i++) g.addEdge(b + i, T, 1);
    return b + r - g.maxflow(S, T, T + 1) >= n;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::cin >> n >> b >> r;
    for (int i = 1; i <= b; i++) std::cin >> bp[i].x >> bp[i].y;
    for (int i = 1; i <= r; i++) std::cin >> rp[i].x >> rp[i].y;
    for (int i = 1; i <= b; i++)
        for (int j = 1; j <= r; j++) dis[i][j] = bp[i].dis(rp[j]);
    double l = 0, r = 2e4 + 1;
    for (double mid; r - l > 1e-7;) {
        mid = (l + r) / 2;
        if (check(mid))
            l = mid;
        else
            r = mid;
    }
    std::cout << std::fixed << std::setprecision(8) << l;
    return 0;
}
