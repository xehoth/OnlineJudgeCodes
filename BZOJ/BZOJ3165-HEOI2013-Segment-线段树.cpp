/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3165」Segment 24-11-2017
 * 线段树
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

inline char read() {
    static const int IN_LEN = 100000;
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

const int MAXN = 100000 * 4 + 10;

struct Line {
    double k, b;
    int id;

    inline double v(const int x) const { return k * x + b; }
};

struct Node {
    Node *lc, *rc;
    Line d;

    Node();

    inline void *operator new(size_t);
};

const int NODE_SIZE = sizeof(Node);

char pool[MAXN * NODE_SIZE], *cur = pool;

Node *null = (Node *)pool;

inline void *Node::operator new(size_t) { return cur += NODE_SIZE; }

Node::Node() : lc(null), rc(null), d() {}

Node *root = null;

inline bool cmp(const Line &a, const Line &b, const int x) {
    return a.v(x) > b.v(x) || (a.v(x) == b.v(x) && a.id < b.id);
}

inline void fix(Node *p, int l, int r, Line d) {
    if (p == null) return;
    if (p->d.id == 0) return (void)(p->d = d);
    if (cmp(d, p->d, l)) std::swap(d, p->d);
    if (p->d.v(r) > d.v(r)) return;
    register int mid = (l + r) >> 1;
    register double x = (p->d.b - d.b) / (d.k - p->d.k);
    if (x <= mid)
        fix(p->lc, l, mid, p->d), p->d = d;
    else
        fix(p->rc, mid + 1, r, d);
}

inline void insert(Node *&p, int l, int r, int s, int t, const Line &d) {
    if (s <= l && t >= r) return (void)(fix(p, l, r, d));
    register int mid = (l + r) >> 1;
    if (s <= mid) insert(p->lc, l, mid, s, t, d);
    if (t > mid) insert(p->rc, mid + 1, r, s, t, d);
}

inline Line *query(Node *p, int l, int r, int x) {
    if (l == r) return &p->d;
    register int mid = (l + r) >> 1;
    Line *ret;
    if (x <= mid)
        ret = query(p->lc, l, mid, x);
    else
        ret = query(p->rc, mid + 1, r, x);
    if (cmp(p->d, *ret, x)) return &p->d;
    return ret;
}

inline void build(Node *&p, int l, int r) {
    p = new Node();
    if (l == r) return;
    register int mid = l + r >> 1;
    build(p->lc, l, mid), build(p->rc, mid + 1, r);
}

const int INF = 0x3f3f3f;
const int n = 40000;

inline void solve() {
    null->lc = null->rc = null;
    build(root, 1, n);
    register int q;
    io >> q;
    register int lastans = 0, tot = 0;
    for (register int i = 1, cmd; i <= q; i++) {
        io >> cmd;
        switch (cmd) {
            case 0: {
                register int x;
                io >> x;
                x = (x + lastans - 1) % 39989 + 1;
                lastans = query(root, 1, n, x)->id;
                io << lastans << '\n';
                break;
            }
            case 1: {
                register int x1, y1, x0, y0;
                io >> x0 >> y0 >> x1 >> y1;
                x0 = (x0 + lastans - 1) % 39989 + 1;
                x1 = (x1 + lastans - 1) % 39989 + 1;
                y0 = (y0 + lastans - 1) % 1000000000 + 1;
                y1 = (y1 + lastans - 1) % 1000000000 + 1;
                if (x0 > x1) std::swap(x0, x1), std::swap(y0, y1);
                Line tmp;
                if (x0 == x1) {
                    tmp.k = -INF, tmp.b = std::max(y1, y0);
                } else {
                    tmp.k = 1.0 * (y1 - y0) / (1.0 * (x1 - x0));
                    tmp.b = 1.0 * y0 - 1.0 * x0 * tmp.k;
                }
                tmp.id = ++tot;
                insert(root, 1, n, x0, x1, tmp);
                break;
            }
        }
    }
}
}  // namespace

int main() {
    solve();
    return 0;
}