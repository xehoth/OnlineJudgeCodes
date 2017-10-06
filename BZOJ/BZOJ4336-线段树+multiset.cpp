/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4336」骑士的旅行 06-10-2017
 * 线段树 + multiset
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

using IO::io;

const int MAXN = 40005;

int n, m, q, k;
std::multiset<int, std::greater<int> > set[MAXN];
int F[MAXN], P[MAXN];
int fa[MAXN], sz[MAXN], son[MAXN], dep[MAXN];
int top[MAXN], id[MAXN], dfn[MAXN], idx;
int mergeL, mergeR, *heap[MAXN], heapSize;

inline bool cmp(int *a, int *b) { return *a < *b; }

inline void push(int *a) {
    heap[heapSize++] = a, std::push_heap(heap, heap + heapSize, cmp);
}

struct Node {
    Node *lc, *rc, *fa;
    int l, r;
    int v[21];

    inline void init(int x) {
        int p = 0;
        for (std::multiset<int>::iterator it = set[x].begin();
             p < k && it != set[x].end(); v[p++] = *(it++))
            ;
        for (; p < k; v[p++] = -1)
            ;
    }

    inline void maintain() {
        register int p = 0, p1 = 0, p2 = 0, *v1 = lc->v, *v2 = rc->v;
        while (p < k && p1 < k && p2 < k)
            v[p++] = v1[p1] > v2[p2] ? v1[p1++] : v2[p2++];
        while (p < k && p1 < k) v[p++] = v1[p1++];
        while (p < k && p2 < k) v[p++] = v2[p2++];
    }

    inline void update() {
        for (Node *w = fa; w; w->maintain(), w = w->fa)
            ;
    }

    inline void merge() {
        if (mergeL <= l && r <= mergeR) {
            (~v[0]) ? push(v) : (void)0;
            return;
        }
        register int mid = l + r >> 1;
        if (mergeL <= mid) lc->merge();
        if (mergeR > mid) rc->merge();
    }
} pool[MAXN * 2], *cur = pool, *root[MAXN], *pos[MAXN];

inline Node *build(int l, int r) {
    Node *p = cur++;
    p->l = l, p->r = r, memset(p->v, -1, sizeof(p->v));
    if (l != r) {
        register int mid = l + r >> 1;
        (p->lc = build(l, mid))->fa = (p->rc = build(mid + 1, r))->fa = p;
        p->maintain();
    } else {
        pos[dfn[l]] = p, p->init(dfn[l]);
    }
    return p;
}

std::vector<int> edge[MAXN + 1];

typedef std::vector<int>::iterator Iterator;

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

inline void dfs1(int u, int pre) {
    dep[u] = dep[fa[u] = pre] + 1, sz[u] = 1;
    for (register Iterator v = edge[u].begin(); v != edge[u].end(); v++) {
        if (*v != pre) {
            dfs1(*v, u), sz[u] += sz[*v];
            sz[*v] > sz[son[u]] ? son[u] = *v : 0;
        }
    }
}

inline void dfs2(int u, int pre) {
    top[u] = pre, dfn[id[u] = ++idx] = u;
    (son[u]) ? dfs2(son[u], pre) : (void)(root[pre] = build(id[pre], id[u]));
    for (register Iterator v = edge[u].begin(); v != edge[u].end(); v++)
        if (*v != fa[u] && *v != son[u]) dfs2(*v, *v);
}

inline void solve() {
    io >> n;
    for (register int i = 1, u, v; i < n; i++) {
        io >> u >> v;
        addEdge(u, v);
    }
    io >> m;
    for (int i = 1; i <= m; ++i) {
        io >> F[i] >> P[i];
        set[P[i]].insert(F[i]);
    }
    io >> q >> k;
    dfs1(1, 0), dfs2(1, 1);
    for (register int i = 0, cmd, x, y; i < q; ++i) {
        io >> cmd >> x >> y;
        switch (cmd) {
            case 1: {
                for (heapSize = 0; top[x] != top[y];) {
                    dep[top[x]] < dep[top[y]] ? std::swap(x, y) : (void)0;
                    mergeL = id[top[x]], mergeR = id[x], root[top[x]]->merge();
                    x = fa[top[x]];
                }
                dep[x] > dep[y] ? std::swap(x, y) : (void)0;
                mergeL = id[x], mergeR = id[y], root[top[x]]->merge();
                if (!heapSize) io << "-1";
                for (register int j = 0; j < k && heapSize; ++j) {
                    register int *p = heap[0];
                    std::pop_heap(heap, heap + heapSize--, cmp);
                    io << *p << ' ';
                    (~*++p) ? push(p) : (void)0;
                }
                io << '\n';
                break;
            }
            case 2: {
                set[P[x]].erase(set[P[x]].find(F[x])), pos[P[x]]->init(P[x]);
                pos[P[x]]->update(), set[P[x] = y].insert(F[x]);
                pos[P[x]]->init(P[x]), pos[P[x]]->update();
                break;
            }
            case 3: {
                set[P[x]].erase(set[P[x]].find(F[x]));
                set[P[x]].insert(F[x] = y), pos[P[x]]->init(P[x]);
                pos[P[x]]->update();
                break;
            }
        }
    }
}
}

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    solve();
    return 0;
}