#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 100000;
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
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
}

const int OUT_LEN = 100000;

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

struct InputOutputStream {
    ~InputOutputStream() { flush(); }

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
}  // namespace IO

namespace {

using IO::io;

const int MAXN = 20000 + 5;
const int MAXG = 60;
const int MAX_LOG = 15;

struct LinearBasis {
    long long a[MAXG];

    inline void insert(register unsigned long long v) {
        for (register unsigned i; v;) {
            if (a[i = 63 - __builtin_clzll(v)]) {
                v ^= a[i];
            } else {
                a[i] = v;
                break;
            }
        }
    }

    inline void merge(const LinearBasis &p) {
        for (register int i = MAXG - 1; i >= 0; i--)
            p.a[i] ? insert(p.a[i]) : (void)0;
    }

    inline long long query() {
        register long long w = 0;
        for (register int i = MAXG - 1; i >= 0; --i)
            if (~w >> i & 1) w ^= a[i];
        return w;
    }
} f[MAX_LOG][MAXN];

std::vector<int> edge[MAXN], vc[MAXN];

typedef std::vector<int>::iterator Iterator;

int sz[MAXN], dep[MAXN], fa[MAXN];
bool vis[MAXN];
long long g[MAXN];

void getSize(const int u, const int pre) {
    sz[u] = 1;
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); ++p)
        if (*p != pre && !vis[*p]) getSize(*p, u), sz[u] += sz[*p];
}

int getCenter(const int u, const int pre, const int n) {
    register int max = n - sz[u];
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); ++p) {
        if (*p != pre && !vis[*p]) {
            if (int ret = getCenter(*p, u, n)) return ret;
            max = std::max(max, sz[*p]);
        }
    }
    return max * 2 <= n ? u : 0;
}

int build(int u) {
    getSize(u, 0), u = getCenter(u, 0, sz[u]), vis[u] = 1;
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); ++p)
        if (!vis[*p]) vc[u].push_back(build(*p));
    return u;
}

void insertBasis(int u, int v, LinearBasis *c) {
    c[u].insert(g[u]);
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); ++p)
        if (*p != v && vis[*p]) c[*p] = c[u], insertBasis(*p, u, c);
}

void dfs(int u) {
    insertBasis(u, 0, f[dep[u]]), vis[u] = 0;
    for (register Iterator p = vc[u].begin(); p != vc[u].end(); ++p)
        dep[*p] = dep[fa[*p] = u] + 1, dfs(*p);
}

inline int lca(int u, int v) {
    while (u != v) dep[u] > dep[v] ? u = fa[u] : v = fa[v];
    return u;
}

inline void solve() {
    register int n, m;
    io >> n >> m;
    for (register int i = 1; i <= n; i++) io >> g[i];
    for (register int i = 1, u, v; i < n; ++i)
        io >> u >> v, edge[u].push_back(v), edge[v].push_back(u);
    dfs(build(1));
    for (register int u, v; m--;) {
        io >> u >> v;
        register int i = dep[lca(u, v)];
        LinearBasis b = f[i][u];
        b.merge(f[i][v]);
        io << b.query() << '\n';
    }
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}