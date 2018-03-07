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
 * 「BZOJ 5017」炸弹 07-03-2018
 * 线段树优化建图 + Tarjan
 * @author xehoth
 */
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <queue>
#include <vector>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };

    char ibuf[SIZE], *s, *t;

    inline char read() {
        return (s == t) && (t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin)),
               s == t ? -1 : *s++;
    }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        iosig && (x = -x);
        return *this;
    }
} io;

const int MAXN = 500000 + 1;
const int MAXM = 524288 * 2 + 1;
const int MOD = 1e9 + 7;

struct Bomb {
    int id;
    long long x, r;

    Bomb() {}

    Bomb(long long x) : x(x) {}

    inline bool operator<(const Bomb &p) const { return x < p.x; }
} b[MAXN];

int n, id[MAXM];

std::vector<int> g[MAXM], ng[MAXM];

void build(int k, int l, int r) {
    if (l == r) {
        id[l] = k;
        return;
    }
    int mid = (l + r) >> 1;
    build(k << 1, l, mid);
    build(k << 1 | 1, mid + 1, r);
    g[k].push_back(k << 1);
    g[k].push_back(k << 1 | 1);
}

void insert(int k, int l, int r, int s, int t, int pos) {
    if (s <= l && t >= r) {
        g[pos].push_back(k);
        return;
    }
    int mid = (l + r) >> 1;
    if (s <= mid) insert(k << 1, l, mid, s, t, pos);
    if (t > mid) insert(k << 1 | 1, mid + 1, r, s, t, pos);
}

bool vis[MAXM];
int low[MAXM], dfn[MAXM], idx, val[MAXM], cnt, scc[MAXM], f[MAXM], in[MAXM];

void tarjan(int u) {
    static std::vector<int> st;
    low[u] = dfn[u] = ++idx;
    st.push_back(u);
    vis[u] = true;
    int v;
    for (int i = 0; i < (int)g[u].size(); i++) {
        if (!dfn[v = g[u][i]]) {
            tarjan(v);
            low[u] = std::min(low[u], low[v]);
        } else if (vis[v]) {
            low[u] = std::min(low[u], dfn[v]);
        }
    }
    if (low[u] == dfn[u]) {
        cnt++;
        do {
            vis[v = st.back()] = false;
            st.pop_back();
            scc[v] = cnt;
        } while (u != v);
    }
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    io >> n;
    build(1, 1, n);
    int M = 2 * n - 1;
    for (int i = 1; i <= n; i++) {
        io >> b[i].x >> b[i].r;
        b[i].id = i;
    }
    std::sort(b + 1, b + n + 1);
    for (int i = 1, l, r; i <= n; i++) {
        l = std::lower_bound(b + 1, b + n + 1, Bomb(b[i].x - b[i].r)) - b;
        r = std::upper_bound(b + 1, b + n + 1, Bomb(b[i].x + b[i].r)) - b - 1;
        insert(1, 1, n, l, r, id[i]);
    }

    for (int i = 1; i <= M; i++)
        if (!dfn[i]) tarjan(i);
    for (int i = 1; i <= M; i++)
        for (int j = 0; j < (int)g[i].size(); j++)
            if (scc[i] != scc[g[i][j]]) ng[scc[g[i][j]]].push_back(scc[i]);
    for (int i = 1; i <= cnt; i++) {
        std::sort(ng[i].begin(), ng[i].end());
        ng[i].erase(std::unique(ng[i].begin(), ng[i].end()), ng[i].end());
        for (int j = 0; j < (int)ng[i].size(); j++) in[ng[i][j]]++;
    }
    for (int i = 1; i <= n; i++) val[scc[id[i]]]++;
    std::vector<int> q;
    q.reserve(cnt + 1);
    for (int i = 1; i <= cnt; i++)
        if (in[i] == 0) q.push_back(i);
    for (int u; !q.empty();) {
        u = q.back();
        q.pop_back();
        f[u] += val[u];
        for (int i = 0, v; i < (int)ng[u].size(); i++) {
            if (--in[v = ng[u][i]] == 0) q.push_back(v);
            f[v] += f[u];
        }
    }
    int ans = 0;
    for (int i = 1; i <= n; i++)
        ans = (ans + (long long)f[scc[id[i]]] * b[i].id) % MOD;
    std::cout << ans;
    return 0;
}