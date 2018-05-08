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
 * 「BZOJ 5286」转盘 07-05-2018
 * 线段树
 * @author xehoth
 */
#include <bits/stdc++.h>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };
    char ibuf[SIZE], *s, *t, obuf[SIZE], *oh;

    InputOutputStream() : s(), t(), oh(obuf) {}
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    inline char read() {
        if (s == t) t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin);
        return s == t ? -1 : *s++;
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
        if (iosig) x = -x;
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

const int MAXN = 100000 + 9;
const int MAXM = 262144 + 1;
const int INF = 0x3f3f3f3f;

struct Node {
    int max, ans;
} d[MAXM];

int n;

inline int query(int max, int k = 1, int l = 1, int r = n) {
    if (l == r) return d[k].max > max ? max + l : INF;
    int mid = (l + r) >> 1;
    return d[k << 1 | 1].max > max
               ? std::min(d[k].ans, query(max, k << 1 | 1, mid + 1, r))
               : query(max, k << 1, l, mid);
}

inline void maintain(int k, int l, int r) {
    d[k].max = std::max(d[k << 1].max, d[k << 1 | 1].max);
    d[k].ans = query(d[k << 1 | 1].max, k << 1, l, (l + r) >> 1);
}

void modify(int x, int v, int k = 1, int l = 1, int r = n) {
    if (l == r) return (void)(d[k].max = v - x);
    int mid = (l + r) >> 1;
    x <= mid ? modify(x, v, k << 1, l, mid)
             : modify(x, v, k << 1 | 1, mid + 1, r);
    maintain(k, l, r);
}

void build(int k, int l, int r) {
    if (l == r) {
        io >> d[k].max;
        d[k].max -= l;
        return;
    }
    int mid = (l + r) >> 1;
    build(k << 1, l, mid);
    build(k << 1 | 1, mid + 1, r);
    maintain(k, l, r);
}

int main() {
    int m, online, lastans;
    io >> n >> m >> online;
    build(1, 1, n);
    lastans = query(d[1].max - n) + n;
    io << lastans << '\n';
    for (int x, y; m--;) {
        io >> x >> y;
        if (online) {
            x ^= lastans;
            y ^= lastans;
        }
        modify(x, y);
        lastans = query(d[1].max - n) + n;
        io << lastans << '\n';
    }
    return 0;
}