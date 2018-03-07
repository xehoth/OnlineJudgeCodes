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
 * 「HDU 5275」Dylans loves polynomial 07-03-2018
 * 牛顿插值法
 * @author xehoth
 */
#include <iostream>

const int MAXV = 250000 + 1;
const int MAXN = 3000 + 3;
const int MOD = 1e9 + 7;

int inv[MAXV], f[MAXN][MAXN], x[MAXN], y[MAXN], n, q;

inline int get(int x) { return x < 0 ? -inv[-x] : inv[x]; }

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    inv[1] = 1;
    for (int i = 2; i < MAXV; i++)
        inv[i] = (unsigned long long)(MOD - MOD / i) * inv[MOD % i] % MOD;
    while (std::cin >> n) {
        for (int i = 1; i <= n; i++) std::cin >> x[i] >> y[i];
        for (int i = 1; i <= n; i++) f[0][i] = y[i];
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j + i <= n; j++) {
                f[i][j] = ((long long)(f[i - 1][j + 1] - f[i - 1][j]) *
                               get(x[i + j] - x[j]) % MOD +
                           MOD) %
                          MOD;
            }
        }

        std::cin >> q;
        while (q--) {
            int l, r, pos, ans = 0, mul = 1;
            std::cin >> l >> r >> pos;
            for (int i = 0; i < r - l + 1; i++) {
                ans = (ans + (long long)f[i][l] * mul) % MOD;
                mul = (long long)mul * (MOD + pos - x[i + l]) % MOD;
            }
            std::cout << ans << '\n';
        }
    }
}