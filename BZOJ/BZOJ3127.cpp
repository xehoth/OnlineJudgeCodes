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
}  // namespace IO

namespace Task {

const int MAXN = 200010;

struct Node {
    int v, w;

    Node(int v, int w) : v(v), w(w) {}
};

std::vector<Node> edge[MAXN];

inline void addEdge(const int u, const int v, const int w) {
    edge[u].push_back(Node(v, w));
    edge[v].push_back(Node(u, w));
}

#define long long long

int sz[MAXN], mark[MAXN], dep[MAXN], maxDep, n, dis[MAXN];
bool vis[MAXN];
long f[MAXN * 2][2], g[MAXN * 2][2], ans;

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

inline void getDep(int u, int fa) {
    maxDep = std::max(maxDep, dep[u]);
    mark[dis[u]] ? f[dis[u]][1]++ : f[dis[u]][0]++;
    mark[dis[u]]++;
    for (register int i = 0, v; i < edge[u].size(); i++)
        if (!vis[v = edge[u][i].v] && v != fa)
            dep[v] = dep[u] + 1, dis[v] = dis[u] + edge[u][i].w, getDep(v, u);
    mark[dis[u]]--;
}

inline void calc(int u) {
    register int max = 0;
    g[n][0] = 1;
    for (register int i = 0, v; i < edge[u].size(); i++) {
        if (!vis[v = edge[u][i].v]) {
            dis[v] = n + edge[u][i].w, dep[v] = 1, maxDep = 1, getDep(v, 0);
            max = std::max(max, maxDep), ans += (g[n][0] - 1) * f[n][0];
            for (register int j = -maxDep; j <= maxDep; j++) {
                ans += g[n - j][1] * f[n + j][1] + g[n - j][0] * f[n + j][1] +
                       g[n - j][1] * f[n + j][0];
            }
            for (register int j = n - maxDep; j <= n + maxDep; j++)
                g[j][0] += f[j][0], g[j][1] += f[j][1], f[j][0] = f[j][1] = 0;
        }
    }
    for (register int i = n - max; i <= n + max; i++) g[i][0] = g[i][1] = 0;
}

inline void solve(int u) {
    dfsSize(u, 0), vis[u = getCenter(u, 0, sz[u])] = true, calc(u);
    for (register int i = 0, v; i < edge[u].size(); i++)
        if (!vis[v = edge[u][i].v]) solve(v);
}

struct Edge {
    int u, v, w;
} e[MAXN];

int in[MAXN];

inline void solve() {
    using namespace IO;
    read(n);
    for (register int i = 1; i < n; i++)
        read(e[i].u), read(e[i].v), read(e[i].w), in[e[i].u]++, in[e[i].v]++;
    for (register int i = 1; i <= n; i++) edge[i].reserve(in[i]);
    for (register int i = 1; i < n; i++)
        addEdge(e[i].u, e[i].v, e[i].w ? 1 : -1);
    solve(1);
    print(ans);
}

#undef long
}  // namespace Task

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    Task::solve();
    IO::flush();
    return 0;
}