#include <bits/stdc++.h>

struct InputOutputStream {
    enum { SIZE = 1 << 18 | 1 };
    char ibuf[SIZE], obuf[SIZE], *s, *t, *oh;

    InputOutputStream() : s(), t(), oh(obuf) {}
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    inline char read() {
        return (s == t) && (t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin)),
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

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    inline void print(char c) {
        (oh == obuf + SIZE) && (fwrite(obuf, 1, SIZE, stdout), oh = obuf);
        *oh++ = c;
    }

    template <typename T>
    inline void print(T x) {
        static int buf[21], cnt;
        if (x != 0) {
            (x < 0) && (print('-'), x = -x);
            for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
            while (cnt) print((char)buf[cnt--]);
        } else {
            print('0');
        }
    }

    inline void print(const char *s) {
        for (; *s; s++) print(*s);
    }

    static const double EPS = 1e-9;

    static inline int sign(const double x) { return (x > EPS) - (x < -EPS); }

    inline void print(double x) {
        static char buf[21];
        sprintf(buf, "%.10f", x);
        print((const char *)buf);
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 100000 + 9;
const int MAX_LOG = 17;

std::vector<int> g[MAXN];

int n, m, root;

int fa[MAX_LOG][MAXN], in[MAXN], out[MAXN], dep[MAXN], val[MAXN], idx;

inline void addEdge(const int u, const int v) {
    g[u].push_back(v);
    g[v].push_back(u);
}

inline int up(register int u, const int d) {
    for (register int i = 0; (1 << i) <= d; i++)
        if (d & (1 << i)) u = fa[i][u];
    return u;
}

namespace SegmentTree {

char *cur;

struct Node *null;

struct Node {
    static const int NODE_SIZE;
    Node *lc, *rc;
    long long sum, add;
    int l, r;

    Node(int l, int r) : lc(null), rc(null), l(l), r(r) {}

    inline void *operator new(size_t) { return cur += NODE_SIZE; }

    inline void cover(const int x) {
        if (this != null) {
            sum += x * (r - l + 1ll);
            add += x;
        }
    }

    inline void pushDown() {
        if (add) {
            lc->cover(add);
            rc->cover(add);
            add = 0;
        }
    }

    inline void maintain() { sum = lc->sum + rc->sum; }
};

const int Node::NODE_SIZE = sizeof(Node);

char pool[MAXN * Node::NODE_SIZE * 4];

Node *root;

void build(Node *&p, int l, int r) {
    p = new Node(l, r);
    if (l == r) {
        p->sum = val[l];
        return;
    }
    register int mid = (l + r) >> 1;
    build(p->lc, l, mid);
    build(p->rc, mid + 1, r);
    p->maintain();
}

void modify(Node *p, int s, int t, const int v) {
    if (s <= p->l && t >= p->r) {
        p->cover(v);
        return;
    }
    p->pushDown();
    register int mid = (p->l + p->r) >> 1;
    if (s <= mid) modify(p->lc, s, t, v);
    if (t > mid) modify(p->rc, s, t, v);
    p->maintain();
}

long long query(Node *p, int s, int t) {
    if (s <= p->l && t >= p->r) return p->sum;
    p->pushDown();
    register int mid = (p->l + p->r) >> 1;
    register long long ret = 0;
    if (s <= mid) ret += query(p->lc, s, t);
    if (t > mid) ret += query(p->rc, s, t);
    return ret;
}

inline void init() {
    cur = pool;
    null = (Node *)pool;
    null->lc = null;
    null->rc = null;
}
}  // namespace SegmentTree

namespace LinkCutTree {

inline void modify(int x, const int v) {
    if (!x) return;
    if (x == root) {
        SegmentTree::modify(SegmentTree::root, 1, n, v);
    } else if (in[x] <= in[root] && out[root] <= out[x]) {
        x = up(root, dep[root] - dep[x] - 1);
        if (in[x] - 1 >= 1)
            SegmentTree::modify(SegmentTree::root, 1, in[x] - 1, v);
        if (out[x] + 1 <= n)
            SegmentTree::modify(SegmentTree::root, out[x] + 1, n, v);
    } else {
        SegmentTree::modify(SegmentTree::root, in[x], out[x], v);
    }
}

struct Node *null;

struct Node {
    Node *c[2], *fa, *top;
    int id, rev;

    inline void reverse() {
        rev ^= 1;
        std::swap(c[0], c[1]);
    }

    inline void pushDown() {
        rev && (c[0]->reverse(), c[1]->reverse(), rev = false);
    }

    inline int relation() { return fa->c[1] == this; }

    inline void rotate(register int f) {
        register Node *o = fa;
        top = o->top;
        o->pushDown();
        pushDown();
        (fa = o->fa)->c[o->relation()] = this;
        (o->c[f] = c[!f])->fa = o;
        (c[!f] = o)->fa = this;
    }

    inline void splay() {
        register bool f;
        for (pushDown(); fa != null;) {
            (f = relation(), fa->fa == null)
                ? rotate(f)
                : (f == fa->relation() ? (fa->rotate(f), rotate(f))
                                       : (rotate(f), rotate(!f)));
        }
    }

    inline Node *get(Node *p) {
        while (p->pushDown(), p->c[0] != null) p = p->c[0];
        return p;
    }

    inline void expose() {
        splay();
        if (c[1] != null) {
            modify(get(c[1])->id, 1);
            c[1]->top = this;
            c[1]->fa = null;
            c[1] = null;
        }
    }

    inline bool splice() {
        splay();
        if (top == null) return false;
        modify(get(this)->id, -1);
        top->expose();
        top->c[1] = this;
        fa = top;
        top = null;
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
} pool[MAXN];

inline void init() {
    null = pool;
    null->c[0] = null;
    null->c[1] = null;
    null->fa = null;
    null->top = null;
    for (int i = 1; i <= n; i++) {
        (pool + i)->c[0] = null;
        (pool + i)->c[1] = null;
        (pool + i)->fa = null;
        (pool + i)->top = null;
        (pool + i)->id = i;
    }
}

}  // namespace LinkCutTree

void dfs(const int u) {
    using LinkCutTree::pool;
    in[u] = ++idx;
    dep[u] = dep[fa[0][u]] + 1;
    val[idx] = val[in[fa[0][u]]] + 1;
    for (register int i = 1; (1 << i) <= dep[u]; i++)
        fa[i][u] = fa[i - 1][fa[i - 1][u]];
    for (register int i = 0, v; i < (int)g[u].size(); i++) {
        if ((v = g[u][i]) != fa[0][u]) {
            fa[0][v] = u;
            (pool + v)->top = pool + u;
            dfs(v);
        }
    }
    out[u] = idx;
}

inline void query(register int x) {
    if (!x) return;
    if (x == root) {
        io << ((double)SegmentTree::query(SegmentTree::root, 1, n) / n) << '\n';
    } else if (in[x] <= in[root] && out[root] <= out[x]) {
        x = up(root, dep[root] - dep[x] - 1);
        io << ((double)(SegmentTree::query(SegmentTree::root, 1, in[x] - 1) +
                        SegmentTree::query(SegmentTree::root, out[x] + 1, n)) /
               (n - (out[x] - in[x] + 1)))
           << '\n';
    } else {
        io << ((double)SegmentTree::query(SegmentTree::root, in[x], out[x]) /
               (out[x] - in[x] + 1))
           << '\n';
    }
}

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    // freopen("recompile.in", "r", stdin);
    // freopen("recompile.out", "w", stdout);
    io >> n >> m;
    for (register int i = 1, u, v; i < n; i++) {
        io >> u >> v;
        addEdge(u, v);
    }
    LinkCutTree::init();
    dfs(1);
    SegmentTree::init();
    SegmentTree::build(SegmentTree::root, 1, n);
    static char cmd[21];
    for (register int x; m--;) {
        io >> cmd;
        switch (cmd[2]) {
            case 'L': {
                io >> x;
                (LinkCutTree::pool + x)->access();
                break;
            }
            case 'C': {
                io >> x;
                (LinkCutTree::pool + x)->evert();
                root = x;
                break;
            }
            case 'Q': {
                io >> x;
                query(x);
                break;
            }
        }
    }
    return 0;
}