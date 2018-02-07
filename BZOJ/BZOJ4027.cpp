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

const int MAXN = 2000000;

int ans, n, m;

std::vector<std::vector<int> > edge;
int c[MAXN + 1];

inline bool cmp(const int u, const int v) { return c[u] < c[v]; }

void dfs(const int u) {
    for (register int i = 0; i < edge[u].size(); i++) dfs(edge[u][i]);
    std::sort(edge[u].begin(), edge[u].end(), cmp);
    c[u] += edge[u].size();
    for (register int i = 0; i < edge[u].size(); i++) {
        register int t = c[edge[u][i]];
        if (c[u] + t - 1 <= m)
            c[u] += t - 1, ans++;
        else
            break;
    }
}

using IO::io;

inline void solve() {
    io >> n >> m;
    edge.resize(n);
    for (register int i = 0; i < n; i++) io >> c[i];
    for (register int i = 0, k, v; i < n; i++)
        for (io >> k; k--;) io >> v, edge[i].push_back(v);
    dfs(0);
    io << ans;
}
}  // namespace

int main() {
    solve();
    return 0;
}