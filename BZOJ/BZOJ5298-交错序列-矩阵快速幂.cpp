/**
 * Copyright (c) 2016-2018, xehoth
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * 「BZOJ 5298」交错序列 03-05-2018
 * 矩阵快速幂
 * @author xehoth
 */
#include <bits/stdc++.h>

typedef unsigned long long H;

const int MAXN = 45;
const int MAXM = (MAXN * 2 + 1) * 2 + 9;

int n, m, a, b, mod, c[MAXN * 2 + 9][MAXN * 2 + 9], pw[MAXN + 9];

inline int add(int x, int v) { return x + v >= mod ? x + v - mod : x + v; }

struct Matrix {
    int a[MAXM][MAXM];

    inline void operator*=(const Matrix &b) {
        static H c[MAXM][MAXM];
        for (int i = 0; i < m; i++) memset(c[i], 0, sizeof(H) * m);
        for (int i = 0; i < m; i++) {
            for (int k = 0; k < m; k++) {
                if (a[i][k]) {
                    for (int j = 0; j < m; j++) {
                        c[i][j] += (H)a[i][k] * b[k][j];
                    }
                }
            }
        }
        for (int i = 0; i < m; i++)
            for (int j = 0; j < m; j++) a[i][j] = c[i][j] % mod;
    }

    int *operator[](int i) { return a[i]; }

    const int *operator[](int i) const { return a[i]; }
} mat, ret;

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    std::cin >> n >> a >> b >> mod;
    m = a + b + 1;
    for (int i = 0; i <= m; i++) c[i][0] = 1;
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= i; j++)
            c[i][j] = add(c[i - 1][j], c[i - 1][j - 1]);
    pw[0] = 1;
    for (int i = 1; i <= a; i++) pw[i] = (H)pw[i - 1] * n % mod;
    for (int i = 0; i < m; i++) mat[i + m][i] = mat[i + m][i + m] = 1;
    for (int i = 0; i < m; i++)
        for (int j = 0; j <= i; j++) mat[j][i + m] = c[i][j];
    ret.a[0][m] = 1;
    m <<= 1;
    for (int b = n + 1; b; b >>= 1, mat *= mat)
        if (b & 1) ret *= mat;
    m >>= 1;
    int ans = 0;
    for (int k = 1, i = 0; i <= a; i++, k = -k) {
        ans = add(ans, (H)c[a][i] * (mod + k) % mod * pw[a - i] % mod *
                           ret[0][b + i + m] % mod);
    }
    std::cout << ans;
    return 0;
}