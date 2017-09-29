/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3600」没有人的算术 29-09-2017
 * 线段树 + 替罪羊树 + 动态标号
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

namespace ScapeGoatTree {

const int MAXN = 500000;

struct Node *pos[MAXN + 1];
struct Data {
    Node *l, *r;

    Data(Node *l, Node *r) : l(l), r(r) {}

    Data();

    inline bool operator>(const Data &d) const;

    inline bool operator==(const Data &d) const { return l == d.l && r == d.r; }
};

inline int max(int a, int b) { return a > b ? a : b; }

const double ALPHA = 0.8;

struct Node {
    Node *c[2];
    Data d;
    double v;
    int size;

    Node();
    Node(const Data &, const double);

    inline void maintain() { size = c[0]->size + c[1]->size + 1; }

    inline bool check() { return max(c[0]->size, c[1]->size) > ALPHA * size; }

    inline void *operator new(size_t);
} pool[MAXN + 2], *null = pool, *cur = pool + 1;

inline void *Node::operator new(size_t) { return cur++; }

Node::Node() { c[0] = c[1] = null; }

Node::Node(const Data &d, const double v) : d(d), size(1), v(v) {
    c[0] = c[1] = null;
}

inline bool Data::operator>(const Data &d) const {
    return l->v > d.l->v || (l->v == d.l->v && r->v > d.r->v);
}

Data::Data() : l(null), r(null) {}

Node *root;
double badL, badR;

inline Node **insert(Node *&p, double l, double r, const Data &v, int id) {
    if (p == null) {
        pos[id] = p = new Node(v, (l + r) / 2);
        return &null;
    } else {
        if (p->d == v) {
            pos[id] = p;
            return &null;
        }
        p->size++;
        Node **res = (v > p->d ? insert(p->c[1], (l + r) / 2, r, v, id)
                               : insert(p->c[0], l, (l + r) / 2, v, id));
        if (p->check()) res = &p, badL = l, badR = r;
        return res;
    }
}

inline void travel(Node *p, std::vector<Node *> &v) {
    if (p == null) return;
    travel(p->c[0], v), v.push_back(p), travel(p->c[1], v);
}

inline Node *divide(std::vector<Node *> &v, int l, int r, double lv,
                    double rv) {
    if (l >= r) return null;
    register int mid = l + r >> 1;
    register double t = (lv + rv) / 2;
    Node *p = v[mid];
    p->v = t;
    p->c[0] = divide(v, l, mid, lv, t);
    p->c[1] = divide(v, mid + 1, r, t, rv);
    p->maintain();
    return p;
}

inline void rebuild(Node *&p) {
    static std::vector<Node *> v;
    v.clear(), travel(p, v), p = divide(v, 0, v.size(), badL, badR);
}

inline void insert(const Data &v, int id) {
    Node **p = insert(root, 0, 1, v, id);
    if (*p != null) rebuild(*p);
}

inline void init(const int n) {
    null->v = -1, pos[0] = null, root = new Node(Data(null, null), 0.5);
    for (register int i = 1; i <= n; i++) pos[i] = root;
}
}

namespace {

const int MAXN = 131072 * 2;
using IO::io;
using ScapeGoatTree::Data;
using ScapeGoatTree::pos;

int d[MAXN + 1], M;

inline void maintain(int k) {
    d[k] = (pos[d[k << 1]]->v >= pos[d[k << 1 | 1]]->v ? d[k << 1]
                                                       : d[k << 1 | 1]);
}

inline void build(const int n) {
    for (M = 1; M < n + 2; M <<= 1)
        ;
    for (register int i = 1; i <= n; i++) d[i + M] = i;
    for (register int i = M - 1; i; i--) maintain(i);
}

inline void modify(int k) {
    for (k += M; k;) maintain(k >>= 1);
}

inline int cmp(int a, int b) {
    return pos[a]->v > pos[b]->v ? a : (pos[a]->v == pos[b]->v ? (a < b ? a : b)
                                                               : b);
}

inline int query(int s, int t) {
    register int ret = 0;
    for (s = s + M - 1, t = t + M + 1; s ^ t ^ 1; s >>= 1, t >>= 1) {
        (~s & 1) ? (ret = cmp(d[s ^ 1], ret)) : 0;
        (t & 1) ? (ret = cmp(d[t ^ 1], ret)) : 0;
    }
    return ret;
}

inline void solve() {
    register int n, m;
    io >> n >> m;
    ScapeGoatTree::init(n);
    build(n);
    register char cmd;
    for (register int l, r, k; m--;) {
        io >> cmd >> l >> r;
        switch (cmd) {
            case 'Q':
                io << query(l, r) << '\n';
                break;
            case 'C':
                io >> k;
                ScapeGoatTree::insert(Data(pos[l], pos[r]), k);
                modify(k);
                break;
        }
    }
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}
