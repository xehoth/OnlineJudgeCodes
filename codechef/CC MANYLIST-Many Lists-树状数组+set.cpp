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
 * 「CC MANYLIST」12-05-2018
 * 树状数组 + set
 * @author xehoth
 */
#include <bits/stdc++.h>

const int POOL_SIZE = 20 * 1024 * 1024;

char pool[POOL_SIZE];

void *operator new(size_t size) {
    static char *s = pool;
    char *t = s;
    s += size;
    return t;
}

void operator delete(void *) {}

void operator delete(void *, size_t) {}

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

const int MAXN = 250000 + 9;

int n, m;

struct BinaryIndexedTree {
    int d[MAXN], n;

    inline void add(int k, int v) {
        for (; k <= n; k += k & -k) d[k] += v;
    }

    inline void modify(int l, int r, int v) {
        add(l, v);
        add(r + 1, -v);
    }

    inline int query(int k) {
        int ret = 0;
        for (; k; k ^= k & -k) ret += d[k];
        return ret;
    }
} d;

std::map<int, int> range[MAXN];

inline void add(std::map<int, int> &map, int l, int r) {
    auto it = map.lower_bound(l);
    if (it != map.begin()) {
        --it;
        if (it->second >= r) return;
        if (it->second >= l) {
            l = it->first;
            d.modify(it->first, it->second, -1);
            map.erase(it);
        }
    }
    it = map.lower_bound(l);
    while (it != map.end() && it->first <= r) {
        r = std::max(it->second, r);
        d.modify(it->first, it->second, -1);
        it = map.erase(it);
    }
    map.insert(it, {l, r});
    d.modify(l, r, 1);
}

int main() {
    // freopen("test/in.in", "r", stdin);
    io >> n >> m;
    d.n = n;
    for (int cmd, l, r, v; m--;) {
        io >> cmd;
        switch (cmd) {
            case 0: {
                io >> l >> r >> v;
                add(range[v], l, r);
                break;
            }
            case 1: {
                io >> l;
                io << d.query(l) << '\n';
                break;
            }
        }
    }
    return 0;
}