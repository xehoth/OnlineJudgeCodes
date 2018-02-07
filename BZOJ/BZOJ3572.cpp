/*
 * created by xehoth on 19-01-2017
 */
#include <bits/stdc++.h>

typedef std::pair<int, int> Pair;

inline int read() {
    static char ch;
    while (ch = getchar(), ch < '0' || ch > '9')
        ;
    int res = ch - 48;
    while (ch = getchar(), ch >= '0' && ch <= '9') res = res * 10 + ch - 48;
    return res;
}

const int MAXN = 3e5 + 3;
const int MAX_LOG = 18;
const int INF = INT_MAX >> 1;

int n, m, sal[MAXN], ans[MAXN];
int num, aux[MAXN], faAux[MAXN], val[MAXN], delta[MAXN];
int dep[MAXN], dfn[MAXN], size[MAXN], father[MAXN][MAX_LOG + 1];
int edgeCount, adj[MAXN], next[MAXN * 2], to[MAXN * 2];
Pair f[MAXN];

inline void addEdge(const int u, const int v) {
    (++edgeCount)[next] = adj[u], adj[u] = edgeCount, edgeCount[to] = v;
    (++edgeCount)[next] = adj[v], adj[v] = edgeCount, edgeCount[to] = u;
}

inline bool cmp(const int &u, const int &v) { return dfn[u] < dfn[v]; }

inline void build() {
    static int tot, que[MAXN];
    que[tot = 1] = dep[1] = size[1] = 1;
    for (register int i = 1; i <= tot; i++) {
        register int u = que[i];
        for (register int e = adj[u]; e; e = next[e]) {
            register int v = to[e];
            if (father[u][0] == v) continue;
            father[v][0] = u, dep[v] = dep[u] + 1, que[++tot] = v;
        }
    }

    for (register int i = tot; i >= 2; i--)
        size[father[que[i]][0]] += ++size[que[i]];
    dfn[1] = 1;

    for (register int i = 1; i <= tot; i++) {
        register int u = que[i], s = dfn[u];
        for (register int e = adj[u]; e; e = next[e]) {
            register int v = to[e];
            if (father[u][0] == v) continue;
            dfn[v] = s + 1, s += size[v];
        }
    }
    for (register int j = 1; j <= MAX_LOG; j++)
        for (register int i = 1; i <= n; i++)
            father[i][j] = father[father[i][j - 1]][j - 1];
}

inline int getLca(int u, int v) {
    if (dep[u] < dep[v]) std::swap(u, v);
    for (register int d = dep[u] - dep[v], i = 0; d; ++i, d >>= 1)
        if (d & 1) u = father[u][i];
    if (u == v) return u;
    for (register int i = MAX_LOG; i >= 0; i--)
        if (father[u][i] != father[v][i]) u = father[u][i], v = father[v][i];
    return father[u][0];
}

inline int jump(int u, int d) {
    for (register int i = 0; d; i++, d >>= 1)
        if (d & 1) u = father[u][i];
    return u;
}

inline void buildAuxtree(const int &tot) {
    static int top, stk[MAXN];
    stk[top = 0] = 0, num = tot;

    std::sort(&aux[1], &aux[1] + tot, cmp);

    for (register int i = 1; i <= tot; i++) {
        register int u = aux[i];
        if (!top) {
            faAux[u] = 0, stk[++top] = u;
        } else {
            register int lca = getLca(stk[top], u);
            while (dep[stk[top]] > dep[lca]) {
                if (dep[stk[top - 1]] <= dep[lca]) faAux[stk[top]] = lca;
                top--;
            }
            if (stk[top] != lca) {
                faAux[lca] = stk[top];
                f[lca] = std::make_pair(INF, 0);
                stk[++top] = lca;
                aux[++num] = lca;
            }
            faAux[u] = lca, stk[++top] = u;
        }
    }
    std::sort(&aux[1], &aux[1] + num, cmp);
}

inline void solve() {
    for (register int i = num; i >= 2; i--) {
        register int u = aux[i], v = faAux[u];
        delta[u] = dep[u] - dep[v];
        Pair tmp(f[u].first + delta[u], f[u].second);
        if (tmp < f[v]) f[v] = tmp;
    }
    for (register int i = 2; i <= num; i++) {
        register int u = aux[i], v = faAux[u];
        Pair tmp(f[v].first + delta[u], f[v].second);
        if (tmp < f[u]) f[u] = tmp;
    }
    for (register int i = 1; i <= num; i++) {
        register int u = aux[i], v = faAux[u];
        val[u] = size[u];
        if (i == 1) {
            ans[f[u].second] += n - size[u];
            continue;
        }
        register int son = jump(u, dep[u] - dep[v] - 1);
        register int calc = size[son] - size[u];
        val[v] -= size[son];
        if (f[u].second == f[v].second) {
            ans[f[u].second] += calc;
        } else {
            register int z = f[u].first - f[v].first + dep[u] + dep[v] + 1 >> 1;
            if (f[v].second < f[u].second &&
                f[v].first + z - dep[v] == f[u].first + dep[u] - z)
                z++;
            z = size[jump(u, dep[u] - z)] - size[u];
            ans[f[u].second] += z, ans[f[v].second] += calc - z;
        }
    }
    for (register int i = 1; i <= num; i++)
        ans[f[aux[i]].second] += val[aux[i]];
}

int main() {
    n = read();
    for (register int i = 1; i < n; i++) addEdge(read(), read());
    build();
    for (register int q = read(); q; --q) {
        m = read();
        for (register int i = 1; i <= m; i++) {
            register int u = read();
            aux[i] = sal[i] = u;
            f[u] = std::make_pair(0, u);
            ans[u] = 0;
        }
        buildAuxtree(m);
        solve();
        for (register int i = 1; i <= m; i++) printf("%d ", ans[sal[i]]);
        putchar('\n');
    }
    return 0;
}