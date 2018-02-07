/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4756」Promotion Counting 24-08-2017
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
}  // namespace IO
namespace PersistentSegmentTree {

const int MAXN = 100000;
const int MAX_LOG = 20;

std::vector<int> edge[MAXN + 1];

typedef std::vector<int>::iterator Iterator;

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

struct Node *null, *cur;

struct Node {
    Node *lc, *rc;
    int cnt;

    Node() : lc(null), rc(null), cnt(0) {}

    inline void *operator new(size_t) { return cur++; }
} pool[MAXN * MAX_LOG], *root[MAXN + 1];

inline void init() {
    cur = pool, null = new Node(), null->lc = null->rc = null, root[0] = null;
}

inline void modify(Node *&p, Node *x, int l, int r, int val) {
    p = new Node(*x), p->cnt++;
    if (l == r) return;
    register int mid = l + r >> 1;
    if (val <= mid)
        modify(p->lc, x->lc, l, mid, val);
    else
        modify(p->rc, x->rc, mid + 1, r, val);
}

inline int query(Node *p, Node *x, int l, int r, int s, int t) {
    if (s <= l && t >= r) return x->cnt - p->cnt;
    register int mid = l + r >> 1, ret = 0;
    if (s <= mid) ret += query(p->lc, x->lc, l, mid, s, t);
    if (t > mid) ret += query(p->rc, x->rc, mid + 1, r, s, t);
    return ret;
}

int pos[MAXN + 1], idx, tot, p[MAXN + 1], sz[MAXN + 1];
bool vis[MAXN + 1];

inline void dfs(const int u) {
    vis[u] = true, pos[u] = ++idx, sz[u] = 1;
    modify(root[idx], root[idx - 1], 1, tot, p[u]);
    for (Iterator it = edge[u].begin(); it != edge[u].end(); it++) {
        if (!vis[*it]) {
            dfs(*it), sz[u] += sz[*it];
        }
    }
}

inline void solve() {
    using namespace IO;
    register int n;
    static int w[MAXN + 1];
    read(n);
    for (register int i = 1; i <= n; i++) read(p[i]), w[i] = p[i];
    std::sort(w + 1, w + n + 1), tot = std::unique(w + 1, w + n + 1) - w - 1;
    for (register int i = 1; i <= n; i++)
        p[i] = std::lower_bound(w + 1, w + n + 1, p[i]) - w;
    for (register int i = 2, fa; i <= n; i++) read(fa), addEdge(fa, i);
    init(), dfs(1);
    for (register int i = 1; i <= n; i++) {
        print(query(root[pos[i] - 1], root[pos[i] + sz[i] - 1], 1, tot,
                    p[i] + 1, tot)),
            print('\n');
    }
}
}  // namespace PersistentSegmentTree

int main() {
    PersistentSegmentTree::solve();
    IO::flush();
    return 0;
}