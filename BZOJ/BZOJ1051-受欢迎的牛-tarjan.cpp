/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1051」受欢迎的牛 24-02-2017
 * tarjan
 * @author xehoth
 */
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stack>
#include <vector>
const int MAXN = 10010;
std::vector<int> edge[MAXN];
inline void addEdge(const int u, const int v) { edge[u].push_back(v); }
std::stack<int> st;
int out[MAXN], num[MAXN], dfn[MAXN], low[MAXN], inComponent[MAXN], idx, cnt;
bool vis[MAXN];
inline void tarjan(const int u) {
    vis[u] = true, st.push(u), low[u] = dfn[u] = ++idx;
    for (register int i = 0, v; i < edge[u].size(); i++) {
        if (!dfn[v = edge[u][i]])
            tarjan(v), low[u] = std::min(low[u], low[v]);
        else if (vis[v])
            low[u] = std::min(low[u], dfn[v]);
    }
    if (dfn[u] == low[u]) {
        register int v;
        num[inComponent[u] = ++cnt] = 1, vis[u] = false;
        while (v = st.top(), st.pop(), v != u)
            num[inComponent[v] = cnt]++, vis[v] = false;
    }
}
int n, m, ans;
int main() {
    std::ios::sync_with_stdio(0), std::cin.tie(0);
    std::cin >> n >> m;
    for (register int i = 1, u, v; i <= m; i++)
        std::cin >> u >> v, addEdge(v, u);
    for (register int i = 1; i <= n; i++)
        if (!dfn[i]) tarjan(i);
    for (register int i = 1; i <= n; i++)
        for (register int e = 0; e < edge[i].size(); e++)
            if (inComponent[i] != inComponent[edge[i][e]])
                out[inComponent[edge[i][e]]]++;
    for (register int i = 1; i <= cnt; i++) {
        if (!out[i] && !ans)
            ans = num[i];
        else if (!out[i]) {
            ans = 0;
            break;
        }
    }
    std::cout << ans;
    return 0;
}