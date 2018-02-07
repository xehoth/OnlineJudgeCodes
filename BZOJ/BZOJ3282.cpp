/*
 * created by xehoth on 24-03-2017
 */
#include <bits/stdc++.h>

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    if (s == t) {
        t = (s = buf) + fread(buf, 1, IN_LEN, stdin);
        if (s == t) return -1;
    }
    return *s++;
}

template <class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (iosig = false, c = read(); !isdigit(c); c = read()) {
        if (c == -1) return;
        if (c == '-') iosig = true;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    if (iosig) x = -x;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}

template <class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        if (x < 0) print('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

namespace LinkCutTree {
const int MAXN = 300010;

struct Node *null;

struct Node {
    Node *c[2], *fa;

    bool rev;

    Node *top;

    int val, sum;

    Node() : val(0), sum(0), top(NULL), rev(false), fa(null) {
        c[0] = c[1] = null;
    }

    inline void maintain() { sum = val ^ c[0]->sum ^ c[1]->sum; }

    inline void reverse() { rev ^= 1, std::swap(c[0], c[1]); }

    inline void pushDown() {
        rev ? c[0]->reverse(), c[1]->reverse(), rev = false : 0;
    }

    inline bool relation() { return this == fa->c[1]; }

    inline void rotate(bool f) {
        Node *o = fa;
        top = o->top;
        o->pushDown(), pushDown();
        (fa = o->fa)->c[o->relation()] = this;
        (o->c[f] = c[!f])->fa = o;
        (c[!f] = o)->fa = this;
        o->maintain();
    }

    inline void splay() {
        Node *o = fa;
        bool f;
        for (pushDown(); o != null; o = fa) {
            o->fa == null ? rotate(o->c[1] == this)
                          : ((f = o->c[1] == this) == (o->fa->c[1] == o)
                                 ? (o->rotate(f), rotate(f))
                                 : (rotate(f), rotate(!f)));
        }
        maintain();
    }

    inline void expose(Node *p = null) {
        splay();
        if (c[1] != null) c[1]->top = this, c[1]->fa = null;
        (c[1] = p)->fa = this;
        maintain();
    }

    inline Node *access() {
        Node *x = this;
        for (x->expose(); x->top; x = x->top) x->top->expose(x);
        return x;
    }

    inline void evert() { access(), splay(), reverse(); }

    inline void link(Node *f) {
        Node *x = access();
        x->reverse(), x->top = f;
    }

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
} pool[MAXN];

inline void update(int u, int v) {
    (pool + u)->splay();
    (pool + u)->val = v;
    (pool + u)->maintain();
}

inline void init() {
    null = pool, null->fa = null;
    null->val = null->sum = 0;
}

inline void solve() {
    register int n, m;
    read(n), read(m);
    init();
    for (register int i = 1; i <= n; i++) pool[i] = Node();
    for (register int i = 1, v; i <= n; i++) {
        read(v), (pool + i)->val = (pool + i)->sum = v;
    }
    register int cmd, x, y;
    while (m--) {
        read(cmd);
        switch (cmd) {
            case 0:
                read(x), read(y), (pool + x)->split(pool + y);
                print((pool + x)->sum), print('\n');
                break;
            case 1:
                read(x), read(y);
                if ((pool + x)->findRoot() != (pool + y)->findRoot())
                    (pool + x)->link(pool + y);
                break;
            case 2:
                read(x), read(y);
                if ((pool + x)->findRoot() == (pool + y)->findRoot())
                    (pool + x)->cut(pool + y);
                break;
            case 3:
                read(x), read(y);
                update(x, y);
                break;
        }
    }
}
}  // namespace LinkCutTree

int main() {
#ifndef ONLINE_JUDGE
// freopen("in.in", "r", stdin);
#endif
    LinkCutTree::solve();
    flush();
    return 0;
}