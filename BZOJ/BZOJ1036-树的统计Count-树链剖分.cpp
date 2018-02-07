/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1036」树的统计Count 14-05-2017
 * 树链剖分
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

inline __attribute__((always_inline)) char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <class T>
inline __attribute__((always_inline)) void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline __attribute__((always_inline)) void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <class T>
inline __attribute__((always_inline)) void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline __attribute__((always_inline)) void flush() {
    fwrite(obuf, 1, oh - obuf, stdout);
}
}  // namespace IO

const int MAXN = 30010;

struct Node {
    int v, w;
    Node() {}
    Node(int v, int w) : v(v), w(w) {}
};

std::vector<Node> edge[MAXN];

int dep[MAXN];     //当前节点的深度
int father[MAXN];  //当前节点的父亲
int size[MAXN];    //以当前节点为根的子树节点个树
int son[MAXN];     //当前节点的重儿子
int top[MAXN];     //当前节点所在链的顶端节点
int pos[MAXN];     //当前节点在线段树中的编号
int idx;           // dfs序
bool vis[MAXN];

inline void dfs1(int u) {
    dep[u] = dep[father[u]] + 1, size[u] = 1, vis[u] = true;
    for (register int i = 0; i < edge[u].size(); i++) {
        Node *x = &edge[u][i];
        if (!vis[x->v]) {
            register int pre = x->v;
            father[pre] = u;
            dfs1(pre);
            size[u] += size[pre];
            if (size[pre] > size[son[u]]) son[u] = pre;
        }
    }
}

inline void dfs2(int u) {
    vis[u] = false, pos[u] = idx++,
    top[u] = (u == son[father[u]] ? top[father[u]] : u);
    for (register int i = 0; i < edge[u].size(); i++)
        if (edge[u][i].v == son[u]) dfs2(edge[u][i].v);
    for (register int i = 0; i < edge[u].size(); i++)
        if (vis[edge[u][i].v]) dfs2(edge[u][i].v);
}

struct SegmentNode {
    int sum, max;
    SegmentNode() {}
    SegmentNode(int sum, int max) : sum(sum), max(max) {}
};

struct SegmentTree {
    int M, size;

    SegmentNode data[32768 << 2 | 1];

    inline __attribute__((always_inline)) void init(int n) {
        for (M = 1; M < n; M <<= 1)
            ;
        size = M << 1 | 1;
        for (register int i = n; i < M; i++)
            data[i + M] = SegmentNode(0, INT_MIN);
    }

    inline __attribute__((always_inline)) void build() {
        for (register int i = M - 1; i; i--) {
            data[i].sum = data[i << 1].sum + data[i << 1 | 1].sum;
            data[i].max = std::max(data[i << 1].max, data[i << 1 | 1].max);
        }
    }

    inline __attribute__((always_inline)) void modify(const int pos,
                                                      const int val) {
        register int i = pos + M;
        for (data[i].sum = data[i].max = val, i >>= 1; i; i >>= 1) {
            data[i].sum = data[i << 1].sum + data[i << 1 | 1].sum;
            data[i].max = std::max(data[i << 1].max, data[i << 1 | 1].max);
        }
    }

    inline __attribute__((always_inline)) int getMax(int s, int t) {
        register int ans = INT_MIN;
        for (s = s + M - 1, t = t + M + 1; s ^ t ^ 1; s >>= 1, t >>= 1) {
            if (~s & 1) ans = std::max(ans, data[s ^ 1].max);
            if (t & 1) ans = std::max(ans, data[t ^ 1].max);
        }
        return ans;
    }

    inline __attribute__((always_inline)) int getSum(int s, int t) {
        register int ans = 0;
        for (s = s + M - 1, t = t + M + 1; s ^ t ^ 1; s >>= 1, t >>= 1) {
            if (~s & 1) ans += data[s ^ 1].sum;
            if (t & 1) ans += data[t ^ 1].sum;
        }
        return ans;
    }
} tree;

inline __attribute__((always_inline)) int getLCA(int u, int v) {
    while (top[u] ^ top[v])
        dep[top[u]] < dep[top[v]] ? v = father[top[v]] : u = father[top[u]];
    return dep[u] < dep[v] ? u : v;
}

inline __attribute__((always_inline)) int getMax(int u, int v) {
    register int ans = INT_MIN;
    while (top[u] ^ top[v]) {
        dep[top[u]] < dep[top[v]] ? (std::swap(u, v)) : (void)0;
        ans = std::max(ans, tree.getMax(pos[top[u]], pos[u]));
        u = father[top[u]];
    }
    dep[u] > dep[v] ? (std::swap(u, v)) : (void)0;
    return std::max(ans, tree.getMax(pos[u], pos[v]));
}

inline __attribute__((always_inline)) int getSum(int u, int v) {
    register int ans = 0;
    while (top[u] ^ top[v]) {
        dep[top[u]] < dep[top[v]] ? (std::swap(u, v)) : (void)0;
        ans += tree.getSum(pos[top[u]], pos[u]);
        u = father[top[u]];
    }
    dep[u] > dep[v] ? (std::swap(u, v)) : (void)0;
    return ans + tree.getSum(pos[u], pos[v]);
}

inline void cut(int root = 1) { dfs1(root), dfs2(root); }

inline void addEdge(int u, int v, int w = 0) {
    edge[u].push_back(Node(v, w));
    edge[v].push_back(Node(u, w));
}

int n, w[MAXN];

int main() {
    using namespace IO;
    // freopen("in.in", "r", stdin);
    read(n);
    tree.init(n);
    for (register int i = 1, u, v; i < n; i++) read(u), read(v), addEdge(u, v);
    cut();
    for (register int i = 1; i <= n; i++)
        read(w[i]), tree.data[pos[i] + tree.M] = SegmentNode(w[i], w[i]);
    tree.build();
    register int q, x, y;
    read(q);
    register char opt;
    while (q--) {
        opt = read();
        while (!isupper(opt)) opt = read();
        if (opt == 'C') {
            read(x), read(y);
            tree.modify(pos[x], y);
            w[x] = y;
        } else {
            opt = read();
            read(x), read(y);
            register int lca = getLCA(x, y);
            if (opt == 'M')
                print(getMax(x, y)), print('\n');
            else
                print(getSum(x, y)), print('\n');
        }
    }
    flush();
    return 0;
}
