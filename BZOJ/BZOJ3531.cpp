#include <bits/stdc++.h>

#define N 3000000
#define M 30000000
#define oo 0x7f7f7f7f

int n, q, x, y, mm, tot, digit[10], w[N + 10], c[N + 10], next[N + 10],
    son[N + 10], ed[N + 10], fa[N + 10], head[N + 10], heavy[N + 10],
    size[N + 10], deep[N + 10], id[N + 10], tree1[M + 10], tree2[M + 10],
    l[N + 10], r[N + 10], root[N + 10];
bool vis[N + 10];
char s[10];

inline void add(int x, int y) {
    next[++mm] = son[x];
    son[x] = mm;
    ed[mm] = y;
}

inline void dfs1(int x) {
    vis[x] = 1;
    size[x] = 1;
    for (int i = son[x]; i; i = next[i]) {
        int y = ed[i];
        if (!vis[y]) {
            fa[y] = x;
            deep[y] = deep[x] + 1;
            dfs1(y);
            size[x] += size[y];
            if (size[y] > size[heavy[x]]) heavy[x] = y;
        }
    }
}

inline void dfs2(int x) {
    vis[x] = 1;
    id[x] = ++tot;
    if (!head[x]) head[x] = x;
    if (heavy[x]) {
        head[heavy[x]] = head[x];
        dfs2(heavy[x]);
    }
    for (int i = son[x]; i; i = next[i]) {
        int y = ed[i];
        if (!vis[y]) dfs2(y);
    }
}

inline void up(int rt) {
    tree1[rt] = tree1[l[rt]] + tree1[r[rt]];
    tree2[rt] = std::max(tree2[l[rt]], tree2[r[rt]]);
}

inline void modify(int &rt, int lc, int rc, int x, int y) {
    if (!rt) rt = ++tot;
    if (lc == rc) {
        tree1[rt] = y;
        tree2[rt] = y;
        return;
    }
    int mid = (lc + rc) / 2;
    if (x <= mid)
        modify(l[rt], lc, mid, x, y);
    else
        modify(r[rt], mid + 1, rc, x, y);
    up(rt);
}

inline int qsum(int rt, int lc, int rc, int L, int R) {
    if (!rt) return 0;
    if (L == lc && R == rc) return tree1[rt];
    int mid = (lc + rc) / 2;
    if (R <= mid) return qsum(l[rt], lc, mid, L, R);
    if (L > mid) return qsum(r[rt], mid + 1, rc, L, R);
    return qsum(l[rt], lc, mid, L, mid) + qsum(r[rt], mid + 1, rc, mid + 1, R);
}

inline int qmax(int rt, int lc, int rc, int L, int R) {
    if (!rt) return 0;
    if (L == lc && R == rc) return tree2[rt];
    int mid = (lc + rc) / 2;
    if (R <= mid) return qmax(l[rt], lc, mid, L, R);
    if (L > mid) return qmax(r[rt], mid + 1, rc, L, R);
    return std::max(qmax(l[rt], lc, mid, L, mid),
                    qmax(r[rt], mid + 1, rc, mid + 1, R));
}

inline int qs(int x, int y) {
    int ans = 0, C = c[x];
    while (head[x] != head[y]) {
        if (deep[head[x]] < deep[head[y]]) std::swap(x, y);
        ans += qsum(root[C], 1, n, id[head[x]], id[x]);
        x = fa[head[x]];
    }
    if (deep[x] > deep[y]) std::swap(x, y);
    ans += qsum(root[C], 1, n, id[x], id[y]);
    return ans;
}

inline int qm(int x, int y) {
    int ans = -oo, C = c[x];
    while (head[x] != head[y]) {
        if (deep[head[x]] < deep[head[y]]) std::swap(x, y);
        ans = std::max(ans, qmax(root[C], 1, n, id[head[x]], id[x]));
        x = fa[head[x]];
    }
    if (deep[x] > deep[y]) std::swap(x, y);
    ans = std::max(ans, qmax(root[C], 1, n, id[x], id[y]));
    return ans;
}

int main() {
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; ++i) scanf("%d%d", &w[i], &c[i]);
    for (int i = 1; i < n; ++i) {
        scanf("%d%d", &x, &y);
        add(x, y);
        add(y, x);
    }
    dfs1(1);
    for (int i = 1; i <= n; ++i) vis[i] = 0;
    dfs2(1);
    tot = 0;
    for (int i = 1; i <= n; ++i) modify(root[c[i]], 1, n, id[i], w[i]);
    for (int i = 1; i <= q; ++i) {
        scanf("%s%d%d", s, &x, &y);
        if (s[0] == 'C')
            if (s[1] == 'C') {
                modify(root[c[x]], 1, n, id[x], 0);
                modify(root[y], 1, n, id[x], w[x]);
                c[x] = y;
            } else {
                modify(root[c[x]], 1, n, id[x], y);
                w[x] = y;
            }
        else if (s[1] == 'S')
            printf("%d\n", qs(x, y));
        else
            printf("%d\n", qm(x, y));
    }
    return 0;
}