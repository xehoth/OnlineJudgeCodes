#include <bits/stdc++.h>
inline char read() {
    static const int IO_LEN = 1024 * 1024;
    static char buf[IO_LEN], *ioh, *iot;
    if (ioh == iot) {
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
    T buf[size], *tail, *end, *st[size];
    int top;
    MemoryPool() : top(0), tail(buf), end(buf + size) {}
    inline T *alloc() {
        if (top) return st[--top];
        if (tail != end) return tail++;
        return new T;
    }
    inline void clear() { top = 0, tail = buf; }
    inline void recycle(T *x) { top > size ? delete x : st[++top] = x; }
};
#define MAXN 100100
struct Node {
    Node *lc, *rc;
    int cnt;
    inline Node *init(Node *lc, Node *rc, int cnt) {
        return this->lc = lc, this->rc = rc, this->cnt = cnt, this;
    }
    inline Node *clear() { return lc = rc = NULL, cnt = 0, this; }
    inline int rank() const { return lc ? lc->cnt : 0; }
} * tree[MAXN];
MemoryPool<Node, MAXN * 40> pool;
inline Node *build(const Node *p, const int l, const int r, const int value) {
    if (l == r) return pool.alloc()->init(tree[0], tree[0], p->cnt + 1);
    register int mid = l + r >> 1;
    if (value <= mid)
        return pool.alloc()->init(build(p->lc, l, mid, value), p->rc,
                                  p->cnt + 1);
    else
        return pool.alloc()->init(p->lc, build(p->rc, mid + 1, r, value),
                                  p->cnt + 1);
}
inline int query(const Node *p1, const Node *p2, const Node *p3, const Node *p4,
                 const int l, const int r, const int k) {
    register int mid = l + r >> 1;
    if (l == r) return mid;
    register int tmp = p1->rank() + p2->rank() - p3->rank() - p4->rank();
    if (k <= tmp)
        return query(p1->lc, p2->lc, p3->lc, p4->lc, l, mid, k);
    else
        return query(p1->rc, p2->rc, p3->rc, p4->rc, mid + 1, r, k - tmp);
}
struct Edge {
    int to, next;
} edge[MAXN << 1];
int head[MAXN], tot;
int n, m, ans;
int a[MAXN], fa[MAXN][20], dpt[MAXN];
std::pair<int, int> b[MAXN];
inline void addEdge(int x, int y) {
    edge[++tot].to = y;
    edge[tot].next = head[x];
    head[x] = tot;
}
inline void dfs(int x) {
    int i;
    dpt[x] = dpt[fa[x][0]] + 1;
    tree[x] = build(tree[fa[x][0]], 1, n, a[x]);
    for (i = head[x]; i; i = edge[i].next) {
        if (edge[i].to == fa[x][0]) continue;
        fa[edge[i].to][0] = x;
        dfs(edge[i].to);
    }
}
int lcaQuery(int x, int y) {
    int j;
    if (dpt[x] < dpt[y]) std::swap(x, y);
    for (j = 19; ~j; j--)
        if (dpt[fa[x][j]] >= dpt[y]) x = fa[x][j];
    if (x == y) return x;
    for (j = 19; ~j; j--)
        if (fa[x][j] != fa[y][j]) x = fa[x][j], y = fa[y][j];
    return fa[x][0];
}
int Query(int x, int y, int k) {
    int lca = lcaQuery(x, y);
    return query(tree[x], tree[y], tree[lca], tree[fa[lca][0]], 1, n, k);
}
int main() {
    read(n), read(m);
    for (register int i = 1; i <= n; i++) read(b[i].first), b[i].second = i;
    std::sort(b + 1, b + n + 1);
    for (register int i = 1; i <= n; i++) a[b[i].second] = i;
    for (register int i = 1, x, y; i < n; i++)
        read(x), read(y), addEdge(x, y), addEdge(y, x);
    tree[0] = pool.alloc()->init(0x0, 0x0, 0),
    tree[0]->lc = tree[0]->rc = tree[0];
    dfs(1);
    for (register int j = 1; j <= 19; j++)
        for (register int i = 1; i <= n; i++)
            fa[i][j] = fa[fa[i][j - 1]][j - 1];
    for (register int i = 1, x, y, k; i <= m; i++) {
        read(x), read(y), read(k);
        std::cout << (ans = b[Query(x ^ ans, y, k)].first);
        if (i != m) std::cout << "\n";
    }
    return 0;
}
