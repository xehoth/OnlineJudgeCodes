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
 * 「CF 757G」Can Bash Save the Day? 05-03-2018
 * 可持久化点分树
 * @author xehoth
 */
#include <cstdio>
#include <iostream>
#include <vector>

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

const int MAXN = 200000 + 1;
const int MAXG = MAXN * 2 + 1;
const int MAX_LOG = 19;

int n, q;

struct Edge {
    int v, w;

    Edge(int v, int w) : v(v), w(w) {}
};

std::vector<Edge> edge[MAXN], g[MAXG];
int a[MAXN];
int tot;

inline void addEdge(int u, int v, int w) {
    g[u].emplace_back(v, w);
    g[v].emplace_back(u, w);
}

inline void binarize(int u, int pre) {
    static std::vector<Edge *> now;
    now.clear();
    for (auto &p : edge[u])
        if (p.v != pre) now.push_back(&p);
    int o = u;
    for (int i = 0; i < (int)now.size(); i++) {
        addEdge(o, now[i]->v, now[i]->w);
        if (now.size() - i > 2) {
            addEdge(o, ++tot, 0);
            o = tot;
        }
    }
    for (auto &p : edge[u])
        if (p.v != pre) binarize(p.v, u);
}

bool vis[MAXG];
int sz[MAXG], idx[MAXG][MAX_LOG], dep[MAXG];
long long dis[MAXG][MAX_LOG];

void dfsSize(int u, int pre) {
    sz[u] = 1;
    for (auto &p : g[u]) {
        if (!vis[p.v] && p.v != pre) {
            dfsSize(p.v, u);
            sz[u] += sz[p.v];
        }
    }
}

int get(int u, int pre, int n) {
    for (auto &p : g[u])
        if (!vis[p.v] && p.v != pre && sz[p.v] > n) return get(p.v, u, n);
    return u;
}

void dfs(int u, int pre, int d) {
    for (auto &p : g[u]) {
        if (!vis[p.v] && p.v != pre) {
            idx[p.v][d] = idx[u][d];
            dis[p.v][d] = dis[u][d] + p.w;
            dfs(p.v, u, d);
        }
    }
}

void build(int u, int d) {
    dfsSize(u, 0);
    vis[u = get(u, 0, sz[u] / 2)] = true;
    dep[u] = d;
    int c = 0;
    for (auto &p : g[u]) {
        if (!vis[p.v]) {
            dis[p.v][d] = p.w;
            idx[p.v][d] = c++;
            dfs(p.v, u, d);
            build(p.v, d + 1);
        }
    }
}

char *cur;

struct Node *null;

struct Node {
    static const int NODE_SIZE;
    Node *c[3];
    int sz;
    long long sum;

    inline void *operator new(size_t) { return cur += NODE_SIZE; }
};

const int Node::NODE_SIZE = sizeof(Node);

const int MAXM = MAXG * MAX_LOG * 2;

char pool[MAXM * Node::NODE_SIZE];

void insert(Node *&p, Node *pre, int u, int d) {
    p = new Node(*pre);
    p->sz++;
    p->sum += dis[u][d] - dis[u][d - 1];
    if (d != dep[u]) insert(p->c[idx[u][d]], pre->c[idx[u][d]], u, d + 1);
}

void erase(Node *&p, Node *pre, int u, int d) {
    p = new Node(*pre);
    p->sz--;
    p->sum -= dis[u][d] - dis[u][d - 1];
    if (d != dep[u]) erase(p->c[idx[u][d]], pre->c[idx[u][d]], u, d + 1);
}

long long query(Node *p, int u, int d) {
    return ((d == dep[u] || p == null) ? 0 : query(p->c[idx[u][d]], u, d + 1)) +
           p->sum + p->sz * (dis[u][d] - dis[u][d - 1]);
}

Node *root[MAXG];

int main() {
    // freopen("sample/1.in", "r", stdin);
    cur = pool;
    null = (Node *)pool;
    null->c[0] = null->c[1] = null->c[2] = null;
    io >> n >> q;
    for (int i = 1; i <= n; i++) io >> a[i];
    for (int i = 1, u, v, w; i < n; i++) {
        io >> u >> v >> w;
        edge[u].emplace_back(v, w);
        edge[v].emplace_back(u, w);
    }
    tot = n;
    binarize(1, 0);
    build(1, 1);
    for (int i = 0; i <= n; i++) root[i] = null;
    for (int i = 1; i <= n; i++) insert(root[i], root[i - 1], a[i], 1);
    long long lastans = 0;
    for (int cmd, l, r, u; q--;) {
        io >> cmd;
        switch (cmd) {
            case 1: {
                io >> l >> r >> u;
                l ^= lastans;
                r ^= lastans;
                u ^= lastans;
                lastans = query(root[r], u, 1) - query(root[l - 1], u, 1);
                io << lastans << '\n';
                lastans &= ((1 << 30) - 1);
                break;
            }
            case 2: {
                io >> u;
                u ^= lastans;
                erase(root[u], root[u], a[u], 1);
                std::swap(a[u], a[u + 1]);
                insert(root[u], root[u], a[u], 1);
                break;
            }
        }
    }
    return 0;
}
