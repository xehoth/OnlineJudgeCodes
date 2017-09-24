/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1803」Query on a tree III 24-08-2017
 * dfs 序 + 可持久化线段树
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
namespace PersistentSegmentTree {

const int MAXN = 100000;
const int MAX_LOG = 20;

struct Node *null, *cur;

struct Node {
    Node *lc, *rc;
    int cnt;
    Node() : lc(null), rc(null), cnt(0) {}

    inline void *operator new(size_t) { return cur++; }
} pool[MAXN * MAX_LOG], *root[MAXN + 1];

inline void init() {
    cur = pool, null = new Node(), null->lc = null->rc = null;
    root[0] = null;
}

inline void insert(Node *&p, Node *x, int l, int r, int val) {
    p = new Node(*x), p->cnt++;
    if (l == r) return;
    register int mid = l + r >> 1;
    if (val <= mid)
        insert(p->lc, x->lc, l, mid, val);
    else
        insert(p->rc, x->rc, mid + 1, r, val);
}

inline int query(Node *p, Node *x, int l, int r, int k) {
    if (l == r) return l;
    register int mid = l + r >> 1;
    if (k <= x->lc->cnt - p->lc->cnt)
        return query(p->lc, x->lc, l, mid, k);
    else
        return query(p->rc, x->rc, mid + 1, r, k - x->lc->cnt + p->lc->cnt);
}

std::vector<int> edge[MAXN + 1];

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

bool vis[MAXN + 1];
int idx, pos[MAXN + 1], sz[MAXN + 1], label[MAXN + 1];
typedef std::vector<int>::iterator Iterator;

inline void dfs(const int u) {
    vis[u] = true, pos[u] = ++idx, label[idx] = u, sz[u] = 1;
    for (Iterator it = edge[u].begin(); it != edge[u].end(); it++)
        if (!vis[*it]) dfs(*it), sz[u] += sz[*it];
}

inline void solve() {
    using namespace IO;
    register int n;
    read(n);
    static int w[MAXN + 1], num[MAXN + 1], id[MAXN + 1];
    for (register int i = 1; i <= n; i++) read(w[i]), num[i] = w[i];
    std::sort(num + 1, num + n + 1);
    register int tot = std::unique(num + 1, num + n + 1) - num - 1;
    for (register int i = 1; i <= n; i++) {
        w[i] = std::lower_bound(num + 1, num + tot + 1, w[i]) - num;
        id[w[i]] = i;
    }
    init();
    for (register int i = 1, u, v; i < n; i++) read(u), read(v), addEdge(u, v);
    dfs(1);
    for (register int i = 1; i <= n; i++)
        insert(root[i], root[i - 1], 1, tot, w[label[i]]);
    register int q, x, k;
    for (read(q); q--;) {
        read(x), read(k);
        print(id[query(root[pos[x] - 1], root[pos[x] + sz[x] - 1], 1, tot, k)]);
        print('\n');
    }
}
}

int main() {
    PersistentSegmentTree::solve();
    IO::flush();
    return 0;
}