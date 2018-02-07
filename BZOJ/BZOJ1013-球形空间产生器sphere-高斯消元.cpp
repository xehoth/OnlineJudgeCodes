/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1013」球形空间产生器sphere 19-01-2017
 * 高斯消元
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 13;

int n, m;

double c[MAXN][MAXN], f[MAXN][MAXN], ans[MAXN];

inline void gauss() {
    for (register int i = 1; i <= n; ++i) {
        register int l = i;
        for (register int j = l + 1; j <= n; ++j)
            if (fabs(f[l][i]) < fabs(f[j][i])) l = j;
        if (l != i)
            for (register int j = i; j <= m; ++j) std::swap(f[l][j], f[i][j]);
        for (register int j = i + 1; j <= n; ++j) {
            double temp = f[j][i] / f[i][i];
            for (register int k = i; k <= m; ++k)
                f[j][k] = f[j][k] - f[i][k] * temp;
        }
    }
    for (register int i = n; i >= 1; --i) {
        double t = f[i][m];
        for (register int j = n; j > i; --j) t -= ans[j] * f[i][j];
        ans[i] = t / f[i][i];
    }
    return;
}

int main() {
    scanf("%d", &n);
    m = n + 1;
    for (register int i = 0; i <= n; ++i)
        for (register int j = 1; j <= n; ++j) scanf("%lf", &c[i][j]);
    for (register int i = 1; i <= n; ++i) {
        register int j = i - 1;
        double d = 0;
        for (register int k = 1; k <= n; ++k) {
            f[i][k] = (c[i][k] - c[j][k]) * 2;
            d += c[i][k] * c[i][k] - c[j][k] * c[j][k];
        }
        f[i][m] = d;
    }
    gauss();
    for (register int i = 1; i <= n; ++i) {
        if (i < n)
            printf("%.3lf ", ans[i]);
        else
            printf("%.3lf\n", ans[i]);
    }
    return 0;
}