/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 3730」震波 17-01-2018
 * 点分树
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
const int MAX_LOG = 23;

std::vector<int> edge[MAXN];

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v);
    edge[v].push_back(u);
}

bool vis[MAXN];
int sz[MAXN], dep[MAXN], fa[MAXN][MAX_LOG], dis[MAXN][MAX_LOG];
std::vector<int> bit[MAXN], fbit[MAXN];

void dfsSize(int u, int pre) {
    sz[u] = 1;
    for (register int i = 0, v; i < (int)edge[u].size(); i++) {
        if (!vis[v = edge[u][i]] && v != pre) {
            dfsSize(v, u);
            sz[u] += sz[v];
        }
    }
}

int get(int u, int pre, int size) {
    for (register int i = 0, v; i < (int)edge[u].size(); i++)
        if (!vis[v = edge[u][i]] && v != pre && sz[v] > size)
            return get(v, u, size);
    return u;
}

void dfs(int u, int pre, int d, int root) {
    for (register int i = 0, v; i < (int)edge[u].size(); i++) {
        if (!vis[v = edge[u][i]] && v != pre) {
            fa[v][d] = root;
            dis[v][d] = dis[u][d] + 1;
            dfs(v, u, d, root);
        }
    }
}

void build(int u, int d) {
    dfsSize(u, 0);
    register int size = sz[u];
    vis[u = get(u, 0, sz[u] / 2)] = true;
    dep[u] = d;
    bit[u].resize(size + 1);
    fbit[u].resize(size + 1);
    for (register int i = 0, v; i < (int)edge[u].size(); i++) {
        if (!vis[v = edge[u][i]]) {
            dis[v][d] = 1;
            fa[v][d] = u;
            dfs(v, u, d, u);
            build(v, d + 1);
        }
    }
}

int n, m, val[MAXN], ans;

inline int queryS(int x, register int k) {
    register int ret = val[x];
    for (k = std::min(k, (int)bit[x].size() - 1); k; k ^= k & -k)
        ret += bit[x][k];
    return ret;
}

inline int queryF(int x, register int k) {
    register int ret = 0;
    for (k = std::min(k, (int)fbit[x].size() - 1); k; k ^= k & -k)
        ret += fbit[x][k];
    return ret;
}

inline void modify(int x, int v) {
    register int d = dis[x][dep[x]], limit = (int)bit[x].size() - 1;
    for (register int j = d; j <= limit && j; j += j & -j) fbit[x][j] += v;
    for (register int i = dep[x]; i; i--) {
        d = dis[x][i];
        limit = (int)bit[fa[x][i]].size() - 1;
        for (register int j = d; j <= limit; j += j & -j) bit[fa[x][i]][j] += v;
        d = dis[x][i - 1];
        for (register int j = d; j <= limit && j; j += j & -j)
            fbit[fa[x][i]][j] += v;
    }
}

inline int query(int x, int k) {
    register int ret = queryS(x, k);
    for (register int i = dep[x]; i; i--)
        if (dis[x][i] <= k)
            ret += queryS(fa[x][i], k - dis[x][i]) -
                   queryF(fa[x][i + 1], k - dis[x][i]);
    return ret;
}

inline void solve() {
    io >> n >> m;
    for (register int i = 1; i <= n; i++) io >> val[i];
    for (register int i = 1, u, v; i < n; i++) {
        io >> u >> v;
        addEdge(u, v);
    }
    build(1, 1);
    for (register int i = 1; i <= n; i++) fa[i][dep[i]--] = i;
    for (register int i = 1; i <= n; i++) modify(i, val[i]);

    for (register int cmd, u, v; m--;) {
        io >> cmd >> u >> v;
        u ^= ans;
        v ^= ans;
        if (cmd == 0) {
            ans = query(u, v);
            io << ans << '\n';
        } else {
            modify(u, v - val[u]);
            val[u] = v;
        }
    }
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}