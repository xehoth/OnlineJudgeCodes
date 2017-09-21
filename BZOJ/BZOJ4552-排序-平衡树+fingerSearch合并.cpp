/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4552」排序 21-09-2017
 * 平衡树 + fingerSearch
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

typedef unsigned int uint;
inline uint nextUint() {
    static uint seed = 495;
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}

struct Node {
    Node *lc, *rc;
    int size, key;
    uint rank;

    inline void *operator new(size_t);

    inline void maintain() { size = lc->size + rc->size + 1; }

    Node();
    Node(int);
} pool[MAXN + 1], *null = pool, *cur = pool + 1;

inline void *Node::operator new(size_t) { return cur++; }

Node::Node() : lc(null), rc(null), size(0), rank(nextUint()), key(0) {}

Node::Node(int key) : lc(null), rc(null), size(1), rank(nextUint()), key(key) {}

inline Node *merge(Node *u, Node *v) {
    if (u == null) return v;
    if (v == null) return u;
    if (u->rank < v->rank)
        return u->rc = merge(u->rc, v), u->maintain(), u;
    else
        return v->lc = merge(u, v->lc), v->maintain(), v;
}

typedef std::pair<Node *, Node *> Pair;

inline int query(Node *p, int k) {
    for (; p != null;) {
        if (p->lc->size + 1 == k)
            return p->key;
        else if (p->lc->size >= k)
            p = p->lc;
        else
            k -= p->lc->size + 1, p = p->rc;
    }
}

inline Pair split(Node *u, int v) {
    if (u == null) return Pair(null, null);
    Pair t;
    if (u->key < v) {
        t = split(u->rc, v);
        u->rc = t.first, t.first = u;
    } else {
        t = split(u->lc, v);
        u->lc = t.second, t.second = u;
    }
    u->maintain();
    return t;
}

inline Node *fingerSearch(Node *u, Node *v) {
    if (u == null) return v;
    if (v == null) return u;
    if (u->rank > v->rank) std::swap(u, v);
    Pair t = split(v, u->key);
    u->lc = fingerSearch(u->lc, t.first);
    u->rc = fingerSearch(u->rc, t.second);
    u->maintain();
    return u;
}

bool type[MAXN + 1];
Node *root[MAXN + 1];
int end[MAXN + 1];
std::set<int> s;

inline void split(int x, int pos) {
    if (pos >= end[x] || pos < x) return;
    if (!type[x]) {
        Pair t = split(root[x], query(root[x], pos - x + 1) + 1);
        root[x] = t.first, root[pos + 1] = t.second;
    } else {
        root[pos + 1] = root[x];
        Pair t = split(root[pos + 1], query(root[pos + 1], end[x] - pos) + 1);
        root[pos + 1] = t.first, root[x] = t.second;
    }
    end[pos + 1] = end[x], end[x] = pos;
    s.insert(pos + 1), type[pos + 1] = type[x];
}

int n, q;

inline void merge(int a, int b) {
    if (a == b) return;
    s.erase(b);
    root[a] = fingerSearch(root[a], root[b]);
    end[a] = end[b];
}

inline int query(int x, int k) {
    k -= x - 1;
    if (!type[x])
        return query(root[x], k);
    else
        return query(root[x], end[x] - x + 2 - k);
}

int a[MAXN + 1];

std::vector<int> test(int a) {
    std::vector<int> t;
    for (register int i = 1; i <= root[a]->size; i++) {
        t.push_back(query(root[a], i));
    }
    return t;
}

inline void solve() {
    io >> n >> q;
    for (int i = 1; i <= n; i++) {
        io >> a[i];
        root[i] = new Node(a[i]);
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