#include <bits/stdc++.h>

const int MAXN = 210;

double p[MAXN], f[2][MAXN][MAXN << 1];

int main() {
    int n, t, m, a;
    register double ans = 0;
    scanf("%d%d%d", &n, &t, &m), m = std::min(m, n);
    for (register int i = 1; i <= n; i++) scanf("%lf", &p[i]), p[i] /= 100;
    f[0][0][n + m] = 1;
    for (register int d = 1, i = 1; i <= n; i++, d ^= 1) {
        scanf("%d", &a);
        for (register int j = 0; j <= n; j++)
            for (register int k = 0; k <= n * 2; k++)
                f[d][j][k] = f[d ^ 1][j][k] * (1 - p[i]);
        for (register int j = 0; j < n; j++)
            for (register int k = 1; k <= n * 2; k++)
                f[d][j + 1][std::min(k + a, n * 2)] += f[d ^ 1][j][k] * p[i];
    }
    for (register int i = t; i <= n; i++)
        for (register int j = n; j <= 2 * n; j++) ans += f[n & 1][i][j];
    printf("%.6f\n", ans);
    return 0;
}