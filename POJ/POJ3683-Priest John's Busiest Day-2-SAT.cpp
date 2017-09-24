/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「POJ 3683」Priest John's Busiest Day 23-06-2017
 * 2-SAT
 * @author xehoth
 */
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

namespace TwoSat {

const int MAXN = 2001;

struct Data {
    int s, t;
} data[MAXN];

int low[MAXN], dfn[MAXN], scc[MAXN], idx, cnt, n;
bool vis[MAXN];
std::vector<int> st;

int ans[MAXN];
std::vector<int> edge[MAXN];

inline void tarjan(int u) {
    st.push_back(u), low[u] = dfn[u] = ++idx, vis[u] = true;
    register int v;
    for (register int i = 0; i < edge[u].size(); i++) {
        if (!dfn[v = edge[u][i]])
            tarjan(v), low[u] = std::min(low[u], low[v]);
        else if (vis[v])
            low[u] = std::min(low[u], dfn[v]);
    }
    if (dfn[u] == low[u]) {
        cnt++;
        do
            vis[v = st.back()] = false, st.pop_back(), scc[v] = cnt;
        while (u != v);
    }
}

inline bool isIntersect(const Data &a, const Data &b) {
    return (a.s <= b.s && a.t > b.s) || (b.s <= a.s && b.t > a.s);
}

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v);
    edge[v ^ 1].push_back(u ^ 1);
}

inline void solve() {
    scanf("%d", &n);
    for (register int i = 0, u1, v1, u2, v2, d; i < n; i++) {
        scanf("%d:%d %d:%d %d", &u1, &v1, &u2, &v2, &d);
        data[i << 1].s = u1 * 60 + v1;
        data[i << 1].t = u1 * 60 + v1 + d;
        data[i << 1 | 1].s = u2 * 60 + v2 - d;
        data[i << 1 | 1].t = u2 * 60 + v2;
    }
    for (register int i = 0; i < 2 * n; i++)
        for (register int j = i + 1; j < 2 * n; j++)
            if (((i >> 1) != (j >> 1)) && isIntersect(data[i], data[j]))
                addEdge(i, j ^ 1);
    st.reserve(n);
    for (register int i = 0, r = n << 1; i < r; i++)
        if (!dfn[i]) tarjan(i);
    for (register int i = 0; i < n; i++) {
        if (scc[i << 1] == scc[i << 1 | 1]) {
            printf("NO\n");
            return;
        }
    }
    for (register int i = 0; i < n; i++)
        ans[i] = (scc[i << 1] < scc[i << 1 | 1] ? (i << 1) : (i << 1 | 1));
    printf("YES\n");
    div_t tmp;
    for (register int i = 0; i < n; i++) {
        tmp = div(data[ans[i]].s, 60);
        printf("%02d:%02d ", tmp.quot, tmp.rem);
        tmp = div(data[ans[i]].t, 60);
        printf("%02d:%02d\n", tmp.quot, tmp.rem);
    }
}
}

int main() {
    TwoSat::solve();
    return 0;
}