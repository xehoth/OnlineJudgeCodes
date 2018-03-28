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
 * 「BZOJ 4588」CoinChange 28-03-2018
 * 矩阵快速幂 + 取模优化
 * @author xehoth
 */
#include <bits/stdc++.h>

typedef unsigned long long H;
const int MOD = 1000000007;
const int MAXN = 40 + 1;
const H RMOD = (ULLONG_MAX / MOD - MOD) * MOD;

H v[MAXN], f[MAXN][MAXN], ans[MAXN][MAXN], mat[MAXN][MAXN], idx[MAXN];
H tmp[MAXN][MAXN];

inline void mul(H a[MAXN][MAXN], H b[MAXN][MAXN], int o) {
    for (int j = o; j; j--) {
        for (int i = 1; i <= j; i++) {
            a[i][j] = (a[i][j] * b[j][j]) % MOD;
            for (int k = j - 1; k >= i; --k)
                if (a[i][k] && b[k][j])
                    if ((a[i][j] += a[i][k] * b[k][j]) >= RMOD) a[i][j] -= RMOD;
        }
    }
    for (int j = o; j; j--)
        for (int i = 1; i <= j; i++) a[i][j] %= MOD;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    int n;
    H m;
    while (std::cin >> n >> m) {
        H x = m;
        for (int i = 1; i <= n; i++) std::cin >> v[i];
        v[n + 1] = 0;
        for (int i = n; i; i--) {
            idx[i] = m / v[i];
            m %= v[i];
        }
        m = x;
        memset(ans, 0, sizeof(ans));
        for (int o = 1; o <= n; o++) {
            if (idx[o]) {
                for (int i = o; i; i--) ans[i][i] = 1;
                break;
            }
        }
        for (int o = 1; o <= n && v[o] <= m; o++) {
            for (int i = o; i; i--) f[i][o] = 1;
            memcpy(mat, f, sizeof(mat));
            memset(f, 0, sizeof(f));
            for (int i = o; i; i--) f[i][i] = 1;
            for (H rate = v[o + 1] / v[o];;) {
                if (idx[o] & 1) mul(ans, mat, o);
                if (rate & 1) mul(f, mat, o);
                rate >>= 1;
                if ((idx[o] >>= 1) || rate) {
                    for (int j = o; j; j--)
                        for (int i = j; i; i--) tmp[i][j] = 0;
                    for (int j = o; j; j--) {
                        for (int k = j; k; --k) {
                            if (mat[k][j]) {
                                for (int i = k; i; i--) {
                                    if (mat[i][k] &&
                                        (tmp[i][j] += mat[i][k] * mat[k][j]) >=
                                            RMOD) {
                                        tmp[i][j] -= RMOD;
                                    }
                                }
                            }
                        }
                    }
                    for (int j = o; j; j--)
                        for (int i = j; i; i--) mat[i][j] = tmp[i][j] % MOD;
                } else {
                    break;
                }
            }
        }
        H sum = 0;
        for (int i = 1; i <= n; i++) sum += ans[1][i];
        std::cout << sum % MOD << '\n';
    }
}