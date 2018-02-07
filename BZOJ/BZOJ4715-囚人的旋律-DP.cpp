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
 * 「BZOJ 4715」囚人的旋律 07-02-2018
 * DP
 * @author xehoth
 */
#include <iostream>
#include <vector>

const int MOD = 1000000007;

inline void add(int &a, int b) { a = (a + b >= MOD ? a + b - MOD : a + b); }

int main() {
    int n, m, ans = 0;
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    std::cin >> n >> m;
    std::vector<int> out(n + 1), a(n + 1), f(n + 1), cnt(n + 1);
    for (int i = 1; i <= n; i++) a[i] = i;
    for (int i = 1, u, v; i <= m; i++) {
        std::cin >> u >> v;
        out[u < v ? u : v]++;
    }
    for (int i = 1; i <= n; i++) {
        f[i] = a[out[i] + 1];
        for (int j = out[i] + 1; j + i <= n + 1; j++) a[j] = a[j + 1];
    }
    for (int i = 1; i <= n; i++) {
        int low = f[i], x = n + 1;
        if (cnt[i] == 0) cnt[i] = 1;
        for (int j = i + 1; j <= n; j++) {
            if (f[j] < x && f[j] > low) {
                x = f[j];
                add(cnt[j], cnt[i]);
            }
        }
        if (x == n + 1) add(ans, cnt[i]);
    }
    std::cout << ans;
    return 0;
}