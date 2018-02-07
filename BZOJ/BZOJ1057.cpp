/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1057」24-03-2017
 *
 * @author xehoth
 */
#include <algorithm>
#include <cstdio>

const int MAXN = 2000;

inline int sqr(int x) { return x * x; }

int main() {
    int n, m;
    scanf("%d %d", &n, &m);

    static int a[MAXN + 1][MAXN + 1];
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            scanf("%d", &a[i][j]);
        }
    }

    // for (int i = 1; i <= n; i++) f[i][0] = -1;
    // for (int j = 1; j <= m; j++) f[0][j] = -1;

    static int f[MAXN + 1][MAXN + 1], g[MAXN + 1][MAXN + 1];
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (j == 1 || a[i][j] == a[i][j - 1]) {
                f[i][j] = 1;
            } else {
                f[i][j] = f[i][j - 1] + 1;
            }
            // printf("f(%d, %d) = %d\n", i, j, f[i][j]);
        }

        for (int j = m; j >= 1; j--) {
            if (j == m || a[i][j] == a[i][j + 1]) {
                g[i][j] = 1;
            } else {
                g[i][j] = g[i][j + 1] + 1;
            }
        }
    }

    int ansSquare = 1, ansRectangle = 1;
    for (int j = 1; j <= m; j++) {
        int up = 0, left = 0, right = 0;
        for (int i = 1; i <= n; i++) {
            if (i == 1 || a[i][j] == a[i - 1][j]) {
                up = 1;
                left = f[i][j];
                right = g[i][j];
            } else {
                up++;
                left = std::min(left, f[i][j]);
                right = std::min(right, g[i][j]);
            }

            // printf("up = %d, left = %d, right = %d\n", up, left, right);
            ansRectangle = std::max(ansRectangle, up * (left + right - 1));
            ansSquare =
                std::max(ansSquare, sqr(std::min(up, left + right - 1)));
        }
    }

    printf("%d\n%d\n", ansSquare, ansRectangle);

    return 0;
}