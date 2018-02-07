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

const int MAXN = 500005;

struct Node {
    int v, w;

    Node(int v, int w) : v(v), w(w) {}
};

std::vector<Node> edge[MAXN + 1];

int n, m, cnt, fa[MAXN], f[MAXN], q[MAXN], val[MAXN];
long long d[3][MAXN];
bool vis[MAXN];
typedef std::vector<Node>::iterator Iterator;

inline void addEdge(const int u, const int v, const int w) {
    edge[u].push_back(Node(v, w)), edge[v].push_back(Node(u, w));
}

inline void bfs(int k, int now) {
    register int head = 0, tail = now;
    memset(d[k], -1, sizeof(d[k]));
    for (register int i = 1; i <= tail; i++) d[k][q[i]] = 0;
    for (register int u, v; head < tail;) {
        u = q[++head];
        for (register Iterator p = edge[u].begin(); p != edge[u].end(); ++p) {
            if (d[k][v = p->v] == -1) {
                d[k][v] = d[k][u] + p->w, q[++tail] = v;
                fa[v] = u, val[v] = p->w;
            }
        }
    }
}

inline void solve() {
    io >> n >> m;
    long long sum = 0;
    register int k = 1;
    for (register int i = 1, u, v, w; i < n; i++)
        io >> u >> v >> w, addEdge(u, v, w);
    for (register int i = 1; i <= m; i++) io >> f[i];
    q[1] = f[1], bfs(0, 1), vis[f[1]] = true;
    for (register int i = 2; i <= m; i++) {
        if (d[0][f[i]] > d[0][f[k]]) k = i;
        for (register int j = f[i]; j != f[1] && !vis[j]; j = fa[j])
            sum += val[j], vis[j] = true;
    }
    for (register int i = 1; i <= n; i++)
        if (vis[i]) q[++cnt] = i;
    bfs(0, cnt), q[1] = f[k], bfs(1, 1), k = 1;
    for (register int i = 2; i <= m; i++)
        if (d[1][f[i]] > d[1][f[k]]) k = i;
    q[1] = f[k];
    bfs(2, 1), sum <<= 1;
    for (register int i = 1; i <= n; i++)
        io << (vis[i] ? sum - std::max(d[1][i], d[2][i])
                      : sum + (d[0][i] << 1) - std::max(d[1][i], d[2][i]))
           << '\n';
}
}  // namespace

int main() {
    solve();
    return 0;
}
