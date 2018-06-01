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
 * 「BZOJ 5334」数学计算 21-05-2018
 * 线段树
 * @author xehoth
 */
#include <bits/stdc++.h>
#include <sys/mman.h>
#include <sys/stat.h>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };
    char *buf, *p;
    int size;
    char *oh, obuf[SIZE];

    InputOutputStream() : oh(obuf) {
        int fd = fileno(stdin);
        struct stat sb;
        fstat(fd, &sb);
        size = sb.st_size;
        buf = (char *)mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
        p = buf;
    }
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    inline char read() { return *p++; }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        for (c = read(); !isdigit(c);) c = read();
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        return *this;
    }

    inline void print(char c) {
        if (oh == obuf + SIZE) {
            fwrite(obuf, 1, SIZE, stdout);
            oh = obuf;
        }
        *oh++ = c;
    }

    template <typename T>
    inline void print(T x) {
        static int buf[21], cnt;
        if (x != 0) {
            if (x < 0) {
                print('-');
                x = -x;
            }
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

const int MAXN = 100000;
const int MAXM = 131072 * 2 + 1;

int d[MAXM], mod, M;

inline void solve() {
    int n;
    io >> n >> mod;
    for (M = 1; M < n + 2;) M <<= 1;
    for (int i = 1; i <= M + M; i++) d[i] = 1;
    for (int cmd, m, i = 1; i <= n; i++) {
        io >> cmd >> m;
        switch (cmd) {
            case 1: {
                int k = i + M;
                d[k] = m;
                for (k >>= 1; k; k >>= 1) d[k] = (long long)d[k << 1] * d[k << 1 | 1] % mod;
                io << d[1] << '\n';
                break;
            }
            case 2: {
                int k = m + M;
                d[k] = 1;
                for (k >>= 1; k; k >>= 1) d[k] = (long long)d[k << 1] * d[k << 1 | 1] % mod;
                io << d[1] << '\n';
                break;
            }
        }
    }
}

int main() {
    // freopen("cal.in", "r", stdin);
    // freopen("cal.out", "w", stdout);
    int T;
    io >> T;
    while (T--) solve();
    return 0;
}
