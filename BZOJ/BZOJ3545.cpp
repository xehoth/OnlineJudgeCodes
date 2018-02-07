#include <bits/stdc++.h>

namespace {

inline char read() {
    static const int IN_LEN = 100000;
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
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
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

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
    ~InputOutputStream() { flush(); }

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
const int MAXM = 500000 + 9;
const int MAX_LOG = 20;

struct Node {
    Node *lc, *rc;
    int size;

    Node();

    inline void *operator new(size_t);
};

const int NODE_SIZE = sizeof(Node);

char pool[MAXN * MAX_LOG * NODE_SIZE], *cur = pool;

Node *null = (Node *)pool, *root[MAXN];

inline void *Node::operator new(size_t) { return cur += NODE_SIZE; }

Node::Node() : lc(null), rc(null), size(0) {}

inline void insert(Node *&p, int l, int r, int v) {
    if (p == null) p = new Node();
    p->size++;
    if (l == r) return;
    register int mid = l + r >> 1;
    v <= mid ? insert(p->lc, l, mid, v) : insert(p->rc, mid + 1, r, v);
}

inline Node *merge(Node *u, Node *v) {
    if (u == null) return v;
    if (v == null) return u;
    u->lc = merge(u->lc, v->lc);
    u->rc = merge(u->rc, v->rc);
    u->size += v->size;
    return u;
}

int bound;

inline int select(Node *p, int k) {
    register int l = 1, r = bound;
    for (; l != r;) {
        k > p->rc->size ? (k -= p->rc->size, p = p->lc, r = (l + r) >> 1)
                        : (p = p->rc, l = ((l + r) >> 1) + 1);
    }
    return l;
}

int fa[MAXN], h[MAXN], tmp[MAXN];

inline int get(int x) { return x == fa[x] ? x : fa[x] = get(fa[x]); }

inline void put(register int u, register int v) {
    if ((u = get(u)) != (v = get(v))) {
        root[u] = merge(root[u], root[v]);
        fa[v] = u;
    }
}

struct Edge {
    int u, v, w;

    inline bool operator<(const Edge &p) const { return w < p.w; }
} edges[MAXM];

struct Query {
    int v, x, k, *ans;

    inline bool operator<(const Query &p) const { return x < p.x; }
} que[MAXM];

int ans[MAXM];

inline void solve() {
    null->lc = null->rc = null, null->size = 0;
    register int n, m, q;
    io >> n >> m >> q;
    for (register int i = 1; i <= n; i++) io >> h[i], fa[i] = i, root[i] = null;
    memcpy(tmp + 1, h + 1, sizeof(int) * n);
    std::sort(tmp + 1, tmp + n + 1);
    register int *end = std::unique(tmp + 1, tmp + n + 1);
    bound = end - tmp - 1;
    for (register int i = 1; i <= n; i++)
        h[i] = std::lower_bound(tmp + 1, end, h[i]) - tmp;
    for (register int i = 1; i <= n; i++) insert(root[i], 1, bound, h[i]);
    for (register int i = 1; i <= m; i++)
        io >> edges[i].u >> edges[i].v >> edges[i].w;
    std::sort(edges + 1, edges + m + 1);
    for (register int i = 1; i <= q; i++)
        io >> que[i].v >> que[i].x >> que[i].k, que[i].ans = ans + i;
    std::sort(que + 1, que + q + 1);
    for (register int i = 1, j = 1; i <= q; i++) {
        while (j <= m && edges[j].w <= que[i].x)
            put(edges[j].u, edges[j].v), j++;
        register Node *p = root[get(que[i].v)];
        *que[i].ans = (p->size >= que[i].k ? tmp[select(p, que[i].k)] : -1);
    }
    for (register int i = 1; i <= q; i++) io << ans[i] << '\n';
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}