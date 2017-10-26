/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 2012」大逃杀 26-10-2017
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

inline void read(char &c) {
    while (c = read(), isspace(c) && c != -1)
        ;
}

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), !isprint(c) && c != -1)
        ;
    if (c == -1) {
        *buf = 0;
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), isprint(c) && c != -1);
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

const int MAXN = 310;

struct Node {
    int v, w;

    Node(int v, int w) : v(v), w(w) {}
};

typedef std::vector<Node>::iterator Iterator;

std::vector<Node> edge[MAXN + 1];

using IO::io;

int w[MAXN + 1], t[MAXN + 1], n, T;
int f[MAXN + 1][MAXN + 1], g[MAXN + 1][MAXN + 1], h[MAXN + 1][MAXN + 1];

inline void addEdge(const int u, const int v, const int w) {
    edge[u].push_back(Node(v, w)), edge[v].push_back(Node(u, w));
}

inline void update(int &a, int b) { a < b ? a = b : 0; }

void dfs(const int u, const int fa) {
    register int *f = ::f[u], *g = ::g[u], *h = ::h[u];
    f[t[u]] = g[t[u]] = h[t[u]] = w[u];
    memset(f, 0xc0, sizeof(int) * t[u]);
    memset(g, 0xc0, sizeof(int) * t[u]);
    memset(h, 0xc0, sizeof(int) * t[u]);
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); p++) {
        if (p->v != fa) {
            dfs(p->v, u);
            static int tf[MAXN + 1], tg[MAXN + 1], th[MAXN + 1];
            memcpy(tf, f, sizeof(int) * (T + 1));
            memcpy(tg, g, sizeof(int) * (T + 1));
            memcpy(th, h, sizeof(int) * (T + 1));
            register int *nf = ::f[p->v], *ng = ::g[p->v], *nh = ::h[p->v];
            for (register int j = 0; j <= T && T - j - p->w * 2 >= 0; j++) {
                for (register int k = 0; k <= T - j - p->w * 2; k++) {
                    update(f[j + k + p->w * 2], tf[j] + nf[k]);
                    update(g[j + k + p->w * 2], tg[j] + nf[k]);
                    update(h[j + k + p->w * 2],
                           std::max(th[j] + nf[k], tf[j] + nh[k]));
                }
            }
            for (register int j = 0; j <= T && T - j - p->w >= 0; j++) {
                for (register int k = 0; k <= T - j - p->w; k++) {
                    update(g[j + k + p->w], tf[j] + ng[k]);
                    update(h[j + k + p->w], tg[j] + ng[k]);
                }
            }
        }
    }
}

inline void solve() {
    io >> n >> T;
    register int sum = 0;
    for (register int i = 1; i <= n; i++) io >> w[i], sum += w[i];
    if (sum == 0) {
        io << '0';
        return;
    }
    for (register int i = 1; i <= n; i++)
        io >> t[i], t[i] = std::min(t[i], T + 1);
    for (register int i = 1, u, v, w; i < n; i++)
        io >> u >> v >> w, addEdge(u, v, w);
    dfs(1, 0);
    register int ans = 0;
    for (register int i = 1; i <= n; i++) {
        ans = std::max(
            ans, std::max(*std::max_element(f[i], f[i] + T + 1),
                          std::max(*std::max_element(g[i], g[i] + T + 1),
                                   *std::max_element(h[i], h[i] + T + 1))));
    }
    io << ans;
}
}

int main() {
    solve();
    return 0;
}