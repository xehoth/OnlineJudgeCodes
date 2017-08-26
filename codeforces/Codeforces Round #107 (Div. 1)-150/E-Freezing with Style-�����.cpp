/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「CF 150E」Freezing with Style 点分治 09-07-2017
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

const int MAXN = 101010;
const int INF = INT_MAX >> 1;

struct Node {
    int v, w;
    Node(int v, int w) : v(v), w(w) {}
};

std::vector<Node> edge[MAXN];

inline void addEdge(const int u, const int v, const int w) {
    edge[u].push_back(Node(v, w));
    edge[v].push_back(Node(u, w));
}

int sz[MAXN], n, limitL, limitR, L = -1, R, dep[MAXN], q[MAXN];
bool vis[MAXN];

typedef std::pair<int, int> Pair;

Pair ans;

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

inline bool cmp(const Node &a, const Node &b) { return dep[a.v] < dep[b.v]; }

inline void getDep(int u, int fa) {
    dep[u] = 1;
    for (register int i = 0, v; i < edge[u].size(); i++) {
        if (!vis[v = edge[u][i].v] && v != fa) {
            getDep(v, u), dep[u] = std::max(dep[u], dep[v] + 1);
        }
    }
}

Pair a[MAXN], b[MAXN], tmp;

inline void dfs(int u, int fa, int d, int val, int limit) {
    if (val > b[d].first) b[d] = std::make_pair(val, u);
    for (register int i = 0, v; i < edge[u].size(); i++) {
        if (!vis[v = edge[u][i].v] && v != fa)
            dfs(v, u, d + 1, val + (edge[u][i].w >= limit ? 1 : -1), limit);
    }
}

inline bool check(int u, int limit) {
    for (register int i = 0; i < dep[u]; i++) a[i] = std::make_pair(-INF, 0);
    a[0] = std::make_pair(0, u);
    register int pp = 0;
    for (register int i = 0, v; i < edge[u].size(); i++) {
        if (!vis[v = edge[u][i].v]) {
            for (register int j = 0; j <= dep[v]; j++)
                b[j] = std::make_pair(-INF, 0);
            dfs(v, u, 1, edge[u][i].w >= limit ? 1 : -1, limit);
            register int l = 0, r = 0, end = pp;
            for (register int j = 0; j <= dep[v]; j++) {
                while (j + end >= limitL && end >= 0) {
                    while (l < r && a[q[r - 1]] <= a[end]) r--;
                    q[r++] = end--;
                }
                while (l < r && q[l] + j > limitR) l++;
                if (l < r && a[q[l]].first + b[j].first >= 0)
                    return tmp = std::make_pair(a[q[l]].second, b[j].second),
                           true;
            }
            for (register int j = 0; j <= dep[v]; j++)
                a[j] = std::max(a[j], b[j]);
            pp = dep[v];
        }
    }
    return false;
}

inline void calc(int u) {
    getDep(u, 0);
    std::sort(edge[u].begin(), edge[u].end(), cmp);
    register int l = L, r = R;
    if (!check(u, l)) return;
    while (l + 1 != r) {
        register int mid = l + r >> 1;
        if (check(u, mid))
            l = mid;
        else
            r = mid;
    }
    if (l > L) ans = tmp, L = l;
}

inline void solve(int u) {
    dfsSize(u, 0);
    vis[u = getCenter(u, 0, sz[u])] = true;
    calc(u);
    for (register int i = 0, v; i < edge[u].size(); i++)
        if (!vis[v = edge[u][i].v]) solve(v);
}

inline void solve() {
    using namespace IO;
    read(n), read(limitL), read(limitR);
    for (register int i = 1, u, v, w; i < n; i++)
        read(u), read(v), read(w), addEdge(u, v, w), R = std::max(R, w + 1);
    solve(1);
    print(ans.first), print(' '), print(ans.second);
}
}

int main() {
    Task::solve();
    IO::flush();
    return 0;
}