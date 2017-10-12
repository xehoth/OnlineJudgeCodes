/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4105」平方运算 12-10-2017
 * 线段树
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
inline bool read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return false;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
    return true;
}

inline void read(char &c) {
    while (c = read(), isspace(c) && c != -1)
        ;
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

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
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

    ~InputOutputStream() { flush(); }
} io;
}

namespace {

using IO::io;

const int MAXN = 100000;
const int MAX_MOD = 9977;
bool inCircle[MAX_MOD + 1];
int n, m, next[MAX_MOD + 1], mod, a[MAXN + 1];

inline int lcm(int a, int b) { return a / std::__gcd(a, b) * b; }

struct Node {
    Node *lc, *rc;
    int sum, len, pos, tag, circle[62];
    bool in;
    Node();

    inline void *operator new(size_t);

    inline void init(const int v) {
        sum = v;
        if ((in = inCircle[v])) {
            len = 1, circle[pos = 0] = v;
            for (register int u = next[v]; u != v; u = next[u])
                circle[len++] = u;
        }
    }

    inline void maintain() {
        sum = lc->sum + rc->sum;
        if ((in = lc->in & rc->in)) {
            len = lcm(lc->len, rc->len);
            register int tx = lc->pos, ty = rc->pos;
            for (register int i = 0; i < len; i++) {
                circle[i] = lc->circle[tx++] + rc->circle[ty++];
                (tx == lc->len) ? tx = 0 : 0;
                (ty == rc->len) ? ty = 0 : 0;
            }
            pos = 0;
        }
    }

    inline void pushDown() {
        if (tag) {
            lc->tag += tag, rc->tag += tag, lc->pos += tag, rc->pos += tag;
            (lc->pos >= lc->len) ? lc->pos %= lc->len : 0;
            lc->sum = lc->circle[lc->pos];
            (rc->pos >= rc->len) ? rc->pos %= rc->len : 0;
            rc->sum = rc->circle[rc->pos];
            tag = 0;
        }
    }
} pool[MAXN * 4 + 1], *null = pool, *cur = pool + 1, *root;

inline void *Node::operator new(size_t) { return cur++; }

Node::Node() : lc(null), rc(null) {}

void build(Node *&p, int l, int r) {
    p = new Node();
    if (l == r) {
        p->init(a[l]);
        return;
    }
    register int mid = l + r >> 1;
    build(p->lc, l, mid), build(p->rc, mid + 1, r), p->maintain();
}

void modify(Node *p, int l, int r, int s, int t) {
    if (l == s && t == r && p->in) {
        p->tag++, p->pos++;
        (p->pos == p->len) ? p->pos = 0 : 0;
        p->sum = p->circle[p->pos];
        return;
    }
    if (l == r) {
        p->init(next[p->sum]);
        return;
    }
    p->pushDown();
    register int mid = l + r >> 1;
    if (t <= mid)
        modify(p->lc, l, mid, s, t);
    else if (s > mid)
        modify(p->rc, mid + 1, r, s, t);
    else
        modify(p->lc, l, mid, s, mid), modify(p->rc, mid + 1, r, mid + 1, t);
    p->maintain();
}

int query(Node *p, int l, int r, int s, int t) {
    if (l == s && t == r) return p->sum;
    p->pushDown();
    register int mid = l + r >> 1;
    if (t <= mid)
        return query(p->lc, l, mid, s, t);
    else if (s > mid)
        return query(p->rc, mid + 1, r, s, t);
    return query(p->lc, l, mid, s, mid) + query(p->rc, mid + 1, r, mid + 1, t);
}

inline void init() {
    for (register int i = 1; i <= n; i++) io >> a[i];
    for (register int i = 0; i < mod; i++)
        next[i] = i * i % mod, inCircle[i] = true;
    static bool vis[MAX_MOD + 1];
    for (register int i = 0, y; i < mod; i++) {
        if (!vis[i]) {
            for (y = i; !vis[y];) vis[y] = true, y = next[y];
            for (register int j = i; j != y; j = next[j]) inCircle[j] = false;
        }
    }
}

inline void solve() {
    io >> n >> m >> mod;
    init();
    build(root, 1, n);
    for (register int cmd, l, r; m--;) {
        io >> cmd >> l >> r;
        switch (cmd) {
            case 0: {
                modify(root, 1, n, l, r);
                break;
            }
            case 1: {
                io << query(root, 1, n, l, r) << '\n';
                break;
            }
        }
    }
}
}

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    solve();
    return 0;
}