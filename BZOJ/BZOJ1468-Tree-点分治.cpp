/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1468」Tree 26-06-2017
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

const int MAXN = 40010;

struct Node {
    int v, w;

    Node(int v, int w) : v(v), w(w) {}
};

std::vector<Node> edge[MAXN];

inline void addEdge(const int u, const int v, const int w) {
    edge[u].push_back(Node(v, w));
    edge[v].push_back(Node(u, w));
}

int sz[MAXN], k, dis[MAXN], ans;
bool vis[MAXN];
std::vector<int> dep;

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

inline void getDeep(int u, int father) {
    dep.push_back(dis[u]);
    for (register int i = 0, v; i < edge[u].size(); i++)
        if ((v = edge[u][i].v) != father && !vis[v])
            dis[v] = dis[u] + edge[u][i].w, getDeep(v, u);
}

inline int calc(int u, int init) {
    dep.clear(), dis[u] = init, getDeep(u, 0);
    std::sort(dep.begin(), dep.end());
    register int ret = 0;
    for (register int l = 0, r = dep.size() - 1; l < r;) {
        if (dep[l] + dep[r] <= k)
            ret += r - l++;
        else
            r--;
    }
    return ret;
}

inline void solve(int u) {
    dfsSize(u, 0), vis[u = getCenter(u, 0, sz[u])] = true, ans += calc(u, 0);
    for (register int i = 0, v; i < edge[u].size(); i++)
        if (!vis[v = edge[u][i].v]) ans -= calc(v, edge[u][i].w), solve(v);
}

inline void solve() {
    using namespace IO;
    register int n;
    read(n);
    for (register int i = 1, u, v, w; i < n; i++)
        read(u), read(v), read(w), addEdge(u, v, w);
    read(k), solve(1);
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