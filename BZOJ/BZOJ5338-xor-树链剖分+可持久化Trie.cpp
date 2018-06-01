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
 * 「BZOJ 5338」xor 23-05-2018
 * 树链剖分 + 可持久化 Trie
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

const int MAX_LOG = 30;
const int MAXN = 100000;
const int POOL_SIZE = 1024 * 1024 * 128;

char pool[POOL_SIZE];

void *operator new(size_t size) {
    static char *s = pool;
    char *t = s;
    s += size;
    return t;
}

void operator delete(void *) {}

void operator delete(void *, size_t) {}

struct Node *null;

struct Node {
    Node *c[2];
    int size;

    Node() : size() { c[0] = c[1] = null; }
};

void insert(Node *&p, Node *pre, int v, int dep = MAX_LOG - 1) {
    p = new Node(*pre);
    p->size++;
    if (dep < 0) return;
    insert(p->c[(v >> dep) & 1], pre->c[(v >> dep) & 1], v, dep - 1);
}

int query(Node *l, Node *r, int v, int dep = MAX_LOG - 1) {
    if (dep < 0) return 0;
    int d = (v >> dep) & 1;
    if (r->c[d ^ 1]->size - l->c[d ^ 1]->size)
        return (1 << dep) | query(l->c[d ^ 1], r->c[d ^ 1], v, dep - 1);
    return query(l->c[d], r->c[d], v, dep - 1);
}

std::vector<int> g[MAXN + 1];
Node *root[MAXN + 1];
int sz[MAXN + 1], son[MAXN + 1], dep[MAXN + 1], fa[MAXN + 1];
int idx, top[MAXN + 1], pos[MAXN + 1], out[MAXN + 1], id[MAXN + 1];
bool vis[MAXN + 1];

void dfs1(int u) {
    sz[u] = 1;
    vis[u] = true;
    dep[u] = dep[fa[u]] + 1;
    for (int i = 0, v; i < (int)g[u].size(); i++) {
        if (!vis[v = g[u][i]]) {
            fa[v] = u;
            dfs1(v);
            sz[u] += sz[v];
            if (sz[v] > sz[son[u]]) son[u] = v;
        }
    }
}

void dfs2(int u) {
    vis[u] = false;
    pos[u] = ++idx;
    top[u] = (u == son[fa[u]] ? top[fa[u]] : u);
    id[idx] = u;
    if (son[u]) dfs2(son[u]);
    for (int i = 0, v; i < (int)g[u].size(); i++)
        if (vis[v = g[u][i]]) dfs2(v);
    out[u] = idx;
}

int n, q, val[MAXN + 1];

inline int query(int u, int v, int w) {
    int ret = 0;
    while (top[u] != top[v]) {
        if (dep[top[u]] < dep[top[v]]) std::swap(u, v);
        ret = std::max(ret, query(root[pos[top[u]] - 1], root[pos[u]], w));
        u = fa[top[u]];
    }
    if (dep[u] < dep[v]) std::swap(u, v);
    return std::max(ret, query(root[pos[v] - 1], root[pos[u]], w));
}

int main() {
    io >> n >> q;
    for (int i = 1; i <= n; i++) io >> val[i];
    for (int i = 1, u, v; i < n; i++) {
        io >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs1(1);
    dfs2(1);
    null = new Node;
    null->c[0] = null->c[1] = null;
    null->size = 0;
    root[0] = null;
    for (int i = 1; i <= n; i++) insert(root[i], root[i - 1], val[id[i]]);
    for (int cmd, u, v, w; q--;) {
        io >> cmd;
        switch (cmd) {
            case 1: {
                io >> u >> w;
                io << query(root[pos[u] - 1], root[out[u]], w) << '\n';
                break;
            }
            case 2: {
                io >> u >> v >> w;
                io << query(u, v, w) << '\n';
                break;
            }
        }
    }
    return 0;
}