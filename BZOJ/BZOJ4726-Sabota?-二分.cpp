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
 * 「BZOJ 4726」Sabota? 20-02-2018
 * 二分
 * @author xehoth
 */
#include <cctype>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <vector>

struct InputStream {
    enum { SIZE = 1 << 18 | 1 };
    char ibuf[SIZE], *s, *t;

    InputStream() : s(), t() {}

    inline char read() {
        return (s == t) && (t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin)),
               s == t ? -1 : *s++;
    }

    template <typename T>
    inline InputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        iosig && (x = -x);
        return *this;
    }
} io;

const double EPS = 1e-7;

std::vector<int> fa, sz, f;
int n, k;

inline bool check(double x) {
    f.assign(n + 1, 0);
    for (int i = n; i; i--) {
        if (sz[i] && f[i] > sz[i] * x) f[i] = sz[i] + 1;
        f[fa[i]] = std::max(f[fa[i]], f[i] = std::max(f[i], 1));
    }
    return f[1] <= k;
}

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    io >> n >> k;
    fa.resize(n + 1);
    sz.resize(n + 1, 1);
    sz[0] = 0;
    for (int i = 2; i <= n; i++) io >> fa[i];
    for (int i = n; i; i--) sz[fa[i]] += sz[i];
    for (int i = 1; i <= n; i++) sz[i]--;
    double l = 0, r = 1;
    for (double mid; r - l > EPS;) {
        mid = (l + r) / 2;
        if (check(mid))
            r = mid;
        else
            l = mid;
    }
    std::cout << std::fixed << std::setprecision(9) << r;
    return 0;
}