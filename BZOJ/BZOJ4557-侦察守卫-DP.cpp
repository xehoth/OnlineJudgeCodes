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
 * 「BZOJ 4557」侦察守卫 12-03-2018
 * DP
 * @author xehoth
 */
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 * 20 };

    char ibuf[SIZE], *s;

    InputOutputStream() : s(ibuf) { fread(ibuf, 1, SIZE, stdin); }

    inline InputOutputStream &operator>>(int &x) {
        static char c;
        for (c = *s++; !isdigit(c); c = *s++)
            if (c == -1) return *this;
        for (x = 0; isdigit(c); c = *s++) x = x * 10 + (c ^ '0');
        return *this;
    }
} io;

const int MAXN = 500000 + 1;
const int MAXD = 20 + 3;
const int INF = 0x3f3f3f3f;

std::vector<int> edge[MAXN];
bool mark[MAXN];

int n, m, d, w[MAXN], f[MAXN][MAXD], g[MAXN][MAXD];

void dp(int u, int pre) {
    if (mark[u]) f[u][0] = g[u][0] = w[u];
    std::fill_n(g[u] + 1, d, w[u]);
    g[u][d + 1] = INF;
    for (int i = 0, v; i < (int)edge[u].size(); i++) {
        if ((v = edge[u][i]) != pre) {
            dp(v, u);
            for (int j = d; j >= 0; j--)
                g[u][j] =
                    std::min(g[u][j] + f[v][j], g[v][j + 1] + f[u][j + 1]);
            for (int j = d; j >= 0; j--)
                g[u][j] = std::min(g[u][j], g[u][j + 1]);
            f[u][0] = g[u][0];
            for (int j = 1; j <= d + 1; j++) f[u][j] += f[v][j - 1];
            for (int j = 1; j <= d + 1; j++)
                f[u][j] = std::min(f[u][j], f[u][j - 1]);
        }
    }
}

int main() {
    io >> n >> d;
    for (int i = 1; i <= n; i++) io >> w[i];
    io >> m;
    for (int i = 0, u; i < m; i++) {
        io >> u;
        mark[u] = true;
    }
    for (int i = 1, u, v; i < n; i++) {
        io >> u >> v;
        edge[u].push_back(v);
        edge[v].push_back(u);
    }
    dp(1, 0);
    std::cout << f[1][0];
    return 0;
}
