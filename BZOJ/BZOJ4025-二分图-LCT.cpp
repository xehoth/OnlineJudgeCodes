/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4025」二分图 01-12-2017
 * LCT
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

inline char read() {
    static const int IN_LEN = 100000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

const int OUT_LEN = 100000;

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
    ~InputOutputStream() { flush(); }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            c == '-' ? iosig = true : 0;
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        iosig ? x = -x : 0;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int INF = 0x3f3f3f3f;

const int MAXN = 300000 + 9;

int time[MAXN], n;

struct Node *null;

struct Node {
    Node *c[2], *fa, *top;
    int rev, id, min, sum;

    inline void maintain() {
        if (this != null) {
            sum = (id > n) + c[0]->sum + c[1]->sum;
            min = (time[id] < time[c[0]->min]
                       ? (time[id] < time[c[1]->min] ? id : c[1]->min)
                       : (time[c[0]->min] < time[c[1]->min] ? c[0]->min
                                                            : c[1]->min));
        }
    }

    inline void pushDown() {
        rev ? (c[0]->reverse(), c[1]->reverse(), rev = 0) : 0;
    }

    inline void reverse() { rev ^= 1, std::swap(c[0], c[1]); }

    inline int relation() { return this == fa->c[1]; }

    inline void rotate(register bool f) {
        register Node *o = fa;
        top = o->top, o->pushDown(), pushDown();
        (fa = o->fa)->c[o->relation()] = this;
        (o->c[f] = c[!f])->fa = o, (c[!f] = o)->fa = this, o->maintain();
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
        if (c[1] != null)
            c[1]->top = this, c[1]->fa = null, c[1] = null, maintain();
    }

    inline bool splice() {
        splay();
        if (top == null) return false;
        top->expose(), top->c[1] = this, fa = top, top = null, fa->maintain();
        return true;
    }

    inline void access() {
        for (expose(); splice();)
            ;
    }

    inline void evert() { access(), splay(), reverse(); }

    inline void init(const int i) {
        id = i, fa = null, top = null, c[0] = null, c[1] = null;
    }
} pool[MAXN];

inline void link(register Node *u, register Node *v) { u->evert(), u->top = v; }

inline void cut(register Node *u, register Node *v) {
    u->expose(), v->expose();
    if (u->top == v) u->top = null;
    if (v->top == u) v->top = null;
}

inline int queryMin(register Node *u, register Node *v) {
    return u->evert(), v->access(), v->min;
}

inline int querySum(register Node *u, register Node *v) {
    return u->evert(), v->access(), v->sum;
}

inline Node *get(register Node *p) {
    p->access(), p->splay();
    while (p->c[0] != null) p = p->c[0];
    return p;
}

inline void init(const int n) {
    null = pool;
    null->c[0] = null->c[1] = null, null->fa = null, null->top = null;
    for (register int i = 1; i <= n; i++) pool[i].init(i);
}

struct Edge {
    int u, v, b, e, g;

    inline bool operator<(const Edge &p) const { return b < p.b; }
} E[MAXN], D[MAXN];

inline bool cmp(const Edge &a, const Edge &b) { return a.e < b.e; }

int vis[MAXN], cnt, m, t;

inline void insert(const Edge &k) {
    if (k.u == k.v) {
        vis[k.g] = 1, cnt++;
        return;
    }
    time[k.g] = k.e;
    (pool + k.g)->min = k.g;
    if (get(pool + k.u) == get(pool + k.v)) {
        register int x = queryMin(pool + k.u, pool + k.v);
        if (time[x] > time[k.g]) {
            if (querySum(pool + k.u, pool + k.v) & 1 ^ 1)
                vis[k.g]++, cnt++;
            else
                return;
        } else {
            if (querySum(pool + k.u, pool + k.v) & 1 ^ 1) vis[x]++, cnt++;
            cut(pool + E[x - n].u, pool + E[x - n].g);
            cut(pool + E[x - n].v, pool + E[x - n].g);
            link(pool + k.u, pool + k.g);
            link(pool + k.v, pool + k.g);
        }
    } else {
        link(pool + k.u, pool + k.g), link(pool + k.g, pool + k.v);
    }
}

inline void erase(const Edge &k) {
    if (vis[k.g])
        vis[k.g] = 0, cnt--;
    else if (get(pool + k.u) == get(pool + k.g))
        cut(pool + k.u, pool + k.g), cut(pool + k.v, pool + k.g);
}

inline void solve() {
    io >> n >> m >> t;
    memset(time, 0x3f, sizeof(int) * (n + 1));
    for (register int i = 1; i <= m; i++)
        io >> E[i].u >> E[i].v >> E[i].b >> E[i].e;
    std::sort(E + 1, E + 1 + m);
    for (register int i = 1; i <= m; i++) E[i].g = i + n;
    init(m + n);
    memcpy(D + 1, E + 1, sizeof(Edge) * m);
    std::sort(D + 1, D + 1 + m, cmp);
    for (register int i = 1, j = 1, k = 1; i <= t; i++) {
        while (E[j].b < i && j <= m) insert(E[j++]);
        while (D[k].e < i && k <= m) erase(D[k++]);
        if (cnt)
            io << "No\n";
        else
            io << "Yes\n";
    }
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}