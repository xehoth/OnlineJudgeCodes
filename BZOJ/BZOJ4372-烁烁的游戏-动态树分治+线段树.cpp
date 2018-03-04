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
 * 「BZOJ 4372」烁烁的游戏 04-03-2018
 * 动态树分治 + 线段树
 * @author xehoth
 */
#include <cstdio>
#include <iostream>
#include <vector>

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
    inline void read(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        iosig && (x = -x);
    }

    inline void read(char &c) {
        while (c = read(), isspace(c) && c != -1)
            ;
    }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
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

const int MAXN = 100000 + 9;
const int MAX_LOG = 21;

std::vector<int> g[MAXN];
int sz[MAXN], dep[MAXN], dis[MAXN][MAX_LOG], fa[MAXN][MAX_LOG], n, m;
bool vis[MAXN];

void dfsSize(int u, int pre) {
    sz[u] = 1;
    for (int i = 0, v; i < (int)g[u].size(); i++) {
        if (!vis[v = g[u][i]] && v != pre) {
            dfsSize(v, u);
            sz[u] += sz[v];
        }
    }
}

int get(int u, int pre, int n) {
    for (int i = 0, v; i < (int)g[u].size(); i++)
        if (!vis[v = g[u][i]] && v != pre && sz[v] > n) return get(v, u, n);
    return u;
}

void dfs(int u, int pre, int d, int root) {
    for (int i = 0, v; i < (int)g[u].size(); i++) {
        if (!vis[v = g[u][i]] && v != pre) {
            dis[v][d] = dis[u][d] + 1;
            fa[v][d] = root;
            dfs(v, u, d, root);
        }
    }
}

char *cur;

struct Node *null;

struct Node {
    static const int NODE_SIZE;
    Node *lc, *rc;
    int sum;

    Node() : lc(null), rc(null) {}

    inline void *operator new(size_t) { return cur += NODE_SIZE; }
};

const int Node::NODE_SIZE = sizeof(Node);
const int MAXM = MAXN * MAX_LOG * 8;

char pool[MAXM * Node::NODE_SIZE];

class SegmentTree {
   public:
    int n;
    inline void init() { root = null; }

    void modify(int l, int r, int v) { modify(root, 0, n, l, r, v); }

    int query(int pos) { return query(root, 0, n, pos); }

   private:
    Node *root;

    void modify(Node *&p, int l, int r, int s, int t, int v) {
        if (p == null) p = new Node;
        if (s <= l && t >= r) {
            p->sum += v;
            return;
        }
        int mid = (l + r) >> 1;
        if (s <= mid) modify(p->lc, l, mid, s, t, v);
        if (t > mid) modify(p->rc, mid + 1, r, s, t, v);
    }

    int query(Node *p, int l, int r, int pos) {
        if (p == null || l == r) return p->sum;
        int mid = (l + r) >> 1;
        return p->sum + (pos <= mid ? query(p->lc, l, mid, pos)
                                    : query(p->rc, mid + 1, r, pos));
    }
} fs[MAXN], ds[MAXN];

void build(int u, int d) {
    dfsSize(u, 0);
    int size = sz[u];
    vis[u = get(u, 0, size / 2)] = true;
    dep[u] = d;
    fs[u].n = size;
    ds[u].n = size;
    for (int i = 0, v; i < (int)g[u].size(); i++) {
        if (!vis[v = g[u][i]]) {
            dis[v][d] = 1;
            fa[v][d] = u;
            dfs(v, u, d, u);
            build(v, d + 1);
        }
    }
}

inline void modify(int x, int k, int v) {
    ds[x].modify(0, k, v);
    for (int i = dep[x], d, u = x; i; i--) {
        d = k - dis[x][i];
        if (d >= 0) {
            ds[fa[x][i]].modify(0, d, v);
            fs[u].modify(0, d, v);
        }
        u = fa[x][i];
    }
}

inline int query(int x) {
    int ret = ds[x].query(0);
    for (int i = dep[x], d, u = x; i; i--) {
        d = dis[x][i];
        ret += ds[fa[x][i]].query(d) - fs[u].query(d);
        u = fa[x][i];
    }
    return ret;
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    cur = pool;
    null = (Node *)pool;
    null->lc = null->rc = null;
    io >> n >> m;
    for (int i = 1, u, v; i < n; i++) {
        io >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    build(1, 1);
    for (int i = 1; i <= n; i++) fa[i][dep[i]--] = i;
    for (int i = 0; i <= n; i++) {
        ds[i].init();
        fs[i].init();
    }
    for (char cmd; m--;) {
        io >> cmd;
        switch (cmd) {
            case 'Q': {
                int x;
                io >> x;
                io << query(x) << '\n';
                break;
            }
            case 'M': {
                int x, k, w;
                io >> x >> k >> w;
                modify(x, k, w);
                break;
            }
        }
    }
    return 0;
}