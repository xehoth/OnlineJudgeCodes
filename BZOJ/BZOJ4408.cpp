#include <bits/stdc++.h>
inline char read() {
    static const int IO_LEN = 1024 * 1024;
    static char buf[IO_LEN], *ioh, *iot;
    if (iot == ioh) {
        iot = (ioh = buf) + fread(buf, 1, IO_LEN, stdin);
        if (iot == ioh) return -1;
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
const int MAXN = 1000001;
struct Node {
    int l, r, data;
    Node *lc, *rc;
    Node() : lc(NULL), rc(NULL) {}
} * root[MAXN], *null;
template <class T, size_t size>
struct MemoryPool {
    T buf[size], *tail, *end;
    MemoryPool() : tail(buf), end(buf + size) {}
    inline T *alloc() { return tail != end ? tail++ : new T; }
};
MemoryPool<Node, MAXN> pool;
inline Node *newNode() {
    Node *p = pool.alloc();
    return p->lc = null = p->rc = null, p->data = 0, p;
}
inline Node *build(const Node *p, const int l, const int r, const int v) {
    Node *res;
    if (l != r) {
        if (p == null) {
            res = newNode(), res->l = l, res->r = r, res->data += v;
            register int mid = l + r >> 1;
            if (mid < v)
                res->rc = build(null, mid + 1, r, v);
            else
                res->lc = build(null, l, mid, v);
            return res;
        } else {
            res = newNode(), *res = *p, res->data += v;
            register int mid = l + r >> 1;
            if (mid < v)
                res->rc = build(p->rc, mid + 1, r, v);
            else
                res->lc = build(p->lc, l, mid, v);
            return res;
        }
    } else {
        res = newNode();
        if (p != null) *res = *p;
        return res->l = l, res->r = r, res->data += v, res;
    }
}
inline int query(const Node *p, Node *cur, int r) {
    if (cur->r <= r) return cur->data - p->data;
    register int mid = cur->l + cur->r >> 1, res = 0;
    if (mid < r)
        if (cur->rc != null) res += query(p->rc, cur->rc, r);
    if (cur->lc != null) res += query(p->lc, cur->lc, r);
    return res;
}
int main() {
    //    freopen("in.in", "r", stdin);
    null = pool.alloc(), null->lc = null->rc = null, null->data = 0,
    root[0] = newNode(), root[0]->l = 0, root[0]->r = 1000000001,
    root[0]->data = 0;
    register int n, m, tmp = 0, last = 1;
    read(n);
    for (register int i = 1, j; i <= n; i++)
        read(j), root[i] = build(root[i - 1], 0, root[i - 1]->r, j);
    read(m);
    for (register int i = 1, j, k; i <= m; i++) {
        read(j), read(k);
        if (j > k) std::swap(j, k);
        tmp = 0, last = 1;
        while (tmp ^ last) {
            last = tmp;
            tmp = query(root[j - 1], root[k], tmp + 1);
        }
        std::cout << tmp + 1 << "\n";
    }
    return 0;
}