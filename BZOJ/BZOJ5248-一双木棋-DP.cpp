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
 * 「BZOJ 5248」一双木棋 14-04-2018
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 10;
const int MAXS = 1 << (MAXN * 2) | 1;
const int INF = -0x3f3f3f3f - 1;

int f[MAXS], a[2][MAXN][MAXN], n, m;

int dp(int s, int d) {
    int &ret = f[s];
    if (ret != INF) return ret;
    for (int i = 0, j = m - 1, w = 0; s >> w; (s >> w) & 1 ? i++ : j--, w++)
        if (((s >> w) & 3) == 2)
            ret = std::max(ret, a[d][i][j] - dp(s - (1 << w), !d));
    return ret;
}

int main() {
    memset(f, 0xc0, sizeof(f));
    std::cin >> n >> m;
    for (int d = 0; d < 2; d++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) std::cin >> a[d][i][j];
    f[(1 << n) - 1] = 0;
    std::cout << dp(((1 << n) - 1) << m, 0);
    return 0;
}