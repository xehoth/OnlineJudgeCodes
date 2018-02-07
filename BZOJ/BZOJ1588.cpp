#include <bits/stdc++.h>

namespace Treap {

const int MAXN = 42767;

struct Node *null;

struct Node {
    Node *lc, *rc;

    int key, rank, size;

    Node(int key = 0) : key(key), rank(rand()), lc(null), rc(null), size(1) {}

    inline void maintain() { size = lc->size + rc->size + 1; }

    inline void *operator new(size_t);
} pool[MAXN], *cur = pool, *root;

inline void *Node::operator new(size_t) { return cur++; }

typedef std::pair<Node *, Node *> Pair;

inline Node *merge(Node *u, Node *v) {
    if (u == null) return v;
    if (v == null) return u;
    if (u->rank < v->rank)
        return u->rc = merge(u->rc, v), u->maintain(), u;
    else
        return v->lc = merge(u, v->lc), v->maintain(), v;
}

inline Pair split(Node *u, int k) {
    if (u == null) return Pair(null, null);
    Pair t;
    if (u->lc->size >= k)
        t = split(u->lc, k), u->lc = t.second, t.second = u;
    else
        t = split(u->rc, k - u->lc->size - 1), u->rc = t.first, t.first = u;
    return u->maintain(), t;
}

inline int rank(Node *u, int x) {
    if (u == null) return 0;
    return x <= u->key ? rank(u->lc, x) : rank(u->rc, x) + u->lc->size + 1;
}

inline void insert(int x) {
    register int k = rank(root, x);
    Pair t = split(root, k);
    root = merge(merge(t.first, new Node(x)), t.second);
}

inline int select(int k) {
    Pair t1 = split(root, k - 1), t2 = split(t1.second, 1);
    Node *ret = t2.first;
    root = merge(merge(t1.first, ret), t2.second);
    return ret->key;
}

inline int precursor(int x) { return select(rank(root, x)); }

inline int successor(int x) { return select(rank(root, x) + 1); }

inline int query(int x) {
    register int pre = precursor(x), suc = successor(x);
    return std::min(std::abs(pre - x), std::abs(suc - x));
}

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register int n, x, ans = 0;
    srand(495), std::cin >> n, root = null = new Node();
    null->lc = null->rc = null, null->size = 0;
    while (n--) std::cin >> x, ans += query(x), insert(x);
    std::cout << ans;
}
}  // namespace Treap

int main() {
    Treap::solve();
    return 0;
}