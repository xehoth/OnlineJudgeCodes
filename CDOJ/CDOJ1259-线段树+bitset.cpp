/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「CDOJ 1259」昊昊爱运动 II 26-12-2017
 * 线段树 + bitset
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

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

inline void read(char &c) {
    while (c = read(), isspace(c) && c != -1)
        ;
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
}  // namespace IO

namespace {

const int MAXN = 100000 + 9;
const int MAXC = 100 + 1;

using IO::io;

struct Node {
    Node *lc, *rc;
    std::bitset<MAXC> c;
    int lazy;

    inline void maintain() { c = lc->c | rc->c; }

    inline void cover(int);

    inline void pushDown() {
        lazy ? (lc->cover(lazy), rc->cover(lazy), lazy = 0) : 0;
    }

    inline void *operator new(size_t);
};

const int NODE_SIZE = sizeof(Node);

char pool[MAXN * NODE_SIZE * 4], *cur = pool;

Node *null = (Node *)pool, *root;

inline void *Node::operator new(size_t) { return cur += NODE_SIZE; }

inline void Node::cover(int lazy) {
    if (this == null) return;
    c.reset(), c.set(lazy), this->lazy = lazy;
}

inline void build(Node *&p, int l, int r, int *a) {
    p = new Node, p->lc = null, p->rc = null;
    if (l == r) return (void)(p->lazy = 0, p->c.set(a[l]));
    register int mid = l + r >> 1;
    build(p->lc, l, mid, a), build(p->rc, mid + 1, r, a), p->maintain();
}

inline void modify(Node *p, int l, int r, int s, int t, int c) {
    if (s <= l && t >= r) return (void)(p->cover(c));
    p->pushDown();
    register int mid = l + r >> 1;
    if (s <= mid) modify(p->lc, l, mid, s, t, c);
    if (t > mid) modify(p->rc, mid + 1, r, s, t, c);
    p->maintain();
}

inline std::bitset<MAXC> query(Node *p, int l, int r, int s, int t) {
    if (s <= l && t >= r) return p->c;
    p->pushDown();
    register int mid = l + r >> 1;
    std::bitset<MAXC> ret;
    if (s <= mid) ret |= query(p->lc, l, mid, s, t);
    if (t > mid) ret |= query(p->rc, mid + 1, r, s, t);
    return ret;
}

int a[MAXN];

inline void solve() {
    null->lc = null, null->rc = null, null->lazy = 0, root = null;
    register int n, m, q;
    io >> n >> m;
    for (register int i = 1; i <= n; i++) io >> a[i];
    build(root, 1, n, a);
    io >> q;
    register char cmd;
    for (register int l, r, c; q--;) {
        io >> cmd >> l >> r;
        switch (cmd) {
            case 'M': {
                io >> c;
                modify(root, 1, n, l, r, c);
                break;
            }
            case 'Q': {
                io << query(root, 1, n, l, r).count() << '\n';
                break;
            }
        }
    }
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}