/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4530」大融合 07-10-2017
 * 线段树合并
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

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

inline void read(char &c) {
    while (c = read(), isspace(c) && c != -1)
        ;
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

inline void print(const char *s) {
    for (; *s; s++) print(*s);
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
}

namespace {

const int MAXN = 100000;
const int MAX_LOG = 17;

#define long long long

struct Node {
    Node *lc, *rc;
    int size;

    Node();

    inline void *operator new(size_t);
} pool[MAXN * MAX_LOG + 1], *cur = pool + 1, *null = pool;

inline void *Node::operator new(size_t) { return cur++; }

Node::Node() : size(0) { lc = null, rc = null; }

Node *root[MAXN + 1];
int n, q;

inline Node *merge(Node *u, Node *v) {
    if (u == null) return v;
    if (v == null) return u;
    u->lc = merge(u->lc, v->lc);
    u->rc = merge(u->rc, v->rc);
    u->size += v->size;
    return u;
}

inline void insert(Node *&p, int l, int r, int v) {
    if (p == null) p = new Node();
    p->size++;
    if (l == r) return;
    register int mid = l + r >> 1;
    v <= mid ? insert(p->lc, l, mid, v) : insert(p->rc, mid + 1, r, v);
}

inline int query(Node *p, int l, int r, int s, int t) {
    if (s <= l && t >= r) return p->size;
    register int mid = l + r >> 1;
    return (s <= mid ? query(p->lc, l, mid, s, t) : 0) +
           (t > mid ? query(p->rc, mid + 1, r, s, t) : 0);
}

std::vector<int> edge[MAXN + 1];

typedef std::vector<int>::iterator Iterator;

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

int idx, in[MAXN + 1], out[MAXN + 1], dep[MAXN + 1], fa[MAXN + 1];
bool vis[MAXN + 1];

inline void dfs(const int u) {
    vis[u] = true, in[u] = ++idx;
    insert(root[u], 1, n, idx);
    for (Iterator v = edge[u].begin(); v != edge[u].end(); v++)
        if (!vis[*v]) dep[*v] = dep[u] + 1, dfs(*v);
    out[u] = idx;
}

inline int get(int x) { return x == fa[x] ? x : fa[x] = get(fa[x]); }

using IO::io;

struct Query {
    int cmd, x, y;
} que[MAXN + 1];

inline void solve() {
    io >> n >> q;
    for (register int i = 1; i <= q; i++) {
        register char c;
        io >> c >> que[i].x >> que[i].y;
        switch (c) {
            case 'A':
                que[i].cmd = 1, addEdge(que[i].x, que[i].y);
                break;
            case 'Q':
                que[i].cmd = 2;
                break;
        }
    }
    for (register int i = 0; i <= n; i++) root[i] = null;
    for (register int i = 1; i <= n; i++)
        if (!vis[i]) dfs(i);
    for (register int i = 1; i <= n; i++) fa[i] = i;
    for (register int i = 1; i <= q; i++) {
        switch (que[i].cmd) {
            case 1: {
                register int x = get(que[i].x), y = get(que[i].y);
                dep[x] > dep[y] ? std::swap(x, y) : (void)0;
                root[x] = merge(root[x], root[y]);
                fa[y] = x;
                break;
            }
            case 2: {
                register int x = que[i].x, y = que[i].y;
                dep[x] < dep[y] ? std::swap(x, y) : (void)0;
                register int father = get(x);
                register int v = query(root[father], 1, n, in[x], out[x]);
                io << (long)(root[father]->size - v) * v << '\n';
                break;
            }
        }
    }
}

#undef long
}

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    solve();
    return 0;
}