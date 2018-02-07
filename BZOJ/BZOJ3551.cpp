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
    static bool iosig = 0;
    static char ioc;
    for (iosig = 0, ioc = read(); !isdigit(ioc); ioc = read())
        if (ioc == '-') iosig = 1;
    for (x = 0; isdigit(ioc); ioc = read())
        x = (x << 1) + (x << 3) + (ioc ^ '0');
    if (iosig) x = -x;
}
template <class T, size_t size>
struct MemoryPool {
    T buf[size], *tail, *end;
    int top;
    inline T *alloc() {
        if (tail != end) return tail++;
        return new T;
    }
    MemoryPool() : top(0), tail(buf), end(buf + size) {}
};
const int MAXN = 300100;
const int INF = (INT_MAX >> 1) - 1;
struct Data {
    int u, v, w;
    inline bool operator<(const Data &b) const { return w < b.w; }
} data[MAXN << 1];
int n, m, tmp[MAXN], tmpSize, a[MAXN], q, cnt;
struct Edge {
    Edge *next;
    int v;
    Edge() : next(NULL) {}
    inline void init(const int v, Edge *next) {
        this->v = v, this->next = next;
    }
} edge[MAXN << 2];
Edge *head[MAXN], *cur = edge;
inline void addEdge(int u, int v) { (++cur)->init(v, head[u]), head[u] = cur; }
struct UnionFindSet {
    int father[MAXN + 100];
    inline int &operator[](const int i) { return father[i]; }
    inline int get(const int x) {
        return x == father[x] ? x : father[x] = get(father[x]);
    }
} ufs;
struct Node {
    Node *lc, *rc;
    int cnt;
    inline Node *init(Node *lc, Node *rc, const int cnt) {
        return this->lc = lc, this->rc = rc, this->cnt = cnt, this;
    }
    inline Node *clear() { return lc = rc = NULL, cnt = 0, this; }
    inline int rank() const { return lc ? lc->cnt : 0; }
} * root[MAXN + 100];
MemoryPool<Node, 5000000> pool;
inline Node *build(const Node *p, const int l, const int r, const int value) {
    if (l == r) return pool.alloc()->init(0x0, 0x0, p->cnt + 1);
    register int mid = l + r >> 1;
    if (value <= mid)
        return pool.alloc()->init(build(p->lc, l, mid, value), p->rc,
                                  p->cnt + 1);
    else
        return pool.alloc()->init(p->lc, build(p->rc, mid + 1, r, value),
                                  p->cnt + 1);
}
inline int query(const Node *p1, const Node *p2, const int l, const int r,
                 const int k) {
    if (l == r) return tmp[l];
    register int mid = l + r >> 1, sum = p2->rc->cnt - p1->rc->cnt;
    if (k <= sum)
        return query(p1->rc, p2->rc, mid + 1, r, k);
    else
        return query(p1->lc, p2->lc, l, mid, k - sum);
}
int dep[MAXN], father[MAXN][20], st[MAXN], ed[MAXN], idx;
inline void dfs(const int x) {
    dep[x] = dep[father[x][0]] + 1, st[x] = ++idx;
    for (register int i = 1; i <= 18; i++) {
        if (dep[x] < (1 << i)) break;
        father[x][i] = father[father[x][i - 1]][i - 1];
    }
    if (x <= n)
        root[idx] = build(root[idx - 1], 1, tmpSize, a[x]);
    else
        root[idx] = root[idx - 1];
    for (Edge *p = head[x]; p; p = p->next) father[p->v][0] = x, dfs(p->v);
    ed[x] = idx;
}
int main() {
    //    freopen("in.in", "r", stdin);
    read(n), read(m), read(q), cnt = n;
    for (register int i = 1; i <= n; i++) read(a[i]), tmp[i] = a[i];
    std::sort(tmp + 1, tmp + n + 1), tmp[0] = -INF;
    for (int i = 1; i <= n; ++i)
        if (tmp[i] != tmp[i - 1]) tmp[++tmpSize] = tmp[i];
    for (register int i = 1; i <= n; i++)
        a[i] = std::lower_bound(tmp + 1, tmp + tmpSize + 1, a[i]) - tmp;
    for (register int i = 1; i <= m; i++)
        read(data[i].u), read(data[i].v), read(data[i].w);
    std::sort(data + 1, data + m + 1);
    for (register int i = 1; i < n << 1; i++) ufs[i] = i;
    root[0] = pool.alloc()->init(0x0, 0x0, 0),
    root[0]->lc = root[0]->rc = root[0];
    for (register int i = 1; i <= m; i++) {
        register int x = ufs.get(data[i].u), y = ufs.get(data[i].v);
        if (x != y) {
            ufs[x] = ufs[y] = ++cnt, addEdge(cnt, x), addEdge(cnt, y),
            a[cnt] = data[i].w;
            if (cnt == (n << 1) - 1) break;
        }
    }
    for (register int i = 1, x; i <= n; i++) {
        x = ufs.get(i);
        if (!st[x]) dfs(x);
    }
    register int preAns = 0;
    a[0] = INF;
    for (register int i = 1, x, y, z; i <= q; i++) {
        read(x), read(y), read(z), x ^= preAns, y ^= preAns, z ^= preAns;
        register int rt = x;
        for (register int j = 18; j >= 0; j--) {
            if (dep[rt] < (1 << j)) continue;
            if (a[father[rt][j]] <= y) rt = father[rt][j];
        }
        if (root[ed[rt]]->cnt - root[st[rt] - 1]->cnt < z)
            preAns = -1;
        else
            preAns = query(root[st[rt] - 1], root[ed[rt]], 1, tmpSize, z);
        std::cout << preAns << "\n";
        preAns = preAns < 0 ? 0 : preAns;
    }
    return 0;
}