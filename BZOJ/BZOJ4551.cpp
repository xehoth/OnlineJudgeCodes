#include <bits/stdc++.h>
using namespace std;
#define M 200010
inline int read() {
    char ch = getchar();
    int x = 0, f = 1;
    while (ch < '0' || ch > '9') {
        if (ch == '-') f = -1;
        ch = getchar();
    }
    while ('0' <= ch && ch <= '9') {
        x = (x << 1) + (x << 3) + (ch ^ '0');
        ch = getchar();
    }
    return x * f;
}

struct Edge {
    int u, v, next;
} G[M];
int head[M], tot;
int fa[M], father[M];
int mark[M], res[M];

struct query {
    int op, num;
} a[M];

inline void add(int u, int v) {
    G[++tot] = (Edge){u, v, head[u]};
    head[u] = tot;
}

inline int find(int x) { return fa[x] == x ? x : fa[x] = find(fa[x]); }

inline void dfs(int x) {
    if (mark[x])
        fa[x] = x;
    else
        fa[x] = find(father[x]);
    for (int i = head[x]; i != -1; i = G[i].next)
        if (G[i].v != father[x]) {
            father[G[i].v] = x;
            dfs(G[i].v);
        }
}

int main() {
    int n = read(), Q = read();
    memset(head, -1, sizeof(head));
    for (int i = 1; i < n; i++) {
        int u = read(), v = read();
        add(u, v);
        add(v, u);
    }
    char ch[5];
    mark[1] = 1;
    for (int i = 1; i <= Q; i++) {
        scanf("%s", ch);
        if (ch[0] == 'C') {
            a[i].op = 1;
            a[i].num = read();
            mark[a[i].num]++;
        } else
            a[i].op = 2, a[i].num = read();
    }
    dfs(1);
    for (int i = Q; i >= 1; i--) {
        if (a[i].op == 1) {
            mark[a[i].num]--;
            if (!mark[a[i].num]) fa[a[i].num] = find(father[a[i].num]);
        } else {
            res[i] = find(a[i].num);
        }
    }
    for (int i = 1; i <= Q; i++) {
        if (a[i].op == 2) printf("%d\n", res[i]);
    }
    return 0;
}
