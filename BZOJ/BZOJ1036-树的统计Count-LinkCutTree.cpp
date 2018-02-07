/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1036」树的统计Count 14-05-2017
 * LinkCutTree
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 30010;

struct Node *null;

struct Node {
    Node *c[2], *fa;
    bool rev;
    Node *top;
    int sum, max, val;

    Node() : sum(0), val(0), max(INT_MIN), fa(null) { c[0] = c[1] = null; }

    inline void maintain() {
        sum = c[0]->sum + c[1]->sum + val;
        max = std::max(val, std::max(c[0]->max, c[1]->max));
    }
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
        f->splay();
        return f;
    }

    inline void split(Node *v) { v->evert(), access(), splay(); }

    inline void init(int val) { this->val = max = sum = val; }
} pool[MAXN];

inline void init() {
    null = pool, null->fa = null;
    null->sum = 0;
    null->val = null->max = INT_MIN;
}

inline const int querySum(int u, int v) {
    (pool + u)->split(pool + v);
    return (pool + u)->sum;
}

inline const int queryMax(int u, int v) {
    (pool + u)->split(pool + v);
    return (pool + u)->max;
}

inline void update(int u, const int value) {
    (pool + u)->splay();
    (pool + u)->val = value;
    (pool + u)->maintain();
}
inline char nextChar() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    if (s == t) {
        t = (s = buf) + fread(buf, 1, IN_LEN, stdin);
        if (s == t) return -1;
    }
    return *s++;
}

inline int read() {
    static int x = 0;
    static char c;
    static bool iosig;
    for (iosig = false, x = 0, c = nextChar(); !isdigit(c); c = nextChar()) {
        if (c == '-') iosig = true;
    }
    for (; isdigit(c); c = nextChar()) x = (x + (4 * x) << 1) + (c ^ '0');
    return iosig ? -x : x;
}

const int OUT_LEN = 10000000;
char obuf[OUT_LEN], *oh = obuf;

template <class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        *oh++ = '0';
    } else {
        if (x < 0) *oh++ = '-', x = -x;
        register int cnt = 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) *oh++ = buf[cnt--];
    }
}

template <class T>
inline void println(T x) {
    print(x), *oh++ = '\n';
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

int n, q;

struct Edge {
    int u, v;
    inline void read() { u = ::read(), v = ::read(); }
} edge[MAXN];

int main() {
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
#endif
    n = read();
    init();
    for (register int i = 1; i <= n; i++) pool[i] = Node();
    for (register int i = 0, u, v; i < n - 1; i++) edge[i].read();
    for (register int i = 1; i <= n; i++) {
        (pool + i)->init(read());
    }
    for (register int i = 0; i < n - 1; i++) {
        (pool + edge[i].u)->link(pool + edge[i].v);
    }
    q = read();

    for (register int i = 0; i < q; i++) {
        nextChar();
        register char str = nextChar();
        if (str == 'H') {
            register int u = read(), t = read();
            update(u, t);
        } else if (str == 'M') {
            register int u = read(), v = read();
            println(queryMax(u, v));
        } else if (str == 'S') {
            register int u = read(), v = read();
            println(querySum(u, v));
        }
    }
    flush();
    return 0;
}
