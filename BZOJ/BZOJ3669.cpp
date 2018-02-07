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
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
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
}  // namespace IO

namespace LinkCutTree {

const int MAXN = 50000;
const int MAXM = 100000;

struct Node *null;

struct Node {
    Node *c[2], *fa, *top, *max;
    bool rev;
    int w;

    Node() : fa(null), top(NULL), rev(false), w(0), max(this) {
        c[0] = c[1] = null;
    }

    inline bool relation() { return this == fa->c[1]; }

    inline void maintain() {
        max = this;
        c[0]->max->w > max->w ? max = c[0]->max : 0;
        c[1]->max->w > max->w ? max = c[1]->max : 0;
    }

    inline void reverse() { rev ^= 1, std::swap(c[0], c[1]); }

    inline void pushDown() {
        rev ? c[0]->reverse(), c[1]->reverse(), rev = false : 0;
    }

    inline void rotate(bool f) {
        Node *o = fa;
        top = o->top, o->pushDown(), pushDown();
        (fa = o->fa)->c[o->relation()] = this;
        (o->c[f] = c[!f])->fa = o;
        (c[!f] = o)->fa = this;
        o->maintain();
    }

    inline void splay() {
        Node *o = fa;
        bool f;
        for (; o != null; o = fa) {
            o->fa == null ? rotate(o->c[1] == this)
                          : ((f = o->c[1] == this) == (o->fa->c[1] == o)
                                 ? (o->rotate(f), rotate(f))
                                 : (rotate(f), rotate(!f)));
        }
        maintain();
    }

    inline void expose() {
        splay(), pushDown();
        if (c[1] != null)
            c[1]->top = this, c[1]->fa = null, c[1] = null, maintain();
    }

    inline bool splice() {
        splay();
        if (!top) return false;
        top->expose(), top->c[1] = this, fa = top, top = NULL, fa->maintain();
        return true;
    }

    inline void access() {
        for (expose(); splice();)
            ;
    }

    inline void evert() { access(), splay(), reverse(); }

    inline void link(Node *v) { evert(), top = v; }

    inline void cut(Node *y) {
        Node *x = this;
        x->expose(), y->expose();
        if (x->top == y) x->top = NULL;
        if (y->top == x) y->top = NULL;
    }

    inline Node *findRoot() {
        Node *f = this;
        f->access(), f->splay();
        while (f->pushDown(), f->c[0] != null) f = f->c[0];
        return f;
    }

    inline void split(Node *v) { v->evert(), access(), splay(); }
};

Node nullNode;

struct LinkCutTree {
    Node vir[MAXN], e[MAXM], *a[MAXM][2];

    inline void link(const int u, const int v, const int i, const int w) {
        e[i].w = w, a[i][0] = vir + u, a[i][1] = vir + v;
        (vir + u)->link(e + i), (e + i)->link(vir + v);
    }

    inline void cut(const int i) { a[i][0]->cut(e + i), (e + i)->cut(a[i][1]); }

    inline bool test(const int u, const int v) {
        return (vir + u)->findRoot() == (vir + v)->findRoot();
    }

    inline int query(const int u, const int v) {
        (vir + v)->split(vir + u);
        return (vir + v)->max - e;
    }

    inline void init(const int n, const int m) {
        for (register int i = 0; i < n; i++) vir[i] = Node();
        for (register int i = 0; i < m; i++) e[i] = Node();
    }
} lct;

struct Edge {
    int u, v, a, b;

    bool operator<(const Edge &other) const { return a < other.a; }
} E[MAXM];

inline void solve() {
    int n, m;
    using namespace IO;
    read(n), read(m);
    null = &nullNode, nullNode = Node();
    lct.init(n, m);
    for (register int i = 0; i < m; i++) {
        read(E[i].u), read(E[i].v), read(E[i].a), read(E[i].b);
        E[i].u--, E[i].v--;
    }
    std::sort(E, E + m);
    register int ans = INT_MAX;
    for (register int i = 0; i < m; i++) {
        const Edge &e = E[i];
        if (e.u == e.v) continue;
        if (lct.test(e.u, e.v)) {
            register int max = lct.query(e.u, e.v);
            if (E[max].b > e.b) {
                lct.cut(max), lct.link(e.u, e.v, i, e.b);
            }
        } else {
            lct.link(e.u, e.v, i, e.b);
        }
        if (lct.test(0, n - 1)) {
            ans = std::min(ans, e.a + E[lct.query(0, n - 1)].b);
        }
    }
    print(ans == INT_MAX ? -1 : ans), print('\n');
}
}  // namespace LinkCutTree

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    LinkCutTree::solve();
    IO::flush();
    return 0;
}