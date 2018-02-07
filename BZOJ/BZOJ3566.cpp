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

struct InputOutputStream {
    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }
} io;
}  // namespace IO

namespace {

using IO::io;

const int MAXN = 500010;

struct Node {
    int v, p;

    Node(int v, int p) : v(v), p(p) {}
};

std::vector<Node> edge[MAXN + 1];

inline void addEdge(const int u, const int v, const int w) {
    edge[u].push_back(Node(v, w));
    edge[v].push_back(Node(u, w));
}

int n, q[MAXN], fv[MAXN];
double f[MAXN], g[MAXN];

typedef std::vector<Node>::iterator Iterator;

void dfs1(int u, int fa) {
    f[u] = (100 - q[u]) / 100.0;
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); ++p) {
        if (p->v != fa) {
            fv[p->v] = p->p, dfs1(p->v, u);
            f[u] *= f[p->v] + (1 - f[p->v]) * ((100 - p->p) / 100.0);
        }
    }
}

void dfs2(int u, int fa) {
    if (fa == -1) {
        g[u] = 1.0;
    } else {
        double t =
            g[fa] * f[fa] / (f[u] + (1 - f[u]) * ((100 - fv[u]) / 100.0));
        g[u] = t + (1 - t) * ((100 - fv[u]) / 100.0);
    }
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); ++p)
        if (p->v != fa) dfs2(p->v, u);
}

inline void solve() {
    io >> n;
    for (register int i = 0, u, v, w; i < n - 1; i++)
        io >> u >> v >> w, u--, v--, addEdge(u, v, w);
    for (register int i = 0; i < n; i++) io >> q[i];
    dfs1(0, -1), dfs2(0, -1);
    register double ans = 0;
    for (register int i = 0; i < n; i++) ans += 1 - f[i] * g[i];
    printf("%.6f\n", ans);
}
}  // namespace

int main() {
    solve();
    return 0;
}