/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3878」奇怪的计算器 01-10-2017
 * 线段树
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

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = 0;
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = 0;
    return s;
}

inline void read(char &x) {
    while (x = read(), isspace(x) && x != -1)
        ;
}

const int OUT_LEN = 100000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, oh - obuf, stdout), oh = obuf) : 0;
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

    ~InputOutputStream() { flush(); }
} io;
}

namespace {

using IO::io;

#define long long long
const int MAXN = 100000;

int a[MAXN + 1], L, R, n;

struct Node {
    Node *lc, *rc;
    // kx + b
    long mul, k, b, min, max;

    Node();

    inline void *operator new(size_t);

    inline void maintain() {
        min = std::min(lc->min, rc->min);
        max = std::max(lc->max, rc->max);
    }

    inline void cover(int, int, long, long, long);

    inline void pushDown(int l, int r) {
        register int mid = l + r >> 1;
        lc->cover(l, mid, mul, k, b);
        rc->cover(mid + 1, r, mul, k, b);
        mul = 1, k = b = 0;
    }
} pool[MAXN * 4 + 1], *cur = pool + 1, *null = pool;

Node::Node()
    : lc(null), rc(null), mul(1), k(0), b(0), max(INT_MIN), min(INT_MAX) {}

inline void *Node::operator new(size_t) { return cur++; }

inline void Node::cover(int l, int r, long mul, long k, long b) {
    if (this == null) return;
    min = min * mul + k * a[l] + b;
    max = max * mul + k * a[r] + b;
    this->mul *= mul, this->k = this->k * mul + k, this->b = this->b * mul + b;
}

inline void build(Node *&p, int l, int r) {
    p = new Node();
    if (l == r) return (void)(p->max = p->min = a[l]);
    register int mid = l + r >> 1;
    build(p->lc, l, mid), build(p->rc, mid + 1, r), p->maintain();
}

inline void modifyL(Node *p, int l, int r) {
    if (l == r) return (void)(p->cover(l, r, 0, 0, L));
    p->pushDown(l, r);
    register int mid = l + r >> 1;
    if (p->rc->min < L)
        p->lc->cover(l, mid, 0, 0, L), modifyL(p->rc, mid + 1, r);
    else
        modifyL(p->lc, l, mid);
    p->min = p->lc->min, p->max = p->rc->max;
}

inline void modifyR(Node *p, int l, int r) {
    if (l == r) return (void)(p->cover(l, r, 0, 0, R));
    p->pushDown(l, r);
    register int mid = l + r >> 1;
    if (p->lc->max > R)
        p->rc->cover(mid + 1, r, 0, 0, R), modifyR(p->lc, l, mid);
    else
        modifyR(p->rc, mid + 1, r);
    p->min = p->lc->min, p->max = p->rc->max;
}

inline int query(Node *p, int l, int r, int x) {
    if (l == r) return p->max;
    register int mid = l + r >> 1;
    p->pushDown(l, r);
    return x <= mid ? query(p->lc, l, mid, x) : query(p->rc, mid + 1, r, x);
}

inline void solve() {
    io >> n >> L >> R;
    static int cmd[MAXN + 1][2];
    register char c;
    for (register int i = 1; i <= n; i++) {
        io >> c >> cmd[i][1];
        switch (c) {
            case '+':
                cmd[i][0] = 1;
                break;
            case '-':
                cmd[i][0] = 2;
                break;
            case '*':
                cmd[i][0] = 3;
                break;
            case '@':
                cmd[i][0] = 4;
                break;
        }
    }
    register int m;
    io >> m;
    static int q[MAXN + 1];
    for (register int i = 1; i <= m; i++) io >> a[i], q[i] = a[i];
    std::sort(a + 1, a + m + 1);
    register Node *root;
    build(root, 1, m);
    for (register int i = 1; i <= n; i++) {
        switch (cmd[i][0]) {
            case 1:
                root->cover(1, m, 1, 0, cmd[i][1]);
                break;
            case 2:
                root->cover(1, m, 1, 0, -cmd[i][1]);
                break;
            case 3:
                root->cover(1, m, cmd[i][1], 0, 0);
                break;
            case 4:
                root->cover(1, m, 1, cmd[i][1], 0);
                break;
        }
        if (root->min < L) modifyL(root, 1, m);
        if (root->max > R) modifyR(root, 1, m);
    }
    for (register int i = 1; i <= m; i++) {
        io << query(root, 1, m, std::lower_bound(a + 1, a + m + 1, q[i]) - a)
           << '\n';
    }
}

#undef long
}

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    solve();
    return 0;
}
