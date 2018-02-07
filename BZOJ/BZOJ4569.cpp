#include <bits/stdc++.h>
int n, m, a, b, c, d, f[17][100010], v[100010], ans = 9, i, j;
inline int get(int i, int j) {
    return f[i][j] == j ? j : f[i][j] = get(i, f[i][j]);
}

inline void merge(int p, int x, int y) {
    if (get(p, x) == get(p, y)) return;
    f[p][f[p][x]] = f[p][y];
    if (!p) return;
    p--, merge(p, x, y), merge(p, x + (1 << p), y + (1 << p));
}

int main() {
    scanf("%d%d", &n, &m);
    if (n == 1) puts("10"), exit(0);
    for (i = 0; (1 << i) <= n; i++)
        for (j = 1; j + (1 << i) - 1 <= n; j++) f[i][j] = j;
    while (m--) {
        scanf("%d%d%d%d", &a, &b, &c, &d), i = 31 - __builtin_clz(b - a + 1);
        merge(i, a, c), merge(i, b - (1 << i) + 1, d - (1 << i) + 1);
    }
    for (v[get(0, 1)] = 1, i = 2; i <= n; i++)
        if (!v[get(0, i)]) v[f[0][i]] = 1, ans = 10LL * ans % 1000000007;
    return printf("%d", ans), 0;
}