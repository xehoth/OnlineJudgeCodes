#include <bits/stdc++.h>

const int MAXN = 10010;

struct Node {
    Node *c[2], *fa;
    bool rev;
    Node *top;

    inline void maintain() {}

    inline void reverse() { rev ^= 1, std::swap(c[0], c[1]); }

    inline void pushDown() {
        rev ? c[0]->reverse(), c[1]->reverse(), rev = false : 0;
    }

    Node();

    inline bool relation() { return this == fa->c[1]; }

    inline void rotate(bool f);

    inline void splay();

    inline bool splice();

    inline void expose();

    inline void access() {
        for (expose(); splice();)
            ;
    }

    inline void makeRoot() { access(), splay(), reverse(); }

    inline void link(Node *f) { evert(), top = f; }

    inline void cut(Node *);

    inline void evert() { access(), splay(), reverse(); }

    inline Node *findRoot();

} pool[MAXN], *null = pool;

Node::Node() {
    c[0] = c[1] = null, fa = null, top = null;
    rev = false;
}

inline void Node::expose() {
    splay();
    if (c[1] != null) {
        c[1]->top = this, c[1]->fa = null;
        c[1] = null, maintain();
    }
}

inline void Node::rotate(bool f) {
    Node *o = fa;
    top = o->top;
    o->pushDown(), pushDown();
    (fa = o->fa)->c[o->relation()] = this;
    (o->c[f] = c[!f])->fa = o;
    (c[!f] = o)->fa = this;
    o->maintain();
}

inline bool Node::splice() {
    splay();
    if (top == null) return false;
    top->expose(), top->c[1] = this, fa = top, top = null;
    fa->maintain();
    return true;
}

inline void Node::splay() {
    bool f;
    for (pushDown(); fa != null;) {
        fa->fa == null ? rotate(fa->c[1] == this)
                       : ((f = fa->c[1] == this) == (fa->fa->c[1] == fa)
                              ? (fa->rotate(f), rotate(f))
                              : (rotate(f), rotate(!f)));
    }
    maintain();
}

inline void Node::cut(Node *y) {
    Node *x = this;
    x->expose(), y->expose();
    if (x->top == y) x->top = null;
    if (y->top == x) y->top = null;
}

inline Node *Node::findRoot() {
    Node *f = this;
    f->access(), f->splay();
    while (f->pushDown(), f->c[0] != null) f = f->c[0];
    return f;
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

char obuf[1000000], *oh = obuf;
int n, m;

int main() {
    n = read();
    m = read();
    char c;
    for (int i = 1; i <= m; ++i) {
        while (c = nextChar(), c < 'A' || c > 'Z')
            ;
        if (c == 'Q') {
            if ((pool + read())->findRoot() == (pool + read())->findRoot()) {
                *oh++ = 'Y', *oh++ = 'e', *oh++ = 's', *oh++ = '\n';
            } else {
                *oh++ = 'N', *oh++ = 'o', *oh++ = '\n';
            }
        } else if (c == 'C') {
            (pool + read())->link(pool + read());
        } else {
            (pool + read())->cut(pool + read());
        }
    }
    fwrite(obuf, 1, oh - obuf, stdout);
    return 0;
}
