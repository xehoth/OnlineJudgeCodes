/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1093」最大半连通子图 27-10-2017
 * 缩点 + DP
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
inline bool read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return false;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
    return true;
}

inline void read(char &c) {
    while (c = read(), isspace(c) && c != -1)
        ;
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

    ~InputOutputStream() { flush(); }
} io;
}

namespace {

using IO::io;

const int MAXN = 100000;
const int MAXM = 1000000;
struct Graph {
    std::vector<int> edge[MAXN + 1];

    inline void addEdge(const int u, const int v) { edge[u].push_back(v); }

    inline std::vector<int> &operator[](const int i) { return edge[i]; }
} g, newG;

typedef std::vector<int>::iterator Iterator;

int dfn[MAXN + 1], low[MAXN + 1], idx, scc[MAXN + 1], sum[MAXN + 1], cnt;
bool vis[MAXN + 1];
std::vector<int> st;

void tarjan(const int u) {
    vis[u] = true, st.push_back(u), low[u] = dfn[u] = ++idx;
    register int v;
    for (register Iterator p = g[u].begin(); p != g[u].end(); p++) {
        if (!dfn[v = *p])
            tarjan(v), low[u] = std::min(low[u], low[v]);
        else if (vis[v])
            low[u] = std::min(low[u], dfn[v]);
    }
    if (low[u] == dfn[u]) {
        cnt++;
        do
            vis[v = st.back()] = false, st.pop_back(), sum[scc[v] = cnt]++;
        while (u != v);
    }
}
int n, m, mod;

std::pair<int, int> edges[MAXM + 1];

int f[MAXN + 1], h[MAXN + 1];

void dp(const int u) {
    if (f[u]) return;
    for (register Iterator p = newG[u].begin(); p != newG[u].end(); p++) {
        dp(*p);
        if (f[*p] > f[u])
            f[u] = f[*p], h[u] = h[*p];
        else if (f[*p] == f[u])
            h[u] = (h[u] + h[*p]) % mod;
    }
    if (!f[u]) h[u] = 1;
    f[u] += sum[u];
}

inline void solve() {
    io >> n >> m >> mod;
    for (register int i = 1, u, v; i <= m; i++) io >> u >> v, g.addEdge(u, v);
    for (register int i = 1; i <= n; i++)
        if (!dfn[i]) tarjan(i);
    m = 0;
    for (register int i = 1; i <= n; i++)
        for (register Iterator p = g[i].begin(); p != g[i].end(); p++)
            if (scc[i] != scc[*p]) edges[++m] = std::make_pair(scc[i], scc[*p]);
    std::sort(edges + 1, edges + m + 1);
    m = std::unique(edges + 1, edges + m + 1) - edges - 1;
    memset(vis, 0, sizeof(bool) * (n + 1));
    for (register int i = 1; i <= m; i++) {
        newG.addEdge(edges[i].first, edges[i].second);
        vis[edges[i].second] = true;
    }
    for (register int i = 1; i <= cnt; i++)
        if (!vis[cnt]) newG.addEdge(0, i);
    dp(0);
    io << f[0] << '\n' << h[0];
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}