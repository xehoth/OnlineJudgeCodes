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
 * 「BZOJ 4753」最佳团体 24-03-2018
 * 二分 + DP
 * @author xehoth
 */
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>

const double EPS = 1e-4;

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };
    char ibuf[SIZE], *s;

    InputOutputStream() : s(ibuf) {
#ifdef DBG
        freopen("sample/1.in", "r", stdin);
#endif
        fread(ibuf, 1, SIZE, stdin);
    }

    inline InputOutputStream &operator>>(int &x) {
        while (!isdigit(*s)) s++;
        for (x = 0; isdigit(*s); s++) x = x * 10 + (*s ^ '0');
        return *this;
    }
} io;

const int MAXN = 2500 + 5;

int n, k, s[MAXN], p[MAXN], sz[MAXN];
double val[MAXN], f[MAXN][MAXN];

std::vector<int> g[MAXN];

void dp(int u) {
    memset(f[u], 0xc2, sizeof(double) * (n + 1));
    if (u != 0) {
        f[u][1] = val[u];
        sz[u] = 1;
    } else {
        sz[0] = 0;
        f[0][0] = 0;
    }
    for (int i = 0, v; i < (int)g[u].size(); i++) {
        dp(v = g[u][i]);
        for (int i = std::min(k, sz[u]); i >= 0; i--)
            for (int j = 1; j <= sz[v]; j++)
                f[u][i + j] = std::max(f[u][i + j], f[u][i] + f[v][j]);
        sz[u] += sz[v];
    }
}

inline bool check(double mid) {
    for (int i = 1; i <= n; i++) val[i] = p[i] - mid * s[i];
    dp(0);
    return f[0][k] > -EPS;
}

int main() {
#ifdef DBG
    clock_t start = clock();
#endif
    io >> k >> n;
    for (int i = 1, fa; i <= n; i++) {
        io >> s[i] >> p[i] >> fa;
        g[fa].push_back(i);
    }
    double l = 1, r = 3;
    for (double mid; r - l > EPS;) {
        mid = (l + r) / 2;
        if (check(mid))
            l = mid;
        else
            r = mid;
    }
    std::cout << std::fixed << std::setprecision(3) << l;
#ifdef DBG
    clock_t end = clock();
    std::cerr << std::endl
              << "time :" << std::fixed << std::setprecision(3)
              << (end - start) / (double)CLOCKS_PER_SEC << "s" << std::endl;
#endif
    return 0;
}