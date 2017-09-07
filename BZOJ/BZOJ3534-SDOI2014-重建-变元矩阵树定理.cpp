/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SDOI 2014」重建 07-09-2017
 * 变元矩阵树定理
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

const int MAXN = 60;

const double EPS = 1e-10;

struct Task {
#define long long long
    double a[MAXN + 1][MAXN + 1];

    template <typename T, size_t size>
    inline bool gaussJordan(T a[size][size], const int n) {
        for (register int i = 0, idx; idx = i, i < n; i++) {
            for (register int j = i + 1; j < n; j++)
                if (fabs(a[j][i]) > fabs(a[idx][i])) idx = j;
            if (fabs(a[idx][i]) < EPS) return false;
            if (i != idx)
                for (register int j = i; j <= n; j++)
                    std::swap(a[i][j], a[idx][j]);
            for (register int j = 0; j < n; j++)
                if (i != j)
                    for (register int k = n; k >= i; k--)
                        a[j][k] -= a[i][k] / a[i][i] * a[j][i];
        }
    }

    template <typename T, size_t size>
    inline double getDetVal(T a[size][size], const int n) {
        gaussJordan(a, n);
        register double ans = 1;
        for (register int i = 0; i < n - 1; i++) ans *= a[i][i];
        return fabs(ans);
    }

    inline void solve() {
        register int n;
        std::cin >> n;
        register double f = 1;
        for (register int i = 0; i < n; i++) {
            for (register int j = 0; j < n; j++) {
                std::cin >> a[i][j];
                if (i != j) {
                    if (a[i][j] > 1 - EPS) a[i][j] -= EPS;
                    if (i < j) f *= 1 - a[i][j];
                    a[i][j] = a[i][j] / (1 - a[i][j]);
                }
            }
        }
        for (register int i = 0; i < n; i++)
            for (register int j = 0; j < n; j++)
                if (i != j) a[i][i] -= a[i][j];
        std::cout << std::fixed << std::setprecision(13) << getDetVal(a, n) * f;
    }
#undef long
} task;
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    task.solve();
    return 0;
}
