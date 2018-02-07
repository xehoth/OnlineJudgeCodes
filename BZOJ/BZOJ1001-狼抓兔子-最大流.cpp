/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1001」狼抓兔子 06-03-2017
 * 最大流
 * @author xehoth
 */
#include <bits/stdc++.h>

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
inline void read(T &x) {
    static bool iosig;
    static char ch;
    for (iosig = false, ch = read(); !isdigit(ch); ch = read()) {
        if (ch == -1) return;
        if (ch == '-') iosig = true;
    }
    for (x = 0; isdigit(ch); ch = read()) x = (x + (x << 2) << 1) + (ch ^ 48);
    if (iosig) x = -x;
}

const int MAXN = 1001 * 1001;
inline int min(int a, int b) { return a < b ? a : b; }
int ecnt = -1;
struct Node {
    int v, w, nxt;
} edges[MAXN * 6];
int adj[MAXN], q[MAXN];
bool vis[MAXN];
void addEdge(int u, int v, int w) {
    Node *t = &edges[++ecnt];
    t->v = v;
    t->w = w;
    t->nxt = adj[u];
    adj[u] = ecnt;
    t = &edges[++ecnt];
    t->v = u;
    t->w = w;
    t->nxt = adj[v];
    adj[v] = ecnt;
}
char c;

int n, m, ans, S, T, d[MAXN], vd[MAXN], N;
int aug(int u, int augco) {
    int dmin = N - 1, augc = augco, delta;
    if (u == T) return augco;
    for (int i = adj[u]; ~i; i = edges[i].nxt)
        if (edges[i].w > 0) {
            if (d[edges[i].v] + 1 == d[u]) {
                delta = min(augc, edges[i].w);
                delta = aug(edges[i].v, delta);
                edges[i].w -= delta;
                edges[i ^ 1].w += delta;
                augc -= delta;
                if (d[S] >= N) return augco - augc;
                if (!augc) return augco;
            }
            if (dmin > d[edges[i].v]) dmin = d[edges[i].v];
        }
    if (augco == augc) {
        --vd[d[u]];
        if (!vd[d[u]]) d[S] = N;
        d[u] = ++dmin;
        ++vd[dmin];
    }
    return augco - augc;
}
void bfs() {
    int l = 0, r = 0, u;
    q[++r] = T;
    vis[T] = 1;
    vd[0]++;
    while (l < r) {
        u = q[++l];
        for (int i = adj[u]; ~i; i = edges[i].nxt) {
            if (vis[edges[i].v]) continue;
            d[edges[i].v] = d[u] + 1;
            ++vd[d[u] + 1];
            vis[edges[i].v] = 1;
            q[++r] = edges[i].v;
        }
    }
}
void sap() {
    bfs();
    while (d[S] < N) ans += aug(S, 0x7fffffff);
}
int main() {
    // freopen("in.in", "r", stdin);
    int w;
    memset(adj, -1, sizeof adj);
    read(n), read(m);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m - 1; ++j) {
            read(w);
            addEdge(i * m + j, i * m + j + 1, w);
        }
    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j < m; ++j) {
            read(w);
            addEdge(i * m + j, (i + 1) * m + j, w);
        }
    for (int i = 0; i < n - 1; ++i)
        for (int j = 0; j < m - 1; ++j) {
            read(w);
            addEdge(i * m + j, (i + 1) * m + j + 1, w);
        }
    T = n * m - 1;
    N = n * m;
    sap();
    printf("%d\n", ans);
    return 0;
}