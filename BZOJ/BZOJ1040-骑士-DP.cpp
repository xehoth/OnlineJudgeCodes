/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1040」骑士 16-01-2017
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 1000000 + 10;

struct Edge {
    int v;
    bool f, vis;
    Edge *next, *rev;
} edge[MAXN << 1], *cur = edge, *head[MAXN], *cut;

inline void addEdge(int u, int v) {
    cur->v = v, cur->next = head[u], head[u] = cur++;
    cur->v = u, cur->next = head[v], head[v] = cur++;
    head[u]->rev = head[v], head[v]->rev = head[u];
}

int wt[MAXN], fa[MAXN], n, st, ed, fa2[MAXN];
long long ans, gs[MAXN], s[MAXN], f[MAXN];
bool vis[MAXN];

inline void dfs(int u) {
    vis[u] = true;
    for (Edge *p = head[u]; p; p = p->next) {
        if (!p->vis) {
            p->vis = p->rev->vis = true;
            if (vis[p->v]) {
                st = p->v, ed = u, cut = p;
                continue;
            }
            fa[p->v] = u, dfs(p->v);
        }
    }
}

inline void dfs2(int u) {
    gs[u] = s[u] = 0;
    for (Edge *p = head[u]; p; p = p->next)
        if (!p->f && p->v != fa2[u]) fa2[p->v] = u, dfs2(p->v);
    f[u] = std::max(gs[u] + wt[u], s[u]);
    s[fa2[u]] += f[u];
    gs[fa2[fa2[u]]] += f[u];
}

inline void solve() {
    for (register int i = 1; i < n; i++) {
        long long max = 0;
        if (!vis[i]) {
            dfs(i);
            cut->f = cut->rev->f = true;
            fa2[st] = 0;
            dfs2(st);
            max = std::max(max, s[st]);
            fa2[ed] = 0;
            dfs2(ed);
            max = std::max(max, s[ed]);
            cut->f = cut->rev->f = false;
        }
        ans += max;
    }
}

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    if (s == t) {
        t = (s = buf) + fread(buf, 1, IN_LEN, stdin);
        if (s == t) return -1;
    }
    return *s++;
}

template <class T>
inline bool read(T &x) {
    static bool iosig;
    static char c;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return false;
        if (c == '-') iosig = true;
    }
    for (x = 0; isdigit(c); c = read()) x = (x << 1) + (x << 3) + (c ^ '0');
    if (iosig) x = -x;
    return true;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
#endif
    read(n);
    for (register int i = 1, t; i <= n; i++) {
        read(wt[i]), read(t);
        addEdge(i, t);
    }
    solve();
    std::cout << ans;
    return 0;
}