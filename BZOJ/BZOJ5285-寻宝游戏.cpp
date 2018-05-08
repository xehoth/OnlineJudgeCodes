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
 * 「BZOJ 5285」寻宝游戏 07-05-2018
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

struct InputStream {
    enum { SIZE = 1024 * 1024 };
    char ibuf[SIZE], *s, *t;
    inline char read() {
        if (s == t) t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin);
        return s == t ? -1 : *s++;
    }

    inline int read(char *buf) {
        int s = 0;
        char c;
        while (c = read(), isspace(c) && c != -1)
            ;
        if (c == -1) {
            *buf = '\0';
            return -1;
        }
        do
            buf[s++] = c;
        while (c = read(), !isspace(c) && c != -1);
        buf[s] = '\0';
        return s;
    }

    inline InputStream &operator>>(int &x) {
        static char buf[21];
        read(buf);
        x = atol(buf);
        return *this;
    }
} io;

const int MAXN = 1000 + 9;
const int MAXM = 5000 + 9;
const int MOD = 1000000007;

int n, m, q, a[MAXM], b[MAXM], *rk = a, *tmp = b, sum[MAXM];
int pw[MAXN], cnt[2];
char s[MAXM];

inline int add(int x, int v) { return x + v >= MOD ? x + v - MOD : x + v; }

inline int dec(int x, int v) { return x - v < 0 ? x - v + MOD : x - v; }

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    io >> n >> m >> q;
    for (register int i = 1; i <= m; i++) rk[i] = i;
    pw[0] = 1;
    for (register int i = 1; i <= n; i++) pw[i] = add(pw[i - 1], pw[i - 1]);
    for (register int i = 1; i <= n; i++) {
        io.read(s + 1);
        cnt[0] = cnt[1] = 0;
        for (register int j = 1; j <= m; j++) {
            if (s[j] == '1') sum[j] = add(sum[j], pw[i - 1]);
            cnt[s[j] ^ 48]++;
        }
        cnt[1] += cnt[0];
        for (register int j = m; j; j--) tmp[cnt[s[rk[j]] ^ 48]--] = rk[j];
        std::swap(rk, tmp);
    }
    rk[m + 1] = m + 1;
    sum[m + 1] = pw[n];
    for (int l, r; l = 0, r = m + 1, q--;) {
        io.read(s + 1);
        for (register int i = 1; i <= m; i++) {
            if (s[rk[i]] == '1') {
                r = i;
                break;
            }
        }
        for (register int i = m; i; i--) {
            if (s[rk[i]] == '0') {
                l = i;
                break;
            }
        }
        if (l >= r) {
            std::cout << "0\n";
        } else {
            std::cout << dec(sum[rk[r]], sum[rk[l]]) << '\n';
        }
    }

    return 0;
}