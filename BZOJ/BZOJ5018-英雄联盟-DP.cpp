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
 * 「BZOJ 5018」英雄联盟 07-03-2018
 * DP
 * @author xehoth
 */
#include <iostream>

const int MAXN = 121 + 2;
const int MAXM = MAXN * 199 + 2;

int n, K[MAXN], C[MAXN], limit[MAXN];
long long m, f[MAXN][MAXM];

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    std::cin >> n >> m;
    for (int i = 1; i <= n; i++) std::cin >> K[i];
    for (int i = 1; i <= n; i++) std::cin >> C[i];
    for (int i = 1; i <= n; i++) limit[i] = limit[i - 1] + K[i] * C[i];
    f[0][0] = 1;
    int ans = limit[n];
    for (int i = 1; i <= n; i++) {
        for (int c = 0; c <= std::min(ans, limit[i]); c++) {
            f[i][c] = f[i - 1][c];
            for (int k = 1, sum = 0; k <= K[i]; k++) {
                if ((sum += C[i]) > c) break;
                f[i][c] = std::max(f[i][c], f[i - 1][c - sum] * k);
                if (f[i][c] >= m) {
                    f[i][c] = m;
                    ans = std::min(ans, c);
                    break;
                }
            }
        }
    }
    std::cout << ans;
    return 0;
}