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
 * 「BZOJ 4553」序列 22-03-2018
 * CDQ 分治
 * @author xehoth
 */
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 * 2 };
    char ibuf[SIZE], *s;

    InputOutputStream() : s(ibuf) { fread(ibuf, 1, SIZE, stdin); }

    inline InputOutputStream &operator>>(int &x) {
        while (!isdigit(*s)) s++;
        for (x = 0; isdigit(*s); s++) x = x * 10 + (*s ^ '0');
        return *this;
    }
} io;

const int MAXN = 100000 + 1;

int n, m, a[MAXN], min[MAXN], max[MAXN], id[MAXN], f[MAXN], ans, d[MAXN];

inline bool cmpA(int i, int j) { return a[i] < a[j]; }

inline bool cmpMax(int i, int j) { return max[i] < max[j]; }

inline void clear(int k) {
    for (; k <= n; k += k & -k) d[k] = 0;
}

inline void add(int k, int v) {
    for (; k <= n; k += k & -k) d[k] = std::max(d[k], v);
}

inline int query(int k) {
    int ret = 0;
    for (; k; k ^= k & -k) ret = std::max(ret, d[k]);
    return ret;
}

void solve(int l, int r) {
    if (l == r) {
        f[1] = std::max(f[1], 1);
        ans = std::max(ans, f[l]);
        return;
    }
    int mid = (l + r) >> 1;
    solve(l, mid);
    for (int i = l; i <= r; i++) id[i] = i;
    std::sort(id + l, id + mid + 1, cmpMax);
    std::sort(id + mid + 1, id + r + 1, cmpA);
    for (int i = mid + 1, k = l - 1; i <= r; i++) {
        while (k < mid && max[id[k + 1]] <= a[id[i]]) {
            k++;
            add(a[id[k]], f[id[k]]);
        }
        f[id[i]] = std::max(f[id[i]], query(min[id[i]]) + 1);
    }
    for (int i = l; i <= mid; i++) clear(a[id[i]]);
    solve(mid + 1, r);
}

int main() {
    io >> n >> m;
    for (int i = 1; i <= n; i++) {
        io >> a[i];
        min[i] = max[i] = a[i];
    }
    for (int i = 1, x, y; i <= m; i++) {
        io >> x >> y;
        min[x] = std::min(min[x], y);
        max[x] = std::max(max[x], y);
    }
    solve(1, n);
    std::cout << ans;
    return 0;
}