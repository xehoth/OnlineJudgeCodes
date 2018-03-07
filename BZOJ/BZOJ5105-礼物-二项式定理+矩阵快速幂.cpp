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
 * 「BZOJ 5015」礼物 07-03-2018
 * 二项式定理 + 矩阵快速幂
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXK = 10 + 1;
const int MOD = 1000000007;

int c[MAXK][MAXK], k, m;
long long n;

struct Matrix {
    int a[MAXK + 2][MAXK + 2];

    Matrix() : a() {}

    inline void operator*=(const Matrix &b) {
        static int c[MAXK + 1][MAXK + 1];
        for (int i = 0; i < m; i++) memset(c[i], 0, sizeof(int) * m);
        for (int i = 0; i < m; i++) {
            for (int k = 0; k < m; k++) {
                for (int j = 0; j < m; j++) {
                    c[i][j] =
                        (c[i][j] + (unsigned long long)a[i][k] * b[k][j]) % MOD;
                }
            }
        }
        for (int i = 0; i < m; i++) memcpy(a[i], c[i], sizeof(int) * m);
    }

    inline int *operator[](int i) { return a[i]; }

    inline const int *operator[](int i) const { return a[i]; }

    inline void reset() {
        for (int i = 0; i < m; i++) a[i][i] = 1;
    }
};

inline Matrix modPow(Matrix a, long long b) {
    Matrix ret;
    ret.reset();
    for (; b; b >>= 1, a *= a)
        if (b & 1) ret *= a;
    return ret;
}

Matrix a, st;

int main() {
    std::cin >> n >> k;
    c[0][0] = 1;
    for (int i = 1; i <= k; i++) {
        c[i][0] = 1;
        for (int j = 0; j <= i; j++) c[i][j] = c[i - 1][j] + c[i - 1][j - 1];
    }
    a[0][0] = 2;
    m = k + 2;
    for (int i = 0; i < m; i++) st[0][i] = 1;
    for (int i = 1; i < m; i++) a[i][0] = c[k][i - 1];
    for (int j = 1; j < m; j++)
        for (int i = 1; i <= j; i++) a[i][j] = c[j - 1][i - 1];
    a = modPow(a, n - 2);
    st *= a;
    n %= MOD;
    int t = 1;
    for (int i = 0; i < k; i++) t = t * n % MOD;
    std::cout << (st[0][0] + t) % MOD;
    return 0;
}