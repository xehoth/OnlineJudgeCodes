/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「CF 716E」Digit Tree 28-06-2017
 * 点分治
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
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
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

const int OUT_LEN = 1000000;

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

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}

namespace Task {

#define long long long

int n, mod;

inline void exgcd(long a, long b, long &g, long &x, long &y) {
    !b ? (x = 1, y = 0, g = a) : (exgcd(b, a % b, g, y, x), y -= (a / b) * x);
}

inline long getInv(const long x) {
    static long tmp1, res, tmp2;
    exgcd(x, mod, tmp1, res, tmp2);
    return (res % mod + mod) % mod;
}

const int MAXN = 100010;

struct Node {
    int v, w;

    Node(int v, int w) : v(v), w(w) {}
};

std::vector<Node> edge[MAXN];

inline void addEdge(const int u, const int v, const int w) {
    edge[u].push_back(Node(v, w));
    edge[v].push_back(Node(u, w));
}

int sz[MAXN], pow[MAXN], inv[MAXN];
bool vis[MAXN];
long ans;

std::map<int, int> cnt;

inline void dfsSize(int u, int fa) {
    sz[u] = 1;
    for (register int i = 0, v; i < edge[u].size(); i++)
        if (!vis[v = edge[u][i].v] && v != fa) dfsSize(v, u), sz[u] += sz[v];
}

inline int getCenter(int u, int fa, int n) {
    register int s = n - sz[u];
    for (register int i = 0, v; i < edge[u].size(); i++) {
        if (!vis[v = edge[u][i].v] && v != fa) {
            if (int ret = getCenter(v, u, n)) return ret;
            s = std::max(s, sz[v]);
        }
    }
    return (s << 1) <= n ? u : 0;
}

inline void dfs(int u, int fa, int delta, long p, int val) {
    cnt[val] += delta;
    for (register int i = 0, v; i < edge[u].size(); i++)
        if (!vis[v = edge[u][i].v] && v != fa)
            dfs(v, u, delta, p * 10 % mod, (val + edge[u][i].w * p) % mod);
}

inline void calc(int u, int fa, int num, long val) {
    ans += cnt[(-val * inv[num] % mod + mod) % mod];
    for (register int i = 0, v; i < edge[u].size(); i++)
        if (!vis[v = edge[u][i].v] && v != fa)
            calc(v, u, num + 1, (val * 10 + edge[u][i].w) % mod);
}

inline void solve(int u) {
    dfsSize(u, 0), vis[u = getCenter(u, 0, sz[u])] = true, cnt.clear();
    for (register int i = 0, v; i < edge[u].size(); i++)
        if (!vis[v = edge[u][i].v]) dfs(v, u, 1, 10 % mod, edge[u][i].w);
    ans += cnt[0], cnt[0]++;
    for (register int i = 0, v; i < edge[u].size(); i++) {
        if (!vis[v = edge[u][i].v]) {
            dfs(v, u, -1, 10 % mod, edge[u][i].w);
            calc(v, u, 1, edge[u][i].w);
            dfs(v, u, 1, 10 % mod, edge[u][i].w);
        }
    }
    for (register int i = 0, v; i < edge[u].size(); i++)
        if (!vis[v = edge[u][i].v]) solve(v);
}

inline void solve() {
    using namespace IO;
    read(n), read(mod);
    for (register int i = 1, u, v, w; i < n; i++)
        read(u), read(v), read(w), addEdge(u + 1, v + 1, w % mod);
    pow[0] = inv[0] = 1;
    for (int i = 1; i <= n; i++)
        pow[i] = (long)pow[i - 1] * 10 % mod, inv[i] = getInv(pow[i]);
    solve(1);
    print(ans);
}
}

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    Task::solve();
    IO::flush();
    return 0;
}