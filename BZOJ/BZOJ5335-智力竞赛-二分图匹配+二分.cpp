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
 * 「BZOJ 5335」智力竞赛 21-05-2018
 * 二分图匹配 + 二分
 * @author xehoth
 */
#include <bits/stdc++.h>

struct InputStream {
    enum { SIZE = 1024 * 1024 };
    char ibuf[SIZE], *s, *t;

    inline char read() {
        if (s == t) t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin);
        return s == t ? -1 : *s++;
    }

    template <typename T>
    inline InputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        if (iosig) x = -x;
        return *this;
    }
} io;

const int MAXM = 500;
const int MAXG = MAXM * 2 + 3;
const int INF = 0x3f3f3f3f;
bool g[MAXM + 1][MAXM + 1];

std::bitset<MAXM + 1> tg[MAXM + 1];

int n, m, val[MAXM + 1], id[MAXM + 1];

struct Maxflow {
    struct Node {
        int v, f, i;

        Node(int v, int f, int i) : v(v), f(f), i(i) {}
    };

    std::vector<Node> g[MAXG];
    int s, t, n, h[MAXG], gap[MAXG], iter[MAXG];

    inline void addEdge(int u, int v, int f) {
        g[u].push_back(Node(v, f, g[v].size()));
        g[v].push_back(Node(u, 0, g[u].size() - 1));
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
        for (gap[0] = n; h[s] < n;) ret += dfs(s, INF);
        return ret;
    }

    inline void clear() {
        memset(h, 0, sizeof(int) * n);
        memset(gap, 0, sizeof(int) * n);
        for (int i = 0; i < n; i++) g[i].clear();
    }
} minChainCover;

inline void init() {
    std::vector<std::pair<int, int> > tmp(m);
    for (int i = 0; i < m; i++) tmp[i] = std::make_pair(val[i + 1], i + 1);
    std::sort(tmp.begin(), tmp.end());
    for (int i = 1; i <= m; i++) id[i] = tmp[i - 1].second;
}

inline bool check(int mid) {
    int r = 1;
    while (r <= m && val[id[r]] < mid) r++;
    r--;
    for (int i = 1; i <= r; i++)
        for (int j = 1; j <= r; j++)
            if (g[id[i]][id[j]]) tg[id[i]].set(id[j]);
    int ret = r;
    for (int k = 1, t; k <= r; k++) {
        t = id[k];
        for (int i = 1; i <= r; i++)
            if (tg[id[i]].test(t)) tg[id[i]] |= tg[t];
    }
    const int S = 0, T = r * 2 + 1;
    minChainCover.n = T + 1;
    for (int i = 1, t; i <= r; i++) {
        t = id[i];
        for (int j = 1; j <= r; j++) {
            if (i != j && tg[t].test(id[j])) {
                minChainCover.addEdge(i, j + r, 1);
            }
        }
    }
    for (int i = 1; i <= r; i++) minChainCover.addEdge(S, i, 1);
    for (int i = 1; i <= r; i++) minChainCover.addEdge(i + r, T, 1);
    ret -= minChainCover.maxflow(S, T);
    for (int i = 1; i <= r; i++) tg[id[i]].reset();
    minChainCover.clear();
    return ret <= n;
}

int main() {
    io >> n >> m;
    n++;
    int max = 0;
    for (int i = 1, k; i <= m; i++) {
        io >> val[i] >> k;
        max = std::max(val[i], max);
        for (int v; k--;) {
            io >> v;
            g[i][v] = true;
        }
    }
    init();
    if (check(max)) {
        std::cout << "AK";
        return 0;
    }
    int l = 1, r = 200;
    for (int mid; r - l > 1;) {
        mid = (l + r) >> 1;
        if (check(mid)) {
            l = mid;
        } else {
            r = mid;
        }
    }
    std::cout << l;
    return 0;
}
