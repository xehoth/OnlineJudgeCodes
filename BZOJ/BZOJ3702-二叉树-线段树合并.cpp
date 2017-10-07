/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3702」二叉树 07-10-2017
 * 线段树合并
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
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
}

const int OUT_LEN = 100000;

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

struct InputOutputStream {
    ~InputOutputStream() { flush(); }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }
} io;
}

#define long long long

namespace SegmentTree {

const int MAXN = 200000 * 2 * 10;

struct Node {
    Node *lc, *rc;
    int size;
    inline void *operator new(size_t);

    Node();
} pool[MAXN + 1], *cur = pool + 1, *null = pool;

Node::Node() : size(0) { lc = null, rc = null; }

inline void *Node::operator new(size_t) { return cur++; }

long ans1, ans2;

inline Node *merge(Node *u, Node *v) {
    if (u == null) return v;
    if (v == null) return u;
    ans1 += (long)u->lc->size * v->rc->size;
    ans2 += (long)u->rc->size * v->lc->size;
    u->lc = merge(u->lc, v->lc);
    u->rc = merge(u->rc, v->rc);
    u->size += v->size;
    return u;
}

inline void insert(Node *&p, int l, int r, int v) {
    if (p == null) p = new Node();
    p->size++;
    if (l == r) return;
    register int mid = l + r >> 1;
    v <= mid ? insert(p->lc, l, mid, v) : insert(p->rc, mid + 1, r, v);
}
}

namespace {

using SegmentTree::merge;
using SegmentTree::insert;
using SegmentTree::ans1;
using SegmentTree::ans2;
using IO::io;

const int MAXN = 200000 * 2;

struct Node {
    Node *lc, *rc;
    int v;
    SegmentTree::Node *root;

    Node();

    inline void *operator new(size_t);
} pool[MAXN + 1], *cur = pool + 1, *null, *root;

inline void *Node::operator new(size_t) { return cur++; }

Node::Node() : v(0) { lc = null, rc = null, root = SegmentTree::null; }

inline void build(Node *&p) {
    p = new Node(), io >> p->v;
    if (p->v == 0) build(p->lc), build(p->rc);
}

long ans;

inline void solve(Node *p) {
    if (p == null) return;
    solve(p->lc), solve(p->rc);
    if (p->v == 0) {
        ans1 = ans2 = 0, p->root = merge(p->lc->root, p->rc->root);
        ans += std::min(ans1, ans2);
    }
}

inline void solve() {
    register int n;
    io >> n;
    build(root);
    for (register Node *p = root; p != cur; p++)
        if (p->v) insert(p->root, 1, n, p->v);
    solve(root);
    io << ans;
}
}

#undef long

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    solve();
    return 0;
}