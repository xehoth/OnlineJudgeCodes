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
 * 「BZOJ 1758」重建计划 05-03-2018
 * 点分治
 * @author xehoth
 */
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

inline void *operator new(size_t size) {
    static const int POOL_SIZE = 1024 * 1024 * 15;
    static char pool[POOL_SIZE], *s = pool;
    char *t = s;
    s += size;
    return t;
}

inline void operator delete(void *) {}

inline void operator delete(void *, size_t) {}

struct InputStream {
    enum { SIZE = 1 << 18 | 1 };

    char ibuf[SIZE], *s, *t;

    inline char read() {
        return (s == t) && (t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin)),
               s == t ? -1 : *s++;
    }

    inline InputStream &operator>>(int &x) {
        static char c;
        for (c = read(); !isdigit(c); c = read())
            ;
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        return *this;
    }
} io;

const int MAXN = 100000 + 1;
const double EPS = 1e-4;
const double INF = 1e7;

struct Node {
    int v, w;

    Node(int v, int w) : v(v), w(w) {}
};

struct Data {
    int v, w, dep;

    inline bool operator<(const Data &p) const { return dep < p.dep; }
};

int n, L, U, sz[MAXN];

std::vector<Node> g[MAXN];
std::vector<Data> sub;
bool vis[MAXN];

typedef std::vector<Node>::iterator Iterator;

void dfsSize(int u, int pre) {
    sz[u] = 1;
    for (Iterator p = g[u].begin(); p != g[u].end(); ++p) {
        if (!vis[p->v] && p->v != pre) {
            dfsSize(p->v, u);
            sz[u] += sz[p->v];
        }
    }
}

int get(int u, int pre, int n) {
    for (Iterator p = g[u].begin(); p != g[u].end(); ++p)
        if (!vis[p->v] && p->v != pre && sz[p->v] > n) return get(p->v, u, n);
    return u;
}

int getMaxDep(int u, int pre, int d) {
    int max = d;
    for (Iterator p = g[u].begin(); p != g[u].end(); ++p)
        if (!vis[p->v] && p->v != pre)
            max = std::max(max, getMaxDep(p->v, u, d + 1));
    return max;
}

double f[MAXN], s[MAXN];
int tf, ts;

void dfs(int u, int pre, double mid, int d, double w) {
    if (d > U) return;
    if (ts < d + 1) {
        s[d] = w;
        ts = d + 1;
    } else {
        s[d] = std::max(s[d], w);
    }
    for (Iterator p = g[u].begin(); p != g[u].end(); ++p)
        if (!vis[p->v] && p->v != pre) dfs(p->v, u, mid, d + 1, w + p->w - mid);
}

inline bool check() {
    static int q[MAXN];
    int l = 0, r = 0;
    for (int i = ts - 1, j = 0; i >= 0; i--) {
        while (j < tf && j <= U - i) {
            while (l < r && f[q[r - 1]] <= f[j]) r--;
            q[r++] = j++;
        }
        while (l < r && q[l] < L - i) l++;
        if (l < r && f[q[l]] + s[i] >= 0) return true;
    }
    return false;
}

inline bool check(int u, double mid) {
    tf = 1;
    for (std::vector<Data>::iterator p = sub.begin(); p != sub.end(); ++p) {
        ts = 1;
        dfs(p->v, u, mid, 1, p->w - mid);
        if (check()) return true;
        for (int i = 1; i < tf; i++) f[i] = std::max(f[i], s[i]);
        for (int i = tf; i < ts; i++) f[i] = s[i];
        tf = ts;
    }
    return false;
}

void solve(int u, double &l, double tr) {
    dfsSize(u, 0);
    if (sz[u] - 1 < L) return;
    vis[u = get(u, 0, sz[u] / 2)] = true;
    sub.clear();
    Data tmp;
    for (Iterator p = g[u].begin(); p != g[u].end(); ++p) {
        if (!vis[p->v]) {
            tmp.dep = std::min(U, getMaxDep(p->v, u, 1));
            tmp.v = p->v;
            tmp.w = p->w;
            sub.push_back(tmp);
        }
    }
    std::sort(sub.begin(), sub.end());
    double r = tr;
    for (double mid; r - l > EPS;) {
        mid = (l + r) / 2;
        if (check(u, mid))
            l = mid;
        else
            r = mid;
    }
    for (Iterator p = g[u].begin(); p != g[u].end(); ++p)
        if (!vis[p->v]) solve(p->v, l, tr);
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    io >> n >> L >> U;
    double l = INF, r = 0;
    for (int i = 1, u, v, w; i < n; i++) {
        io >> u >> v >> w;
        g[u].push_back(Node(v, w));
        g[v].push_back(Node(u, w));
        l = std::min(l, (double)w);
        r = std::max(r, (double)w);
    }
    solve(1, l, r);
    printf("%.3f", l);
    return 0;
}