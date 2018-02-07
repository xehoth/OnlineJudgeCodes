#include <bits/stdc++.h>

const int N = 2001;

struct Edge {
    int v, next;
} map[N * N];
int tt, hd[N];
int n, v[N], res;

void dfs(int now) {
    v[now] = 1;
    res++;
    for (int k = hd[now]; k; k = map[k].next)
        if (!v[map[k].v]) dfs(map[k].v);
}

inline void ins(int u, int v) {
    map[++tt].v = v;
    map[tt].next = hd[u];
    hd[u] = tt;
}

int main() {
    char c;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("\n");
        for (int j = 1; j <= n; j++) {
            scanf("%c", &c);
            if (c == '1') ins(i, j);
        }
    }

    for (int i = 1; i <= n; i++) {
        memset(v, 0, sizeof v);
        dfs(i);
    }
    printf("%d\n", res);

    return 0;
}
