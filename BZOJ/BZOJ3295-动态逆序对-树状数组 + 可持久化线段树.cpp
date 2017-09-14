/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3295」动态逆序对 14-09-2017
 * 树状数组 + 可持久化线段树
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
const int MAXM = MAXN * 90 + 1;
#define long long long

struct Node {
    Node *lc, *rc;
    int cnt;

    Node();

    inline void *operator new(size_t);
} pool[MAXM + 1], *cur = pool + 1, *null = pool;

inline void *Node::operator new(size_t) { return cur++; }

Node::Node() : lc(null), rc(null), cnt(0) {}

struct PersistentSegmentTree {
    Node *root;

    PersistentSegmentTree() : root(null) {}

    inline void insert(Node *&p, int l, int r, int x) {
        if (p == null) p = new Node();
        p->cnt++;
        if (l == r) return;
        register int mid = l + r >> 1;
        if (x <= mid)
            insert(p->lc, l, mid, x);
        else
            insert(p->rc, mid + 1, r, x);
    }

    inline void erase(Node *&p, int l, int r, int x) {
        if (p == null) return;
        p->cnt--;
        if (l == r) return;
        register int mid = l + r >> 1;
        if (x <= mid)
            erase(p->lc, l, mid, x);
        else
            erase(p->rc, mid + 1, r, x);
    }

    inline int query(Node *p, int l, int r, int s, int t) {
        if (p == null) return 0;
        if (l == s && r == t) return p->cnt;
        register int mid = l + r >> 1;
        if (t <= mid)
            return query(p->lc, l, mid, s, t);
        else if (s > mid)
            return query(p->rc, mid + 1, r, s, t);
        else
            return query(p->lc, l, mid, s, mid) +
                   query(p->rc, mid + 1, r, mid + 1, t);
    }

    inline void insert(int l, int r, int x) { insert(root, l, r, x); }

    inline void erase(int l, int r, int v) { erase(root, l, r, v); }

    inline int query(int l, int r, int s, int t) {
        return query(root, l, r, s, t);
    }
};

struct BinaryIndexedTree {
#define long long long
    PersistentSegmentTree d[MAXN + 1];
    int n, m, pos[MAXN + 1];
    long ans;

    inline void insert(int k, int v) {
        for (; k <= n; k += k & -k) d[k].insert(1, n, v);
    }

    inline void erase(int k, int v) {
        for (; k <= n; k += k & -k) d[k].erase(1, n, v);
    }

    inline int query(int k, int l, int r) {
        register int ret = 0;
        for (; k; k ^= k & -k) ret += d[k].query(1, n, l, r);
        return ret;
    }

    inline void solve() {
        io >> n >> m;
        register long ans = 0;
        for (register int i = 1, x; i <= n; i++) {
            io >> x, pos[x] = i, insert(i, x);
            if (x != n) ans += query(i, x + 1, n);
        }
        for (register int i = 1, x; i <= m; i++) {
            io >> x, io << ans << '\n';
            if (x != n) ans -= query(pos[x], x + 1, n);
            if (x != 1) {
                ans -= query(n, 1, x - 1);
                ans += query(pos[x] - 1, 1, x - 1);
            }
            erase(pos[x], x);
        }
    }
#undef long
} task;
}

int main() {
    task.solve();
    return 0;
}