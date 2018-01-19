/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 4763」雪辉 11-01-2018
 * 树分块 + std::bitset
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
    static int buf[21], cnt;
    if (x != 0) {
        (x < 0) && (print('-'), x = -x);
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
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

const int MAXN = 100000 + 9;
const int BLOCK_SIZE = 319;
const int MAXV = 30000 + 1;

typedef std::bitset<MAXV> BitSet;

BitSet d[BLOCK_SIZE + 2][BLOCK_SIZE + 2];

std::vector<int> edge[MAXN];

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v);
    edge[v].push_back(u);
}

int id[MAXN], max[MAXN], dep[MAXN], fa[MAXN], son[MAXN], sz[MAXN], top[MAXN];
int root[MAXN], rootCnt, w[MAXN], topId[MAXN];
bool vis[MAXN];

void dfs1(const int u) {
    dep[u] = dep[fa[u]] + 1;
    max[u] = dep[u];
    vis[u] = true;
    sz[u] = 1;
    for (register int i = 0, v; i < (int)edge[u].size(); i++) {
        if (!vis[v = edge[u][i]]) {
            fa[v] = u;
            dfs1(v);
            max[u] = std::max(max[u], max[v]);
            sz[u] += sz[v];
            (sz[v] > sz[son[u]]) && (son[u] = v);
        }
    }
    if (max[u] - dep[u] >= BLOCK_SIZE || u == 1) {
        max[u] = 0;
        root[id[u] = ++rootCnt] = u;
    }
}

void dfs2(const int u) {
    vis[u] = false;
    top[u] = (u == son[fa[u]] ? top[fa[u]] : u);
    if (son[u]) dfs2(son[u]);
    for (register int i = 0, v; i < (int)edge[u].size(); i++)
        if (vis[v = edge[u][i]]) dfs2(v);
}

inline int lca(register int u, register int v) {
    while (top[u] != top[v])
        dep[top[u]] < dep[top[v]] ? v = fa[top[v]] : u = fa[top[u]];
    return dep[u] < dep[v] ? u : v;
}

BitSet tmp;

inline void solve() {
    register int n, m, flag;
    io >> n >> m >> flag;
    for (register int i = 1; i <= n; i++) io >> w[i];
    for (register int i = 1, u, v; i < n; i++) {
        io >> u >> v;
        addEdge(u, v);
    }
    dfs1(1);
    dfs2(1);
    for (register int i = 1; i <= rootCnt; i++) {
        tmp.reset();
        tmp.set(w[root[i]]);
        d[i][i] = tmp;
        for (register int k = fa[root[i]]; k; k = fa[k]) {
            tmp.set(w[k]);
            if (id[k]) {
                d[i][id[k]] = tmp;
                if (!topId[root[i]]) topId[root[i]] = k;
            }
        }
    }
    register int last = 0;
    for (register int a; m--;) {
        tmp.reset();
        io >> a;
        for (register int i = 1, u, v; i <= a; i++) {
            io >> u >> v;
            if (flag) {
                u ^= last;
                v ^= last;
            }
            register int l = lca(u, v);
            tmp.set(w[u]);
            tmp.set(w[v]);
            for (; !id[u] && dep[u] > dep[l];) {
                u = fa[u];
                tmp.set(w[u]);
            }
            for (; !id[v] && dep[v] > dep[l];) {
                v = fa[v];
                tmp.set(w[v]);
            }
            register int tu = u, tv = v;
            for (; dep[topId[tu]] >= dep[l]; tu = topId[tu])
                ;
            for (; dep[topId[tv]] >= dep[l]; tv = topId[tv])
                ;
            tmp |= d[id[u]][id[tu]];
            tmp |= d[id[v]][id[tv]];
            for (; tu != tv;) {
                (dep[tu] < dep[tv]) && (std::swap(tu, tv), 0);
                tu = fa[tu];
                tmp.set(w[tu]);
            }
        }
        register int ans1 = tmp.count();
        tmp.flip();
        register int ans2 = tmp._Find_first();
        last = ans1 + ans2;
        io << ans1 << ' ' << ans2 << '\n';
    }
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}