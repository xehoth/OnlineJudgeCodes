/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4034」树上操作 24-08-2017
 * 树链剖分 + dfs 序
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}

namespace HeavyLightChainDecomposition {

#define long long long

const int MAXN = 100000;
int n, m;

std::vector<int> edge[MAXN + 1];

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

typedef std::vector<int>::iterator Iterator;

int sz[MAXN + 1], dep[MAXN + 1], fa[MAXN + 1], idx;
int son[MAXN + 1], top[MAXN + 1], pos[MAXN + 1];
bool vis[MAXN + 1];

inline void dfs1(int u) {
    vis[u] = true, sz[u] = 1, dep[u] = dep[fa[u]] + 1;
    for (Iterator it = edge[u].begin(); it != edge[u].end(); it++) {
        if (!vis[*it]) {
            fa[*it] = u, dfs1(*it), sz[u] += sz[*it];
            sz[*it] > sz[son[u]] ? son[u] = *it : 0;
        }
    }
}

inline void dfs2(int u) {
    vis[u] = false, pos[u] = ++idx, top[u] = (u == son[fa[u]] ? top[fa[u]] : u);
    for (Iterator it = edge[u].begin(); it != edge[u].end(); it++)
        if (*it == son[u]) dfs2(*it);
    for (Iterator it = edge[u].begin(); it != edge[u].end(); it++)
        if (vis[*it]) dfs2(*it);
}

struct Node *null, *cur;

struct Node {
    Node *lc, *rc;
    int len;
    long sum, add;

    Node(int len = 0) : lc(null), rc(null), len(len), sum(0), add(0) {}

    inline void maintain() { sum = lc->sum + rc->sum; }

    inline void cover(const long add) {
        if (this == null) return;
        sum += len * add, this->add += add;
    }

    inline void pushDown() {
        if (add) lc->cover(add), rc->cover(add), add = 0;
    }
} pool[MAXN * 4 + 1], *root;

inline void build(Node *&p, int l, int r) {
    p = new Node(r - l + 1);
    if (l == r) return;
    register int mid = l + r >> 1;
    build(p->lc, l, mid), build(p->rc, mid + 1, r);
}

inline void modify(Node *p, int l, int r, int s, int t, int v) {
    if (s <= l && t >= r) {
        p->cover(v);
        return;
    }
    p->pushDown();
    register int mid = l + r >> 1;
    if (s <= mid) modify(p->lc, l, mid, s, t, v);
    if (t > mid) modify(p->rc, mid + 1, r, s, t, v);
    p->maintain();
}

inline long query(Node *p, int l, int r, int s, int t) {
    if (s <= l && t >= r) return p->sum;
    p->pushDown();
    register int mid = l + r >> 1;
    register long ret = 0;
    if (s <= mid) ret += query(p->lc, l, mid, s, t);
    if (t > mid) ret += query(p->rc, mid + 1, r, s, t);
    p->maintain();
    return ret;
}

inline void init() {
    cur = pool, null = new Node(), null->lc = null->rc = null;
}

inline void query(int x) {
    using namespace IO;
    register long ans = 0;
    while (top[x] != top[1]) {
        ans += query(root, 1, n, pos[top[x]], pos[x]);
        x = fa[top[x]];
    }
    print(ans + query(root, 1, n, pos[1], pos[x])), print('\n');
}

inline void solve() {
    using namespace IO;
    static int w[MAXN + 1];
    read(n), read(m);
    for (register int i = 1; i <= n; i++) read(w[i]);
    for (register int i = 1, u, v; i < n; i++) read(u), read(v), addEdge(u, v);
    dfs1(1), dfs2(1), init(), build(root, 1, n);
    for (register int i = 1; i <= n; i++)
        modify(root, 1, n, pos[i], pos[i], w[i]);
    for (register int cmd, x, a; m--;) {
        read(cmd);
        switch (cmd) {
            case 1:
                read(x), read(a);
                modify(root, 1, n, pos[x], pos[x], a);
                break;
            case 2:
                read(x), read(a);
                modify(root, 1, n, pos[x], pos[x] + sz[x] - 1, a);
                break;
            case 3:
                read(x), query(x);
                break;
        }
    }
}

#undef long
}

int main() {
    HeavyLightChainDecomposition::solve();
    IO::flush();
    return 0;
}