/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3217」ALOEXT 21-09-2017
 * 平衡树 + 线段树分裂合并
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
const int MAX_LOG = 20;

struct Node {
    Node *lc, *rc;
    int size;

    inline void *operator new(size_t);

    inline void operator delete(void *);

    inline void maintain();

    Node();
} pool[MAXN * MAX_LOG + 1], *null = pool, *cur = pool + 1,
                            *bin[MAXN * MAX_LOG + 1];
int binTop;

inline void *Node::operator new(size_t) {
    return binTop ? bin[--binTop] : cur++;
}

inline void Node::operator delete(void *p) { bin[binTop++] = (Node *)p; }

Node::Node() : lc(null), rc(null), size(0) {}

inline void Node::maintain() { size = lc->size + rc->size + 1; }

inline Node *merge(Node *u, Node *v) {
    if (u == null) return v;
    if (v == null) return u;
    u->lc = merge(u->lc, v->lc);
    u->rc = merge(u->rc, v->rc);
    u->size += v->size, delete v;
    return u;
}

inline void split(Node *p, Node *&u, int k) {
    u = new Node();
    if (k > p->lc->size)
        split(p->rc, u->rc, k - p->lc->size);
    else
        std::swap(p->rc, u->rc);
    if (k < p->lc->size) split(p->lc, u->lc, k);
    u->size = p->size - k, p->size = k;
}

inline int query(Node *p, int l, int r, int k) {
    if (l == r) return l;
    register int mid = l + r >> 1;
    return p->lc->size < k ? query(p->rc, mid + 1, r, k - p->lc->size)
                           : query(p->lc, l, mid, k);
}

inline void insert(Node *&p, int l, int r, int v) {
    p = new Node();
    p->size = 1;
    if (l == r) return;
    register int mid = l + r >> 1;
    v <= mid ? insert(p->lc, l, mid, v) : insert(p->rc, mid + 1, r, v);
}

bool type[MAXN + 1];
Node *root[MAXN + 1];
int end[MAXN + 1];
std::set<int> s;

inline void split(int x, int pos) {
    if (pos >= end[x] || pos < x) return;
    if (!type[x]) {
        split(root[x], root[pos + 1], pos - x + 1);
    } else {
        root[pos + 1] = root[x];
        split(root[pos + 1], root[x], end[x] - pos);
    }
    end[pos + 1] = end[x], end[x] = pos;
    s.insert(pos + 1), type[pos + 1] = type[x];
}
int n, q;

inline void merge(int a, int b) {
    if (a == b) return;
    s.erase(b);
    root[a] = merge(root[a], root[b]);
    end[a] = end[b];
}

inline int query(int x, int k) {
    k -= x - 1;
    if (!type[x])
        return query(root[x], 1, n, k);
    else
        return query(root[x], 1, n, end[x] - x + 2 - k);
}

int a[MAXN + 1];

inline void solve() {
    io >> n >> q;
    for (int i = 1; i <= n; i++) {
        io >> a[i];
        insert(root[i], 1, n, a[i]);
        s.insert(s.end(), i);
        end[i] = i;
    }
    static int tmp[MAXN], cnt = 0;
    for (register int cmd, l, r; q--;) {
        io >> cmd >> l >> r;
        split(*(--s.upper_bound(l)), l - 1);
        split(*(--s.upper_bound(r)), r);
        std::set<int>::iterator L = s.lower_bound(l), R = --s.upper_bound(r);
        register int pos = *L;
        if (L != R) {
            for (std::set<int>::iterator i = ++L;; i++) {
                tmp[++cnt] = *i;
                if (i == R) break;
            }
            for (register int i = 1; i <= cnt; i++) merge(pos, tmp[i]);
            cnt = 0;
        }
        type[pos] = cmd;
    }
    register int x;
    io >> x;
    io << query(*(--s.upper_bound(x)), x);
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}