/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 4713」迷失的字符串 03-01-2018
 * bitset
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

template <typename T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        iosig |= c == '-';
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig && (x = -x);
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

const int OUT_LEN = 1 << 18 | 1;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    (oh == obuf + OUT_LEN) && (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf);
    *oh++ = c;
}

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

struct InputOutputStream {
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

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
} io;

const int MAXM = 16;
const int SIGMA = 26 + 1;
const int MAXN = 30000 + 9;

struct Node {
    int v, w;

    Node(int v, int w) : v(v), w(w) {}
};

std::vector<Node> edge[MAXN];

typedef std::vector<Node>::iterator Iterator;

int sz[MAXN], son[MAXN];
int n, m, tot, lPos[MAXN], rPos[MAXN], id[MAXN], pool[MAXM], top;
bool vis[MAXN];
char s[MAXN];
std::bitset<MAXN> out, ans, end, st[SIGMA], start[SIGMA], f[MAXM], g[MAXM];

inline void addEdge(int u, int v, int c) {
    edge[u].push_back(Node(v, c));
    edge[v].push_back(Node(u, c));
}

void dfs1(int u) {
    sz[u] = 1;
    vis[u] = true;
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); ++p) {
        if (!vis[p->v]) {
            dfs1(p->v);
            sz[u] += sz[p->v];
            (sz[son[u]] < sz[p->v]) && (son[u] = p->v);
        }
    }
}

inline void init(int u) {
    id[u] = pool[top--];
    f[id[u]].reset();
    g[id[u]] = end;
}

inline void dp(int x, int y, int c) {
    register int ix = id[x], iy = id[y];
    f[iy] <<= 1;
    f[iy] &= st[c];
    f[iy] |= start[c];
    g[iy] &= st[c];
    out |= g[iy];
    g[iy] >>= 1;
    ans |= f[ix] & g[iy];
    ans |= g[ix] & f[iy];
    f[ix] |= f[iy];
    g[ix] |= g[iy];
    pool[++top] = iy;
}

void dfs2(int u) {
    vis[u] = false;
    if (son[u]) dfs2(son[u]);
    init(u);
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); ++p)
        if (p->v == son[u]) dp(u, son[u], p->w);
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); ++p) {
        if (vis[p->v]) {
            dfs2(p->v);
            dp(u, p->v, p->w);
        }
    }
}

inline void solve() {
    for (register int i = (top = MAXM - 1); i >= 0; i--) pool[i] = i;
    io >> n;
    for (register int i = 1, u, v; i < n; i++) {
        io >> u >> v >> s;
        addEdge(u, v, s[0] - 'a');
    }
    io >> m;
    for (register int i = 1, len; i <= m; i++) {
        len = read(s);
        start[s[0] - 'a'].set(lPos[i] = tot + 1);
        for (register int j = 0; j < len; j++) st[s[j] - 'a'].set(++tot);
        end.set(rPos[i] = tot);
    }
    dfs1(1);
    dfs2(1);
    for (register int i = 1, flag = 0; i <= m; i++, flag = 0) {
        flag |= out.test(lPos[i]);
        for (register int j = lPos[i]; j <= rPos[i] && !flag; j++)
            flag |= ans.test(j);
        io << (flag ? "YES\n" : "NO\n");
    }
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}