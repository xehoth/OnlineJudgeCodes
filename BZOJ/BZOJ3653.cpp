#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
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

const int OUT_LEN = 1000000;
char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        if (x < 0) print('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace IO

namespace SegmentTree {

#define long long long

const int MAXN = 300000 + 10, MAX_LOG = 20;

struct Node *null;

struct Node {
    Node *lc, *rc;
    long s;

    Node(long s = 0) : s(s), lc(null), rc(null) {}

    inline void *operator new(size_t);
} pool[MAXN * MAX_LOG], *cur = pool, *root[MAXN];

inline void *Node::operator new(size_t) { return cur++; }

inline void insert(Node *&x, Node *y, int l, int r, int key, int val) {
    x = new Node(*y), x->s += val;
    if (l == r) return;
    register int mid = l + r >> 1;
    if (key <= mid)
        insert(x->lc, y->lc, l, mid, key, val);
    else
        insert(x->rc, y->rc, mid + 1, r, key, val);
}

inline long query(Node *x, Node *y, int l, int r, int s, int t) {
    if (s <= l && r <= t) return y->s - x->s;
    register long ret = 0;
    register int mid = l + r >> 1;
    if (s <= mid) ret += query(x->lc, y->lc, l, mid, s, t);
    if (mid < t) ret += query(x->rc, y->rc, mid + 1, r, s, t);
    return ret;
}

std::vector<int> edge[MAXN];

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

int in[MAXN], idx, out[MAXN], sz[MAXN], dep[MAXN], maxDep, seq[MAXN];

inline void dfs(int u) {
    maxDep = std::max(maxDep, dep[u]), in[u] = ++idx, seq[idx] = u, sz[u] = 1;
    for (register int i = 0, v; i < edge[u].size(); i++)
        if (!dep[v = edge[u][i]]) dep[v] = dep[u] + 1, dfs(v), sz[u] += sz[v];
    out[u] = idx;
}

inline void solve() {
    using namespace IO;
    register int n, m;
    read(n), read(m);
    for (register int i = 1, u, v; i < n; i++) read(u), read(v), addEdge(u, v);
    dep[1] = 1, dfs(1);
    root[0] = null = new Node();
    null->lc = null->rc = null;
    for (register int i = 1; i <= n; i++)
        insert(root[i], root[i - 1], 1, maxDep, dep[seq[i]], sz[seq[i]] - 1);
    for (register int i = 1, p, k; i <= m; i++) {
        read(p), read(k);
        register long res = (long)std::min(dep[p] - 1, k) * (sz[p] - 1);
        res += query(root[in[p]], root[out[p]], 1, maxDep, dep[p] + 1,
                     std::min(dep[p] + k, maxDep));
        print(res), print('\n');
    }
}

#undef long
}

int main() {
    SegmentTree::solve();
    IO::flush();
    return 0;
}
