/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4009」接水果 27-12-2017
 * 树套树
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

const int MAXN = 40000 + 9;
const int MAX_LOG = 16;

int n, m, q;

int dep[MAXN], fa[MAX_LOG][MAXN], in[MAXN], out[MAXN], idx;

std::vector<int> edge[MAXN];

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v);
    edge[v].push_back(u);
}

void dfs(const int u) {
    dep[u] = dep[fa[0][u]] + 1;
    in[u] = ++idx;
    for (register int i = 1; (1 << i) <= dep[u]; i++)
        fa[i][u] = fa[i - 1][fa[i - 1][u]];
    for (register int i = 0; i < (int)edge[u].size(); i++) {
        if (edge[u][i] != fa[0][u]) {
            fa[0][edge[u][i]] = u;
            dfs(edge[u][i]);
        }
    }
    out[u] = idx;
}

inline int up(register int u, register int dep) {
    for (register int i = 0; (1 << i) <= dep; i++)
        if ((dep >> i) & 1) u = fa[i][u];
    return u;
}

struct Line {
    int pos, l, r, v, type;
    inline bool operator<(const Line &p) const { return pos < p.pos; }

    inline void init(int pos, int l, int r, int v, int type) {
        this->pos = pos;
        this->l = l;
        this->r = r;
        this->v = v;
        this->type = type;
    }
} a[MAXN * 8 + 1];

int cnt;

inline void addLine(int l1, int r1, int l2, int r2, int v) {
    a[++cnt].init(l1, l2, r2, v, 1);
    a[++cnt].init(r1 + 1, l2, r2, v, -1);
}

inline void add(int l1, int r1, int l2, int r2, int v) {
    if (l2 <= r2) {
        addLine(l1, r1, l2, r2, v);
        addLine(l2, r2, l1, r1, v);
    }
}

int vals[MAXN], valCnt, ans[MAXN];

struct Query {
    int u, v, k, pos;

    inline bool operator<(const Query &p) const { return u < p.u; }
} que[MAXN];

namespace SegmentTree {

char *cur;

struct Node *null;

struct Node {
    static const int NODE_SIZE;

    Node *lc, *rc;
    int cnt;

    Node() : lc(null), rc(null) {}

    inline void *operator new(size_t) { return cur += NODE_SIZE; }
};

const int Node::NODE_SIZE = sizeof(Node);

char pool[2 * MAXN * Node::NODE_SIZE * MAX_LOG * MAX_LOG];

inline void init() {
    cur = pool;
    null = (Node *)pool;
    null->lc = null->rc = null;
}

inline void insert(Node *&p, int l, int r, int s, int t, int v) {
    if (p == null) p = new Node();
    if (s <= l && t >= r) {
        p->cnt += v;
        return;
    }
    register int mid = (l + r) >> 1;
    if (s <= mid) insert(p->lc, l, mid, s, t, v);
    if (t > mid) insert(p->rc, mid + 1, r, s, t, v);
}

inline int query(Node *p, int l, int r, int v) {
    if (p == null) return 0;
    if (l == r) return p->cnt;
    register int mid = (l + r) >> 1;
    return p->cnt +
           (v <= mid ? query(p->lc, l, mid, v) : query(p->rc, mid + 1, r, v));
}
}  // namespace SegmentTree

char *cur;

struct Node *null;

struct Node {
    static const int NODE_SIZE;
    Node *lc, *rc;
    SegmentTree::Node *p;

    Node() : lc(null), rc(null), p(SegmentTree::null) {}

    inline void *operator new(size_t) { return cur += NODE_SIZE; }
};

const int Node::NODE_SIZE = sizeof(Node);

char pool[MAXN * Node::NODE_SIZE * MAX_LOG];

Node *root;

int oL, oR, oV;

inline void insert(Node *&p, int l, int r, int v) {
    if (p == null) p = new Node();
    SegmentTree::insert(p->p, 1, n, oL, oR, oV);
    if (l == r) return;
    register int mid = (l + r) >> 1;
    v <= mid ? insert(p->lc, l, mid, v) : insert(p->rc, mid + 1, r, v);
}

inline int query(register Node *p, register int l, register int r,
                 register int k, register int v) {
    for (register int mid, sum; l != r && k;) {
        sum = SegmentTree::query(p->lc->p, 1, n, v);
        mid = (l + r) >> 1;
        k <= sum ? (p = p->lc, r = mid) : (p = p->rc, l = mid + 1, k -= sum);
    }
    return vals[l];
}

inline void solve() {
    io >> n >> m >> q;
    for (register int i = 1, u, v; i < n; i++) {
        io >> u >> v;
        addEdge(u, v);
    }
    cur = pool;
    null = (Node *)pool;
    null->lc = null->rc = null;
    SegmentTree::init();
    null->p = SegmentTree::null;
    root = null;
    dfs(1);
    for (register int i = 1, x, y, v, w; i <= m; i++) {
        io >> x >> y >> v;
        vals[++valCnt] = v;
        (in[x] > in[y]) && (std::swap(x, y), 0);
        if (in[x] <= in[y] && out[y] <= out[x]) {
            w = up(y, dep[y] - dep[x] - 1);
            add(in[y], out[y], 1, in[w] - 1, v);
            add(in[y], out[y], out[w] + 1, n, v);
        } else {
            add(in[x], out[x], in[y], out[y], v);
        }
    }
    std::sort(vals + 1, vals + valCnt + 1);
    valCnt = std::unique(vals + 1, vals + valCnt + 1) - vals - 1;
    for (register int i = 1; i <= cnt; i++)
        a[i].v = std::lower_bound(vals + 1, vals + valCnt + 1, a[i].v) - vals;
    std::sort(a + 1, a + cnt + 1);
    for (register int i = 1; i <= q; i++) {
        io >> que[i].u >> que[i].v >> que[i].k;
        que[i].pos = i;
        que[i].u = in[que[i].u];
        que[i].v = in[que[i].v];
    }
    std::sort(que + 1, que + q + 1);
    for (register int i = 1, now = 1; i <= q; i++) {
        while (now <= cnt && a[now].pos <= que[i].u) {
            oL = a[now].l;
            oR = a[now].r;
            oV = a[now].type;
            insert(root, 1, valCnt, a[now].v);
            now++;
        }
        ans[que[i].pos] = query(root, 1, valCnt, que[i].k, que[i].v);
    }
    for (register int i = 1; i <= q; i++) io << ans[i] << '\n';
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}