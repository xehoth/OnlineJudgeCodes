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
 * 「BZOJ 4538」网络 28-02-2018
 * 整体二分 + 树链求交
 * @author xehoth
 */
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

inline void *operator new(size_t size) {
    static const int POOL_SIZE = 1024 * 1024 * 3;
    static char pool[POOL_SIZE], *s = pool;
    char *t = s;
    s += size;
    return t;
}

inline void operator delete(void *) {}

inline void operator delete(void *, size_t) {}

struct InputOutputStream {
    enum { SIZE = 1 << 18 | 1 };

    char ibuf[SIZE], *s, *t, obuf[SIZE], *oh;

    InputOutputStream() : s(), t(), oh(obuf) {}

    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

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

const int MAXN = 100000 + 1;
const int MAXD = MAXN * 2 + 1;
const int MAX_LOG = 18;

std::vector<int> g[MAXN];

int min[MAX_LOG][MAXD], dfn[MAXN], dep[MAXN], idx, logs[MAXD], n, m;

void dfs(int u, int pre) {
    dfn[u] = ++idx;
    min[0][idx] = u;
    for (int i = 0, v; i < (int)g[u].size(); i++) {
        if ((v = g[u][i]) != pre) {
            dep[v] = dep[u] + 1;
            dfs(v, u);
            min[0][++idx] = u;
        }
    }
}

inline int upper(int u, int v) { return dep[u] < dep[v] ? u : v; }

inline int lower(int u, int v) { return dep[u] > dep[v] ? u : v; }

inline int lca(int u, int v) {
    static int bit;
    if ((u = dfn[u]) > (v = dfn[v])) std::swap(u, v);
    bit = logs[v - u + 1];
    return upper(min[bit][u], min[bit][v - (1 << bit) + 1]);
}

struct Path {
    int u, v, p;

    inline bool in(int o) const {
        return p == -1 ? false
                       : (p == 0 || (lca(o, p) == p &&
                                     (lca(u, o) == o || lca(v, o) == o)));
    }
};

inline Path operator&(const Path &A, const Path &B) {
    if (A.p == 0) return B;
    if (B.p == 0) return A;
    if (A.p == -1 || B.p == -1 || (!B.in(A.p) && !A.in(B.p)))
        return (Path){0, 0, -1};
    return A.p != B.p
               ? (Path){lower(lca(A.u, B.u), lca(A.v, B.v)),
                        lower(lca(A.u, B.v), lca(A.v, B.u)), lower(A.p, B.p)}
               : (Path){lower(lca(A.u, B.u), lca(A.u, B.v)),
                        lower(lca(A.v, B.u), lca(A.v, B.v)), A.p};
}

struct Query {
    int cmd, u, v, w;
} que[MAXD];

int num[MAXD], nCnt;

const int MAXM = 524288 + 1;

Path d[MAXM];

void insert(int k, int l, int r, int pos, const Path &p) {
    if (l == r) {
        d[k] = p;
        return;
    }
    int mid = (l + r) >> 1;
    pos <= mid ? insert(k << 1, l, mid, pos, p)
               : insert(k << 1 | 1, mid + 1, r, pos, p);
    d[k] = d[k << 1] & d[k << 1 | 1];
}

void erase(int k, int l, int r, int pos) {
    if (l == r) {
        d[k].p = 0;
        return;
    }
    int mid = (l + r) >> 1;
    pos <= mid ? erase(k << 1, l, mid, pos)
               : erase(k << 1 | 1, mid + 1, r, pos);
    d[k] = d[k << 1] & d[k << 1 | 1];
}

int query(int k, int l, int r, int pos) {
    if (l == r) return l;
    int mid = (l + r) >> 1;
    return d[k << 1 | 1].in(pos) ? query(k << 1, l, mid, pos)
                                 : query(k << 1 | 1, mid + 1, r, pos);
}

int main() {
    io >> n >> m;
    for (int i = 2, u, v; i <= n; i++) {
        io >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1, 0);
    for (int i = 2; i <= idx + 1; i++) logs[i] = logs[i >> 1] + 1;
    for (int w = 1; (1 << w) <= idx; w++)
        for (int i = 1; i + (1 << w) <= idx + 1; i++)
            min[w][i] = upper(min[w - 1][i], min[w - 1][i + (1 << (w - 1))]);
    for (int i = 1; i <= m; i++) {
        io >> que[i].cmd;
        if (que[i].cmd == 0) {
            io >> que[i].u >> que[i].v >> que[i].w;
            num[++nCnt] = que[i].w;
        } else {
            io >> que[i].u;
        }
    }
    std::sort(num + 1, num + nCnt + 1);
    nCnt = std::unique(num + 1, num + nCnt + 1) - num - 1;
    for (int i = 1; i <= m; i++) {
        switch (que[i].cmd) {
            case 0: {
                que[i].w =
                    std::lower_bound(num + 1, num + nCnt + 1, que[i].w) - num;
                insert(1, 1, nCnt, que[i].w,
                       (Path){que[i].u, que[i].v, lca(que[i].u, que[i].v)});
                break;
            }
            case 1: {
                erase(1, 1, nCnt, que[que[i].u].w);
                break;
            }
            case 2: {
                int x = d[1].in(que[i].u) ? -1 : query(1, 1, nCnt, que[i].u);
                io << (x == -1 ? -1 : num[x]) << '\n';

                break;
            }
        }
    }
    return 0;
}