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
}  // namespace IO

using IO::io;

const int MAX_N = 300000 + 3, MAX_M = 300000 + 3, INF = 0x3f3f3f3f;

struct Node {
    int v, w;
    Node() {}
    Node(int v, int w) : v(v), w(w) {}
};

std::vector<Node> edge[MAX_N];
int n, m;
struct Query {
    int u, v, lca, dep;

    inline bool operator<(const Query &p) const { return dep > p.dep; }
} d[MAX_M];

int dep[MAX_N], fc[MAX_N], fa[MAX_N], son[MAX_N], sz[MAX_N];
bool vis[MAX_N];
int dfn[MAX_N], idx, top[MAX_N];

typedef std::vector<Node>::iterator Iterator;

void dfs1(const int u) {
    vis[u] = true, sz[u] = 1;
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); p++) {
        if (!vis[p->v]) {
            dep[p->v] = dep[u] + p->w, fc[p->v] = p->w, fa[p->v] = u;
            dfs1(p->v);
            sz[u] += sz[p->v], sz[p->v] > sz[son[u]] ? son[u] = p->v : 0;
        }
    }
}

void dfs2(const int u) {
    vis[u] = false, dfn[u] = ++idx, top[u] = (u == son[fa[u]] ? top[fa[u]] : u);
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); p++)
        if (son[u] == p->v) dfs2(p->v);
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); p++)
        if (vis[p->v]) dfs2(p->v);
}

inline int lca(register int u, register int v) {
    while (top[u] != top[v])
        dep[top[u]] < dep[top[v]] ? v = fa[top[v]] : u = fa[top[u]];
    return dep[u] < dep[v] ? u : v;
}

inline bool isAncestor(int u, int v) {
    return dfn[u] <= dfn[v] && dfn[v] < dfn[u] + sz[u];
}

inline void init() {
    dfs1(1), dfs2(1);
    for (register int i = 0; i < m; i++) {
        io >> d[i].u >> d[i].v, d[i].lca = lca(d[i].u, d[i].v);
        d[i].dep = dep[d[i].u] + dep[d[i].v] - 2 * dep[d[i].lca];
    }
    std::sort(d, d + m);
}

inline int optMin(int a, int b) { return a < b ? a : b; }

inline void solve() {
    register int ans = INT_MAX;
    register bool flag;
    for (register int i = d[0].u; i != d[0].lca; i = fa[i]) {
        register int nowDis = d[0].dep - fc[i];
        if (nowDis >= ans) continue;
        flag = 0;
        for (register int j = 1; j < m && d[j].dep > nowDis; ++j) {
            if (isAncestor(i, d[j].u) == isAncestor(i, d[j].v)) {
                ans = optMin(ans, d[j].dep), flag = true;
                break;
            }
        }
        if (!flag) ans = optMin(ans, nowDis);
    }
    for (register int i = d[0].v; i != d[0].lca; i = fa[i]) {
        register int nowDis = d[0].dep - fc[i];
        if (nowDis >= ans) continue;
        flag = 0;
        for (register int j = 1; j < m && d[j].dep > nowDis; ++j) {
            if (isAncestor(i, d[j].u) == isAncestor(i, d[j].v)) {
                ans = optMin(ans, d[j].dep), flag = true;
                break;
            }
        }
        if (!flag) ans = optMin(ans, nowDis);
    }
    io << (ans == INT_MAX ? 0 : ans);
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    io >> n >> m;
    for (register int i = 0, f, t, c; i < n - 1; i++) {
        io >> f >> t >> c;
        edge[f].push_back(Node(t, c));
        edge[t].push_back(Node(f, c));
    }
    init();
    solve();
    return 0;
}