/*
 * created by xehoth on 01-04-2017
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
const int MAXN = 200010;

struct Node *null;

struct Node {
    Node *c[2], *fa;
    bool rev;
    Node *top;
    int val, sum, size;
    Node() : top(NULL), rev(false), fa(null), val(0), sum(0), size(1) {
        c[0] = c[1] = null;
    }

    inline void reverse() { rev ^= 1, std::swap(c[0], c[1]); }

    inline void maintain() {
        sum = val + c[0]->sum + c[1]->sum, size = 1 + c[0]->size + c[1]->size;
    }

    inline bool relation() { return this == fa->c[1]; }

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

inline void init(const int n) {
    null = pool, null->fa = null, null->size = 0;
    for (register int i = 1; i <= n; i++) pool[i] = Node();
}

inline void link(const int u, const int v) { (pool + u)->link(pool + v); }

inline void cut(const int u, const int v) { (pool + u)->cut(pool + v); }

inline void update(const int u, const int val) {
    (pool + u)->splay();
    (pool + u)->val = val;
    (pool + u)->maintain();
}

inline void query(const int u, const int v, int &size, int &sum) {
    (pool + v)->split(pool + u);
    sum = (pool + v)->sum, size = (pool + v)->size;
}

struct Day {
    int id, task;
    int u, v, c;

    struct Answer {
        int size, sum;
    } ans;

    inline bool operator<(const Day &other) const {
        if (c == other.c) return task < other.task;
        return c < other.c;
    }
} days[MAXN];

inline bool compareByC(const Day &a, const Day &b) {
    if (a.c == b.c)
        return a.task < b.task;
    else
        return a.c < b.c;
}

inline bool compareByID(const Day &a, const Day &b) { return a.id < b.id; }

inline void solve() {
    register int n, m;
    read(n);
    init(n);
    for (register int i = 1, u; i <= n; i++) {
        read(u);
        if (u != 0) link(u, i);
    }
    read(m);
    register bool flag1 = true, flag2 = true;
    for (register int i = 0; i < m; i++) {
        days[i].id = i, read(days[i].task);
        if (days[i].task == 2) {
            read(days[i].u), days[i].c = i;
        } else {
            register int tmp;
            read(days[i].u), read(days[i].v), read(tmp);
            days[i].c = i - tmp;
        }
    }
    std::sort(days, days + m, &compareByC);
    for (register int i = 0; i < m; i++) {
        if (days[i].task == 2) {
            update(days[i].u, 1);
        } else {
            query(days[i].u, days[i].v, days[i].ans.size, days[i].ans.sum);
        }
    }
    std::sort(days, days + m, &compareByID);

    for (register int i = 0; i < m; i++) {
        if (days[i].task == 1)
            print(days[i].ans.size), print(' '), print(days[i].ans.sum),
                print('\n');
    }
}
}  // namespace LinkCutTree

int main() {
#ifndef ONLINE_JUDGE
//    freopen("in.in", "r", stdin);
#endif
    LinkCutTree::solve();
    flush();
    return 0;
}
