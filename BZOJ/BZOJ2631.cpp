/*
 * created by xehoth on 21-01-2017
 */
#include <bits/stdc++.h>

typedef unsigned int uint;

const int MAXN = 100010;
const int MOD = 51061;

struct Node *null;

struct Node {
    Node *c[2], *fa;
    bool rev;
    Node *top;
    uint sum, add, mul, size, val;

    Node() : mul(1), sum(1), val(1), size(1), fa(null), top(NULL) {
        c[0] = c[1] = null;
    }

    inline void cover(int m, int a) {
        val = (val * m + a) % MOD;
        sum = (sum * m + a * size) % MOD;
        add = (add * m + a) % MOD;
        mul = (mul * m) % MOD;
    }

    inline void maintain() {
        sum = (c[0]->sum + c[1]->sum + val) % MOD;
        size = (c[0]->size + c[1]->size + 1) % MOD;
    }
    inline void reverse() { rev ^= 1, std::swap(c[0], c[1]); }

    inline void pushDown() {
        rev ? c[0]->reverse(), c[1]->reverse(), rev = false : 0;
        mul != 1 || add != 0 ? c[0]->cover(mul, add), c[1]->cover(mul, add),
            mul = 1, add = 0 : 0;
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

    inline void expose() {
        splay();
        // pushDown();
        if (c[1] != null) {
            c[1]->top = this, c[1]->fa = null;
            c[1] = null, maintain();
        }
    }

    /*inline void expose(Node *p = null) {
        splay();
        if (c[1] != null)
            c[1]->top = this, c[1]->fa = null;
        (c[1] = p)->fa = this;
        maintain();
    }*/

    inline bool splice() {
        splay();
        if (!top) return false;
        top->expose(), top->c[1] = this;
        fa = top, top = NULL;
        fa->maintain();
        return true;
    }

    inline void access() {
        expose();
        while (splice())
            ;
    }

    inline void evert() { access(), splay(), reverse(); }

    inline void link(Node *f) { evert(), top = f; }

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
} pool[MAXN];

inline void init() {
    null = pool, null->fa = null;
    null->sum = null->val = null->size = null->add = 0;
    null->mul = 1;
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
    for (x = 0, c = nextChar(); !isdigit(c); c = nextChar())
        ;
    for (; isdigit(c); c = nextChar()) x = (x + (4 * x) << 1) + (c ^ '0');
    return x;
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

int main() {
#ifndef ONLINE_JUDGE
// freopen("in.in", "r", stdin);
#endif
    init();
    n = read(), q = read();
    for (register int i = 1; i <= n; i++) pool[i] = Node();
    char s;
    register int x, y, z;
    for (register int i = 1; i < n; i++) (pool + read())->link(pool + read());
    for (register int i = 1; i <= q; i++) {
        s = nextChar(), x = read(), y = read();
        switch (s) {
            case '+':
                z = read(), (pool + x)->split(pool + y);
                (pool + x)->cover(1, z);
                break;
            case '-':
                (pool + x)->cut(pool + y), x = read(), y = read(),
                                           (pool + x)->link(pool + y);
                break;
            case '*':
                z = read(), (pool + x)->split(pool + y);
                (pool + x)->cover(z, 0);
                break;
            case '/':
                (pool + x)->split(pool + y), println((pool + x)->sum);
                break;
        }
    }
    flush();
    return 0;
}
