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
 * 「BZOJ 4184」shallot 28-02-2018
 * 线段树分治 + 线性基
 * @author xehoth
 */
#include <bits/stdc++.h>

struct InputOutputStream {
    enum { SIZE = 1 << 18 | 1 };

    char ibuf[SIZE], *s, *t, obuf[SIZE], *oh;

    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    InputOutputStream() : s(), t(), oh(obuf) {}

    inline char read() {
        return (s == t) && (t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin)),
               s == t ? -1 : *s++;
    }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
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

    inline void print(char c) {
        (oh == obuf + SIZE) && (fwrite(obuf, 1, SIZE, stdout), oh = obuf);
        *oh++ = c;
    }

    template <typename T>
    inline void print(T x) {
        static int buf[21], cnt;
        if (x != 0) {
            (x < 0) && (print('-'), x = -x);
            for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
            while (cnt) print((char)buf[cnt--]);
        } else {
            print('0');
        }
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAX_LOG = 31;
const int MAXN = 500000;
const int MAXM = 1048576 + 1;

struct LinearBasis {
    unsigned int a[MAX_LOG];

    inline void insert(unsigned int x) {
        for (int i; x;) {
            if (a[i = MAX_LOG - __builtin_clz(x)]) {
                x ^= a[i];
            } else {
                a[i] = x;
                break;
            }
        }
    }

    inline unsigned int query() {
        unsigned int ret = 0;
        for (int i = MAX_LOG - 1; i >= 0; i--)
            if ((ret ^ a[i]) > ret) ret ^= a[i];
        return ret;
    }

    inline unsigned int &operator[](int i) { return a[i]; }

    inline const unsigned int &operator[](int i) const { return a[i]; }
} d[MAXM];

int M, n;

std::map<int, int> map;

inline void modify(int s, int t, int x) {
    for (s = s + M - 1, t = t + M + 1; s ^ t ^ 1; s >>= 1, t >>= 1) {
        if (~s & 1) d[s ^ 1].insert(x);
        if (t & 1) d[t ^ 1].insert(x);
    }
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    io >> n;
    for (M = 1; M < n + 2;) M <<= 1;
    for (int i = 1, x; i <= n; i++) {
        io >> x;
        if (x > 0) {
            map[x] = i;
        } else {
            std::map<int, int>::iterator it = map.find(-x);
            modify(it->second, i - 1, -x);
            map.erase(it);
        }
    }
    for (std::map<int, int>::iterator it = map.begin(); it != map.end(); ++it)
        modify(it->second, n, it->first);
    for (int i = 1; i < M; i++) {
        for (int j = 0; j < MAX_LOG; j++) {
            d[i << 1].insert(d[i][j]);
            d[i << 1 | 1].insert(d[i][j]);
        }
    }
    for (int i = 1; i <= n; i++) io << d[i + M].query() << '\n';
    return 0;
}