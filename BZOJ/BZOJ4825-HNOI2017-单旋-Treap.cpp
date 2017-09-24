/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4825」单旋 23-09-2017
 * Treap
 * @author xehoth
 */
#define NDEBUG
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

namespace Splay {

const int MAXN = 100000;

struct Node {
    Node *fa, *lc, *rc;
    int key;

    Node();
    Node(int);

    inline void *operator new(size_t);
} pool[MAXN + 3], *cur = pool + 1, *null = pool;

Node::Node() : fa(null), lc(null), rc(null), key(0) {}

Node::Node(int key) : fa(null), lc(null), rc(null), key(key) {}

inline void *Node::operator new(size_t) { return cur++; }
}

namespace Treap {

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
    int key, tag, dep, size;
    uint rank;
    Splay::Node *splay;

    inline void maintain() { size = lc->size + rc->size + 1; }

    inline void add(int x) { tag += x, dep += x; }

    inline void pushDown() { tag ? (lc->add(tag), rc->add(tag), tag = 0) : 0; }

    inline void *operator new(size_t);

    Node();

    Node(int, int, Splay::Node *);

} pool[MAXN + 3], *cur = pool + 1, *null = pool;

Node::Node()
    : key(0),
      tag(0),
      dep(0),
      size(0),
      rank(nextUint()),
      splay(Splay::null),
      lc(null),
      rc(null) {}

Node::Node(int key, int dep, Splay::Node *splay)
    : key(key),
      dep(dep),
      splay(splay),
      size(1),
      rank(nextUint()),
      tag(0),
      lc(null),
      rc(null) {}

inline void *Node::operator new(size_t) { return cur++; }

inline int lowerRank(Node *p, int key) {
    register int ret = 0;
    for (; p != null;)
        p->key >= key ? p = p->lc : (ret += p->lc->size + 1, p = p->rc);
    return ret;
}

inline int upperRank(Node *p, int key) {
    register int ret = 0;
    for (; p != null;)
        p->key > key ? p = p->lc : (ret += p->lc->size + 1, p = p->rc);
    return ret;
}

inline Node *select(Node *p, int k) {
    for (; p->pushDown(), p->lc->size + 1 != k;)
        p->lc->size >= k ? p = p->lc : (k -= p->lc->size + 1, p = p->rc);
    return p;
}

inline Node *merge(Node *u, Node *v) {
    if (u == null) return v;
    if (v == null) return u;
    return u->rank < v->rank
               ? (u->pushDown(), u->rc = merge(u->rc, v), u->maintain(), u)
               : (v->pushDown(), v->lc = merge(u, v->lc), v->maintain(), v);
}

inline void split(Node *u, int k, Node *&l, Node *&r) {
    if (u == null) return (void)(l = r = null);
    u->pushDown();
    if (u->lc->size >= k)
        split(u->lc, k, l, r), u->lc = r, r = u;
    else
        split(u->rc, k - u->lc->size - 1, l, r), u->rc = l, l = u;
    u->maintain();
}

#ifdef DBG
inline void printTreap(Node *p) {
    if (p == null) return;
    p->pushDown();
    printTreap(p->lc);
    std::cerr << "(key, dep) = (" << p->key << ", " << p->dep << ")"
              << std::endl;
    printTreap(p->rc);
}
#endif

struct Treap {
    Node *root;

    Treap() : root(null) {}

    inline void insert(int key, int dep, Splay::Node *splay) {
        register Node *l, *r;
        split(root, lowerRank(root, key), l, r);
        root = merge(merge(l, new Node(key, dep, splay)), r);
    }

    inline void add(int lVal, int rVal, int val) {
        register Node *l, *r, *tmp, *t;
        split(root, lowerRank(root, lVal), l, tmp);
        if (tmp == null || select(tmp, 1)->key > rVal) {
            root = merge(l, tmp);
            return;
        }
        split(tmp, upperRank(tmp, rVal), t, r);
        // lVal -> rVal
        t->add(val), root = merge(merge(l, t), r);
    }

    inline Node *precursor(int k) {
        return (k = lowerRank(root, k)) == 0 ? NULL : select(root, k);
    }

    inline Node *successor(int k) {
        return (k = upperRank(root, k)) == root->size ? NULL
                                                      : select(root, k + 1);
    }

    inline Node *min() {
        register Node *p = root;
        for (; p->pushDown(), p->lc != null; p = p->lc)
            ;
        return p;
    }

    inline Node *max() {
        register Node *p = root;
        for (; p->pushDown(), p->rc != null; p = p->rc)
            ;
        return p;
    }

    inline void extractMin() {
        register Node *l, *r;
        split(root, 1, l, r), root = r;
    }

    inline void extractMax() {
        register Node *l, *r;
        split(root, root->size - 1, l, r), root = l;
    }
};
}

