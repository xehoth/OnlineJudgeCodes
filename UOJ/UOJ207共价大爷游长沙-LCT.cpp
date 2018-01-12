/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「UOJ 207」共价大爷游长沙 12-01-2018
 * LCT
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

inline char read() {
    static const int IN_LEN = 1 << 18 | 1;
    static char buf[IN_LEN], *s, *t;
    return (s == t) && (t = (s = buf) + fread(buf, 1, IN_LEN, stdin)),
           s == t ? -1 : *s++;
}
const int OUT_LEN = 1 << 18 | 1;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    (oh == obuf + OUT_LEN) && (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf);
    *oh++ = c;
}

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

struct InputOutputStream {
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        iosig && (x = -x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 100000 + 9;

typedef unsigned int uint;

uint seed;

inline uint nextUint() {
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return seed;
}

struct Node *null;

struct Node {
    Node *c[2], *fa, *top;
    uint val, sum;
    int rev;

    inline int relation() { return fa->c[1] == this; }

    inline void reverse() {
        rev ^= 1;
        std::swap(c[0], c[1]);
    }

    inline void pushDown() {
        rev && (c[0]->reverse(), c[1]->reverse(), rev = false);
    }

    inline void maintain() { sum = c[0]->sum ^ c[1]->sum ^ val; }

    inline void rotate(register bool f) {
        register Node *o = fa;
        top = o->top;
        o->pushDown();
        pushDown();
        (fa = o->fa)->c[o->relation()] = this;
        (o->c[f] = c[!f])->fa = o;
        (c[!f] = o)->fa = this;
        o->maintain();
    }

    inline void splay() {
        register bool f;
        for (pushDown(); fa != null;) {
            (f = relation(), fa->fa == null)
                ? rotate(f)
                : (f == fa->relation() ? (fa->rotate(f), rotate(f))
                                       : (rotate(f), rotate(!f)));
        }
        maintain();
    }

    inline void expose() {
        splay();
        if (c[1] != null) {
            val ^= c[1]->sum;
            c[1]->top = this;
            c[1]->fa = null;
            c[1] = null;
            maintain();
        }
    }

    inline bool splice() {
        splay();
        if (top == null) return false;
        top->expose();
        top->c[1] = this;
        top->val ^= sum;
        fa = top;
        top = null;
        fa->maintain();
        return true;
    }

    inline void access() {
        for (expose(); splice();)
            ;
    }

    inline void evert() {
        access();
        splay();
        reverse();
    }

    inline void init() {
        c[0] = c[1] = null;
        fa = null;
        top = null;
    }
} pool[MAXN];

inline void link(int x, int y) {
    register Node *u = pool + x, *v = pool + y;
    u->evert();
    v->evert();
    u->top = v;
    v->val ^= u->sum;
    v->maintain();
}

inline void cut(int x, int y) {
    register Node *u = pool + x, *v = pool + y;
    u->evert();
    v->access();
    v->splay();
    v->c[0] = null;
    u->fa = null;
    v->maintain();
}

inline void insert(int x, int y, uint w) {
    register Node *u = pool + x, *v = pool + y;
    u->evert();
    u->val ^= w;
    u->maintain();
    v->evert();
    v->val ^= w;
    v->maintain();
}

const int MAXM = 300000 + 9;

uint S, sx[MAXM], sy[MAXM], sw[MAXN], cnt;

inline bool query(int x, int y) {
    register Node *u = pool + x, *v = pool + y;
    u->evert();
    v->access();
    v->splay();
    return v->val == S;
}

inline void init() {
    char *t = new char;
    seed = time(0) * (long long)t;
    (seed == 0) && (seed++);
    delete t;
}

inline void solve() {
    init();
    register int n, m;
    io >> n >> n >> m;
    null = pool;
    null->c[0] = null->c[1] = null;
    null->fa = null->top = null;
    for (register int i = 1; i <= n; i++) pool[i].init();

    for (register int i = 1, u, v; i < n; i++) {
        io >> u >> v;
        link(u, v);
    }
    for (register int cmd, x, y, u, v; m--;) {
        io >> cmd;
        switch (cmd) {
            case 1: {
                io >> x >> y >> u >> v;
                cut(x, y);
                link(u, v);
                break;
            }
            case 2: {
                io >> x >> y;
                sx[++cnt] = x;
                sy[cnt] = y;
                sw[cnt] = nextUint();
                insert(x, y, sw[cnt]);
                S ^= sw[cnt];
                break;
            }
            case 3: {
                io >> x;
                insert(sx[x], sy[x], sw[x]);
                S ^= sw[x];
                break;
            }
            case 4: {
                io >> x >> y;
                io << (query(x, y) ? "YES\n" : "NO\n");
                break;
            }
        }
    }
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}