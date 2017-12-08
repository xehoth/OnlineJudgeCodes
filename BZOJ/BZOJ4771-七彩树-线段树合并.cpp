/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4771」七彩树 08-12-2017
 * 线段树合并
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

inline char read() {
    static const int IN_LEN = 1 << 18 | 1;
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
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
}

const int OUT_LEN = 1 << 18 | 1;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[21], cnt;
    if (x == 0) {
        print('0');
    } else {
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
    ~InputOutputStream() { flush(); }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 100000 + 9;
const int MAX_LOG = 20;

char *cur;

struct Node *null;

struct Node {
    Node *lc, *rc;
    int val;
    static const int NODE_SIZE;

    Node() : lc(null), rc(null), val() {}

    inline void *operator new(size_t) { return cur += NODE_SIZE; }
};

const int Node::NODE_SIZE = sizeof(Node);

char pool[MAXN * MAX_LOG * 6 * Node::NODE_SIZE];

int n, m;

namespace PersistentSegmentTree {

Node *root[MAXN];

inline void insert(Node *&p, int l, int r, int pos, int v) {
    p = new Node(*p), p->val += v;
    if (l == r) return;
    register int mid = (l + r) >> 1;
    pos <= mid ? insert(p->lc, l, mid, pos, v)
               : insert(p->rc, mid + 1, r, pos, v);
}

inline Node *merge(Node *u, Node *v) {
    u = (u != null ? new Node(*u) : null);
    v = (v != null ? new Node(*v) : null);
    if (u == null) return v;
    if (v == null) return u;
    if (u->lc != null || u->rc != null) {
        u->lc = merge(u->lc, v->lc);
        u->rc = merge(u->rc, v->rc);
    }
    u->val += v->val;
    return u;
}

inline int query(Node *p, int l, int r, int s, int t) {
    if (s <= l && t >= r) return p->val;
    register int mid = (l + r) >> 1, ret = 0;
    if (s <= mid) ret += query(p->lc, l, mid, s, t);
    if (t > mid) ret += query(p->rc, mid + 1, r, s, t);
    return ret;
}

inline void modify(int p, int pos, int v) { insert(root[p], 1, n, pos, v); }

inline int query(int p, int d) { return query(root[p], 1, n, 1, d); }
}  // namespace PersistentSegmentTree

int dep[MAXN], fa[MAXN], color[MAXN];

namespace SegmentTree {

Node *root[MAXN];

inline void insert(Node *&p, int l, int r, int pos, int v) {
    if (p == null) p = new Node();
    if (l == r) return (void)(p->val = v);
    register int mid = (l + r) >> 1;
    pos <= mid ? insert(p->lc, l, mid, pos, v)
               : insert(p->rc, mid + 1, r, pos, v);
}

inline Node *merge(Node *u, Node *v, int pos) {
    if (u == null) return v;
    if (v == null) return u;
    if (u->lc != null || u->rc != null) {
        u->lc = merge(u->lc, v->lc, pos);
        u->rc = merge(u->rc, v->rc, pos);
    } else {
        PersistentSegmentTree::modify(pos, std::max(u->val, v->val), -1);
        u->val = std::min(u->val, v->val);
    }
    return u;
}

inline void insert(int p, int c) {
    insert(root[p], 1, n, c, dep[p]);
    PersistentSegmentTree::modify(p, dep[p], 1);
}

inline void merge(int a, int b) {
    PersistentSegmentTree::root[a] = PersistentSegmentTree::merge(
        PersistentSegmentTree::root[a], PersistentSegmentTree::root[b]);
    root[a] = merge(root[a], root[b], a);
}
}  // namespace SegmentTree

inline void solveCase() {
    cur = pool;
    io >> n >> m;
    for (register int i = 0; i <= n; i++)
        PersistentSegmentTree::root[i] = SegmentTree::root[i] = null;
    dep[1] = 1;
    for (register int i = 1; i <= n; i++) io >> color[i];
    for (register int i = 2; i <= n; i++) io >> fa[i], dep[i] = dep[fa[i]] + 1;
    for (register int i = 1; i <= n; i++) SegmentTree::insert(i, color[i]);
    for (register int i = n; i > 1; i--) SegmentTree::merge(fa[i], i);

    for (register int i = 1, x, d, last = 0; i <= m; i++) {
        io >> x, x ^= last;
        io >> d, d ^= last;
        last = PersistentSegmentTree::query(x, dep[x] + d);
        io << last << '\n';
    }
}

inline void solve() {
    null = (Node *)pool, null->lc = null->rc = null;
    register int T;
    io >> T;
    while (T--) solveCase();
}
}  // namespace

int main() {
    //	freopen("in.in", "r", stdin);
    solve();
    return 0;
}
