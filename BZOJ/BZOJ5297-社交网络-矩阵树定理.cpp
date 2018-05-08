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
 * 「BZOJ 5297」社交网络 03-05-2018
 * 矩阵树定理
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MOD = 10007;
const int MAXN = 250 + 9;

inline int modPow(int a, int b) {
    int ret = 1;
    for (; b; b >>= 1, a = a * a % MOD)
        if (b & 1) ret = ret * a % MOD;
    return ret;
}

int a[MAXN][MAXN];

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    int n, m, cnt = 0;
    std::cin >> n >> m;
    for (int i = 1, u, v; i <= m; i++) {
        std::cin >> u >> v;
        a[v][u]--;
        a[u][u]++;
    }
    for (int i = 2, j; i <= n; i++) {
        for (j = i; j <= n && !a[j][i];) j++;
        if (j != i) {
            cnt++;
            std::swap_ranges(a[j] + i, a[j] + n + 1, a[i] + i);
        }
        int inv = modPow(a[i][i], MOD - 2);
        for (j = i + 1; j <= n; j++)
            for (int k = n; k; k--)
                a[j][k] = (a[j][k] - a[i][k] * a[j][i] % MOD * inv % MOD) % MOD;
    }

    int ret = a[2][2];
    for (int i = 3; i <= n; i++) ret = ret * a[i][i] % MOD;
    ret = (ret + MOD) % MOD;
    if (cnt & 1) ret = MOD - ret;
    std::cout << ret << '\n';
    return 0;
}