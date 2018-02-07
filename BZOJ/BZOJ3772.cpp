#include <bits/stdc++.h>
inline char read() {
    static const int IO_LEN = 1024 * 1024;
    static char buf[IO_LEN], *ioh, *iot;
    if (iot == ioh) {
        iot = (ioh = buf) + fread(buf, 1, IO_LEN, stdin);
        if (ioh == iot) return -1;
    }
    return *ioh++;
}
template <class T>
inline void read(T &x) {
    static char ioc;
    static bool iosig = 0;
    for (iosig = 0, ioc = read(); !isdigit(ioc); ioc = read())
        if (ioc == '-') iosig = 1;
    for (x = 0; isdigit(ioc); ioc = read())
        x = (x << 1) + (x << 3) + (ioc ^ '0');
    if (iosig) x = -x;
}
template <class T, size_t size>
struct MemoryPool {
    T buf[size], *tail, *end;
    MemoryPool() : tail(buf), end(buf + size) {}
    inline T *alloc() { return tail != end ? tail++ : new T; }
    inline void clear() { this->tail = buf; }
};
const int MAXN = 100100;
using namespace std;
const int MAXM = 3804000;
struct Node {
    Node *lc, *rc;
    int val;
    inline void *operator new(size_t, Node *lc, Node *rc, const int val) {
        static Node buf[MAXM], *tail = buf;
        return tail->lc = lc, tail->rc = rc, tail->val = val, tail++;
    }
    inline Node *insert(const int l, const int r, const int pos,
                        const int val) {
        register int mid = l + r >> 1;
        if (l == r) return new (0x0, 0x0, this->val + val) Node;
        if (pos <= mid)
            return new (lc->insert(l, mid, pos, val), rc, this->val + val) Node;
        else
            return new (lc, rc->insert(mid + 1, r, pos, val), this->val + val)
                Node;
    }
    inline friend int query(const Node *p1, const Node *p2, const Node *p3,
                            const Node *p4, const int x, const int y,
                            const int l, const int r) {
        register int mid = x + y >> 1;
        if (x == l && y == r) return p1->val + p2->val - p3->val - p4->val;
        if (r <= mid)
            return query(p1->lc, p2->lc, p3->lc, p4->lc, x, mid, l, r);
        if (l > mid)
            return query(p1->rc, p2->rc, p3->rc, p4->rc, mid + 1, y, l, r);
        return query(p1->lc, p2->lc, p3->lc, p4->lc, x, mid, l, mid) +
               query(p1->rc, p2->rc, p3->rc, p4->rc, mid + 1, y, mid + 1, r);
    };
} * tree[MAXN];

struct Query {
    int x, y;
    inline bool operator<(const Query &q) const {
        return x != q.x ? x < q.x : y < q.y;
    }
    inline bool operator==(const Query &q) const {
        return x == q.x && y == q.y;
    }
} que[MAXN];

struct Edge {
    int to;
    Edge *next;
    inline void init(const int to, Edge *next) {
        this->to = to, this->next = next;
    }
} edge[MAXN << 1];
Edge *head[MAXN], *cur = edge;
int n, m, fa[MAXN], dep[MAXN], ancestor[MAXN][18], in[MAXN], out[MAXN];
vector<int> g[MAXN];
long long A, B;
inline void addEdge(const int u, const int v) {
    (++cur)->init(v, head[u]), head[u] = cur;
}
inline void dfs1(const int x) {
    static int cnt;
    dep[x] = dep[fa[x]] + 1, in[x] = ++cnt;
    for (Edge *p = head[x]; p; p = p->next)
        if (p->to != fa[x]) fa[p->to] = x, ancestor[p->to][0] = x, dfs1(p->to);
    out[x] = ++cnt;
}
inline void dfs2(const int x) {
    tree[x] = tree[fa[x]];
    for (std::vector<int>::iterator it = g[x].begin(); it != g[x].end(); it++)
        tree[x] = tree[x]->insert(1, n << 1, in[*it], 1),
        tree[x] = tree[x]->insert(1, n << 1, out[*it], -1);
    for (Edge *p = head[x]; p; p = p->next)
        if (p->to != fa[x]) dfs2(p->to);
}
inline int LCA(int x, int y) {
    if (dep[x] < dep[y]) std::swap(x, y);
    for (register int j = 17; ~j; j--)
        if (dep[ancestor[x][j]] >= dep[y]) x = ancestor[x][j];
    if (x == y) return x;
    for (register int j = 17; ~j; j--)
        if (ancestor[x][j] != ancestor[y][j])
            x = ancestor[x][j], y = ancestor[y][j];
    return fa[x];
}
int main() {
    // freopen("in.in", "r", stdin);
    register int x, y;
    read(n), read(m);
    for (register int i = 1; i < n; i++)
        read(x), read(y), addEdge(x, y), addEdge(y, x);
    for (register int i = 1; i <= m; i++)
        read(x), read(y), g[x].push_back(y), que[i].x = x, que[i].y = y;
    tree[0] = new (0x0, 0x0, 0) Node, tree[0]->lc = tree[0]->rc = tree[0],
    dfs1(1), dfs2(1);
    for (register int j = 1; j <= 17; j++)
        for (register int i = 1; i <= n; i++)
            ancestor[i][j] = ancestor[ancestor[i][j - 1]][j - 1];
    for (register int i = 1; i <= m; i++) {
        x = que[i].x, y = que[i].y;
        register int lca = LCA(x, y);
        A += query(tree[x], tree[y], tree[lca], tree[fa[lca]], 1, n << 1,
                   in[lca], in[x]) +
             query(tree[x], tree[y], tree[lca], tree[fa[lca]], 1, n << 1,
                   in[lca], in[y]) -
             query(tree[x], tree[y], tree[lca], tree[fa[lca]], 1, n << 1,
                   in[lca], in[lca]) -
             1;
    }
    std::sort(que + 1, que + m + 1);
    register int i, j;
    for (i = 1; i <= m; i = j) {
        for (j = i + 1; j <= m && que[i] == que[j]; j++)
            ;
        A -= (long long)(j - i) * (j - i - 1) >> 1;
    }
    B = (long long)m * (m - 1) >> 1;
    long long gcd = std::__gcd(A, B);
    A /= gcd;
    B /= gcd;
    cout << A << '/' << B;
    return 0;
}