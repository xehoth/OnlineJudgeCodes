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
 * 「BZOJ 5333」荣誉称号 26-05-2018
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>

struct InputStream {
    char *s;
    InputStream() {
        int fd = fileno(stdin);
        struct stat sb;
        fstat(fd, &sb);
        s = (char *)mmap(0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    }

    template <typename T>
    inline InputStream &operator>>(T &x) {
        while (!isdigit(*s)) s++;
        for (x = 0; isdigit(*s); s++) x = x * 10 + (*s ^ 48);
        return *this;
    }
} io;

typedef unsigned int uint;

uint SA, SB, SC;

inline uint gen() {
    SA ^= SA << 16;
    SA ^= SA >> 5;
    SA ^= SA << 1;
    unsigned int t = SA;
    SA = SB;
    SB = SC;
    SC ^= t ^ SA;
    return SC;
}

const int MAXN = 10000000;
const int MAXM = 200 + 1;
const int MAXS = 4096 + 1;

int n, k, m, p, A, B, a[MAXN + 1], b[MAXN + 1], w;

long long f[MAXS][MAXM], id[MAXN + 1], val[MAXS][MAXM], s1[MAXS], s2[MAXS][MAXM];

inline void solve() {
    io >> n >> k >> m >> p >> SA >> SB >> SC >> A >> B;
    for (int i = 1; i <= p; i++) {
        io >> a[i] >> b[i];
        a[i] %= m;
    }
    for (int i = p + 1; i <= n; i++) {
        a[i] = gen() % A + 1;
        b[i] = gen() % B + 1;
        a[i] %= m;
    }
    w = 2 << k;
    n = std::max(n, w - 1);
    memset(val, 0, sizeof(val));
    memset(s1, 0, sizeof(s1));
    memset(s2, 0, sizeof(s2));
    for (int i = 1; i <= n; i++) {
        id[i] = (i < w ? i : id[i >> (k + 1)]);
        val[id[i]][0] += (a[i] ? b[i] * (m - a[i]) : 0);
        s1[id[i]] += b[i];
        s2[id[i]][a[i]] += m * b[i];
    }
    for (int i = 1; i < w; i++)
        for (int j = 1; j < m; j++) val[i][j] = val[i][j - 1] + s1[i] - s2[i][j];
    memset(f, 0x3f, sizeof(f));
    for (int i = w; i < w + w; i++) f[i][0] = 0;
    for (int i = w - 1; i > 0; i--) {
        for (int j = 0; j < m; j++) {
            for (int k = 0; k < m; k++) {
                f[i][j] = std::min(f[i][j], val[i][k] + f[i << 1][j - k < 0 ? j - k + m : j - k] +
                                                f[i << 1 | 1][j - k < 0 ? j - k + m : j - k]);
            }
        }
    }
    std::cout << f[1][0] << '\n';
}

int main() {
    int T;
    io >> T;
    while (T--) solve();
    return 0;
}