/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3510」首都 28-12-2017
 * Link-Cut Tree + 启发式合并
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

template <typename T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        iosig |= c == '-';
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig && (x = -x);
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

const int OUT_LEN = 1 << 18 | 1;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    (oh == obuf + OUT_LEN) && (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf);
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static char buf[21], cnt;
    if (x != 0) {
        (x < 0) && (print('-'), x = -x);
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print(buf[cnt--]);
    } else {
        print('0');
    }
}

struct InputOutputStream {
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

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
} io;

const int MAXN = 100000 + 9;

struct Node *null;

struct Node {
    Node *c[2], *fa, *top;
    int size, vsize, rev;

    inline void maintain() { size = vsize + 1 + c[0]->size + c[1]->size; }

    inline void pushDown() {
        rev && (c[0]->reverse(), c[1]->reverse(), rev = false);
    }

    inline void reverse() {
        rev ^= 1;
        std::swap(c[0], c[1]);
    }

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

    inline bool relation() { return fa->c[1] == this; }

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
            vsize += c[1]->size;
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
        top->vsize -= size;
        top->c[1] = this;
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

    inline void clear() {
        size = 1;
        vsize = rev = 0;
        fa = top = c[0] = c[1] = null;
    }
} pool[MAXN];

int ans;
int fa[MAXN], size[MAXN], root[MAXN];

inline int get(register int x) { return x == fa[x] ? x : fa[x] = get(fa[x]); }

inline void init(const int n) {
    null = pool;
    null->c[0] = null->c[1] = null->fa = null->top = null;
    for (register int i = 1; i <= n; i++) {
        pool[i].size = size[i] = 1;
        fa[i] = root[i] = i;
        pool[i].c[0] = pool[i].c[1] = pool[i].fa = pool[i].top = null;
        ans ^= i;
    }
}

inline void link(Node *u, Node *v) {
    u->evert();
    v->evert();
    u->top = v;
    v->vsize += u->size;
    v->maintain();
    register int vi = get(v - pool);
    register Node *rt = pool + root[vi];
    rt->evert();
    u->access();
    rt->splay();
    register int tmp = rt->size;
    register Node *p = rt->c[1];
    while (p->pushDown(), p->c[0] != null) p = p->c[0];
    p->access();
    if ((p->vsize + 1) * 2 > tmp ||
        ((p->vsize + 1) * 2 == tmp && p - pool < root[vi])) {
        ans ^= root[vi];
        ans ^= p - pool;
        root[vi] = p - pool;
    }
}

int n, m;

std::vector<int> edge[MAXN];

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v);
    edge[v].push_back(u);
}

void dfs(int u, int pre) {
    (pool + u)->clear();
    link(pool + u, pool + pre);
    for (register int i = 0; i < (int)edge[u].size(); i++)
        if (edge[u][i] != pre) dfs(edge[u][i], u);
}

inline void solve() {
    io >> n >> m;
    init(n);
    char cmd[4];
    for (register int u, v; m--;) {
        read(cmd);
        switch (cmd[0]) {
            case 'A': {
                io >> u >> v;
                register int fu = get(u), fv = get(v);
                if (size[fu] > size[fv]) {
                    std::swap(u, v);
                    std::swap(fu, fv);
                }
                size[fv] += size[fu];
                ans ^= root[fu];
                fa[fu] = fv;
                dfs(u, v);
                addEdge(u, v);
                break;
            }
            case 'Q': {
                io >> u;
                io << root[get(u)] << '\n';
                break;
            }
            case 'X': {
                io << ans << '\n';
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