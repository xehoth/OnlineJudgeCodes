/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2447」「BZOJ 3302」「BZOJ 2103」 25-10-2017
 * 树形 DP
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
        return read(x), *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        return print(x), *this;
    }

    ~InputOutputStream() { flush(); }
} io;
}

namespace {

using IO::io;

const int MAXN = 100005;
const long long INF = LLONG_MAX >> 1;
int fa[MAXN + 1], dep[MAXN + 1];
long long sum[MAXN + 1], res[MAXN + 1];
int ch[MAXN + 1][2];
std::vector<int> edge[MAXN + 1];
typedef std::vector<int>::iterator Iterator;
int n;

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

int rt = 1;
inline void update(int u, int v) {
    if (!ch[u][0] || sum[v] > sum[ch[u][0]])
        ch[u][1] = ch[u][0], ch[u][0] = v;
    else if (!ch[u][1] || sum[v] > sum[ch[u][1]])
        ch[u][1] = v;
}

void dfs(int u, int f, int d) {
    dep[u] = d, fa[u] = f;
    for (register Iterator v = edge[u].begin(); v != edge[u].end(); v++) {
        if (*v != f) {
            dfs(*v, u, d + 1), update(u, *v);
            sum[u] += sum[*v], res[u] += res[*v] + sum[*v];
        }
    }
}

long long ans = INF;
int cut = 0;

void center(long long &ret, int p, int u, long long k) {
    ret = std::min(ret, k);
    register int v = ch[u][0];
    if (v == cut || sum[ch[u][0]] < sum[ch[u][1]]) v = ch[u][1];
    if (!v) return;
    center(ret, p, v, k + sum[p] - 2 * sum[v]);
}

void solve(int u) {
    register int k;
    for (register Iterator v = edge[u].begin(); v != edge[u].end(); v++) {
        if (*v == fa[u]) continue;
        cut = *v;
        register long long ex = INF, ey = INF;
        for (k = u; k;) sum[k] -= sum[*v], k = fa[k];
        center(ex, rt, rt, res[rt] - res[*v] - dep[*v] * sum[*v]);
        center(ey, *v, *v, res[*v]);
        ans = std::min(ans, ex + ey);
        for (k = u; k;) sum[k] += sum[*v], k = fa[k];
        solve(*v);
    }
}

inline void solve() {
    io >> n;
    for (register int i = 1, u, v; i < n; i++) io >> u >> v, addEdge(u, v);
    for (register int i = 1; i <= n; i++) io >> sum[i];
    dfs(rt, 0, 0);
    solve(rt);
    io << ans;
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}