namespace {

const int INF = 0x3f3f3f3f;

Splay::Node *root = Splay::null;

Treap::Treap treap;

#ifdef DBG
struct NullPointerException {
    NullPointerException() {
        std::cerr << "NullPointerException" << std::endl;
        assert(false);
    }
};
#endif

inline int insert(int key) {
    Splay::Node *v = new Splay::Node(key);
    if (root == Splay::null) {
        root = v, treap.insert(key, 1, v);
        return 1;
    } else {
        Treap::Node *precursor = treap.precursor(key),
                    *successor = treap.successor(key);
#ifdef DBG
        if (precursor == NULL && successor == NULL)
            throw NullPointerException();
#endif
        if (precursor == NULL || precursor->splay->rc != Splay::null) {
            successor->splay->lc = v, v->fa = successor->splay;
            treap.insert(key, successor->dep + 1, v);
            return successor->dep + 1;
        } else if (successor == NULL || successor->splay->lc != Splay::null) {
            precursor->splay->rc = v, v->fa = precursor->splay;
            treap.insert(key, precursor->dep + 1, v);
            return precursor->dep + 1;
        }
    }
#ifdef DBG
    throw;
#endif
}

inline int splayMin() {
#ifdef DBG
    if (root == Splay::null) throw NullPointerException();
#endif
    Treap::Node *minNode = treap.min();
    register int dep = minNode->dep;
    Splay::Node *v = minNode->splay;
    if (v->fa != Splay::null) {
        // all
        treap.add(1, INF, 1);
        // -this
        treap.add(v->key, v->key, -dep);
        // this ~ fa
        treap.add(v->key + 1, v->fa->key - 1, -1);
#ifdef DBG
#ifdef PRINT_TAG_ADD
        std::cerr << "add(" << v->key + 1 << ", " << v->fa->key - 1 << ", -1)";
        std::endl(std::cerr);
#endif
#endif
        if (v->rc != Splay::null) v->rc->fa = v->fa;
        v->fa->lc = v->rc;
        v->fa = Splay::null, root->fa = v, v->rc = root;
        root = v;
    }
    return dep;
}

inline int splayMax() {
#ifdef DBG
    if (root == Splay::null) throw NullPointerException();
#endif
    Treap::Node *maxNode = treap.max();
    register int dep = maxNode->dep;
    Splay::Node *v = maxNode->splay;
    if (v->fa != Splay::null) {
        // all
        treap.add(1, INF, 1);
        // -this
        treap.add(v->key, v->key, -dep);
        // this ~ fa
        treap.add(v->fa->key + 1, v->key - 1, -1);
#ifdef DBG
#ifdef PRINT_TAG_ADD
        std::cerr << "add(" << v->key + 1 << ", " << v->fa->key - 1 << ", -1)";
        std::endl(std::cerr);
#endif
#endif
        if (v->lc != Splay::null) v->lc->fa = v->fa;
        v->fa->rc = v->lc;
        v->fa = Splay::null, root->fa = v, v->lc = root;
        root = v;
    }
    return dep;
}

inline void extractMin() {
#ifdef DBG
    if (root == Splay::null) throw NullPointerException();
#endif
    treap.extractMin();
    root = root->rc;
    if (root != Splay::null) root->fa = Splay::null;
    treap.add(1, INF, -1);
}

inline void extractMax() {
#ifdef DBG
    if (root == Splay::null) throw NullPointerException();
#endif
    treap.extractMax();
    root = root->lc;
    if (root != Splay::null) root->fa = Splay::null;
    treap.add(1, INF, -1);
}

using IO::io;

inline void solve() {
#ifdef DBG
    assert(Treap::null->lc == Treap::null && Treap::null->rc == Treap::null);
    assert(Splay::null->lc == Splay::null && Splay::null->rc == Splay::null);
    assert(treap.root == Treap::null);
    assert(root == Splay::null);
#endif
    register int m;
    io >> m;
    for (register int i = 0, cmd, x; i < m; i++) {
        io >> cmd;
        switch (cmd) {
            case 1:
                io >> x, io << insert(x) << '\n';
                break;
            case 2:
                io << (root == Splay::null ? 0 : splayMin()) << '\n';
                break;
            case 3:
                io << (root == Splay::null ? 0 : splayMax()) << '\n';
                break;
            case 4:
                io << splayMin() << '\n';
                extractMin();
                break;
            case 5:
                io << splayMax() << '\n';
                extractMax();
                break;
#ifdef DBG
            default:
                assert(false);
#endif
        }
#ifdef DBG
#ifdef PRINT_TREAP
        Treap::printTreap(treap.root);
        std::cerr << std::endl;
#endif
#endif
    }
}
}

int main() {
    // freopen("splay.in", "r", stdin);
    // freopen("splay.out", "w", stdout);
    solve();
    return 0;
}