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
 * 「SuperOJ 1692」Alice的幸运数 08-02-2018
 * 数位 DP
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXV = 9 * 18 + 1;
const int MAXN = 19;
const int MAXVH = 9 * 9 + 2;
int gcd[MAXV][MAXV];

int K;
long long l, r;
int d[MAXN];

long long f[MAXN][MAXVH][MAXVH];

long long dp(int pos, int F, int G, int limit) {
    if (pos < 1) return F > 0 && G > 0 && gcd[F][G] <= K;
    if (!limit && f[pos][F][G] != -1) return f[pos][F][G];
    register int end = limit ? d[pos] : 9;
    register long long ret = 0;
    for (register int i = 0; i <= end; i++) {
        if (pos & 1) {
            ret += dp(pos - 1, F + i, G, limit && (i == end));
        } else {
            ret += dp(pos - 1, F, G + i, limit && (i == end));
        }
    }
    if (!limit) f[pos][F][G] = ret;
    return ret;
}

inline long long solve(long long x) {
    register int len = 0;
    for (; x; x /= 10) d[++len] = x % 10;
    return dp(len, 0, 0, 1);
}

struct Query {
    long long *ans;
    long long l, r;
} q[1001];

std::vector<Query *> vc[101];

long long ans[1001];

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    for (register int i = 1; i < MAXV; i++)
        for (register int j = 1; j < MAXV; j++) gcd[i][j] = std::__gcd(i, j);
    int T;
    std::cin >> T;
    for (int i = 1; i <= T; i++) {
        std::cin >> K >> q[i].l >> q[i].r;
        q[i].ans = ans + i;
        vc[K].push_back(q + i);
    }
    for (int i = 1; i <= 100; i++) {
        if (vc[i].empty()) continue;
        memset(f, -1, sizeof(f));
        K = i;
        for (int j = 0; j < (int)vc[i].size(); j++)
            *vc[i][j]->ans = solve(vc[i][j]->r) - solve(vc[i][j]->l - 1);
    }
    for (int i = 1; i <= T; i++) std::cout << ans[i] << '\n';
    return 0;
}
