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
 * 「LOJ 6035」洗衣服 31-05-2018
 * 贪心
 * @author xehoth
 */
#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>

const int MAXN = 100000;
const int MAXL = 1000000;

struct InputStream {
    char *s;

    InputStream() {
        int fd = fileno(stdin);
        struct stat sb;
        fstat(fd, &sb);
        s = (char *)mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    }

    InputStream &operator>>(int &x) {
        while (!isdigit(*s)) s++;
        for (x = 0; isdigit(*s); s++) x = x * 10 + (*s ^ 48);
        return *this;
    }
} io;

using Pair = std::pair<long long, int>;

struct PriorityQueue : public std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair> > {
    using super = std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair> >;

    void reserve(int n) { c.reserve(n); }

    void init() { std::make_heap(c.begin(), c.end(), comp); }

    void emplace_back(long long first, int second) { c.emplace_back(first, second); }
} qw, qd;

int w[MAXN + 1], d[MAXN + 1], l, n, m;
long long f[MAXL + 1], g[MAXL + 1], ans;

int main() {
    io >> l >> n >> m;
    qw.reserve(n + l / 2 + 9);
    qd.reserve(m + l / 2 + 9);
    for (int i = 1; i <= n; i++) {
        io >> w[i];
        qw.emplace_back(w[i], i);
    }
    for (int i = 1; i <= m; i++) {
        io >> d[i];
        qd.emplace_back(d[i], i);
    }
    qw.init();
    qd.init();
    long long fw, gw;
    for (int i = 1; i <= l; i++) {
        std::tie(f[i], fw) = qw.top();
        qw.pop();
        qw.emplace(f[i] + w[fw], fw);
        std::tie(g[i], gw) = qd.top();
        qd.pop();
        qd.emplace(g[i] + d[gw], gw);
    }
    for (int i = 1; i <= l; i++) ans = std::max(f[i] + g[l - i + 1], ans);
    std::cout << ans;
    return 0;
}
