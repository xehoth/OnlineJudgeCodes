/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 2003」拆网线 23-10-2017
 * 贪心 + 树状数组
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

int fa[MAXN + 1], deg[MAXN + 1], tot, edges, n, k, sz[MAXN + 1];
int in[MAXN + 1], out[MAXN + 1], idx;
int d[MAXN + 1];
bool vis[MAXN + 1];

inline void modify(register int k, register int v) {
    for (; k <= n; k += k & -k) d[k] += v;
}

inline int query(register int k) {
    register int ret = 0;
    for (; k; k ^= k & -k) ret += d[k];
    return ret;
}

inline int query(int l, int r) { return query(r) - query(l - 1); }

std::vector<int> edge[MAXN + 1];
typedef std::vector<int>::iterator Iterator;

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

void dfs(const int u) {
    sz[u] = 1, in[u] = ++idx;
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); p++) {
        if (*p != fa[u]) {
            dfs(*p), sz[u] += sz[*p];
        }
    }
    out[u] = idx;
}

void decPairDegOne(const int u) {
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); p++) {
        if (*p != fa[u]) {
            decPairDegOne(*p);
        }
    }
    if (!vis[u] && !vis[fa[u]] && deg[u] == 1 &&
        query(in[fa[u]], out[fa[u]]) >= 2) {
        deg[u]--, deg[fa[u]]--;
        vis[u] = vis[fa[u]] = true;
        if (fa[fa[u]]) deg[fa[fa[u]]]--;
        modify(in[fa[u]], -1), modify(in[u], -1);
        tot += 2, edges++;
    }
}

inline void solveCase() {
    io >> n >> k;
    for (register int i = 0; i <= n; i++) edge[i].clear();
    idx = 0;
    memset(deg, 0, sizeof(int) * (n + 1));
    memset(d, 0, sizeof(int) * (n + 1));
    tot = edges = 0;
    memset(vis, 0, sizeof(bool) * (n + 1));
    for (register int i = 1, u; i < n; i++)
        io >> u, fa[i + 1] = u, deg[u]++, deg[i + 1]++;
    for (register int i = 2; i <= n; i++) addEdge(i, fa[i]);
    dfs(1);
    for (register int i = 1; i <= n; i++) modify(in[i], 1);
    decPairDegOne(1);
    if (k % 2 == 0 && tot >= k) {
        io << k / 2 << '\n';
        return;
    }
    if (k % 2 == 1 && tot >= k - 1) {
        io << k / 2 + 1 << '\n';
        return;
    }
    io << k - tot + edges << '\n';
}

inline void solve() {
    register int T;
    io >> T;
    while (T--) solveCase();
}
}

int main() {
    solve();
    return 0;
}