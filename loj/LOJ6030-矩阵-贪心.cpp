/**
 * Copyright (c) 2018, xehoth
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
 * 「LOJ 6030」矩阵 31-05-2018
 * 贪心
 * @author xehoth
 */
#include <bits/stdc++.h>

int main() {
    const int MAXN = 1000;
    static char s[MAXN + 1];
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<int> r(n + 1), c(n + 1);
    bool flag = false;
    for (int i = 1; i <= n; i++) {
        std::cin >> (s + 1);
        for (int j = 1; j <= n; j++) {
            if (s[j] == '#') {
                flag = true;
                r[i]++;
                c[j]++;
            }
        }
    }
    if (!flag) {
        std::cout << "-1";
        return 0;
    }
    int ret = INT_MAX;
    for (int i = 1; i <= n; i++) ret = std::min(ret, n - r[i] + (c[i] == 0));
    for (int i = 1; i <= n; i++)
        if (c[i] != n) ret++;
    std::cout << ret;
    return 0;
}