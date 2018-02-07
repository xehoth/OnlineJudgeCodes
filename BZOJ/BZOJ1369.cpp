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

const int MAXN = 10000;

std::vector<int> edge[MAXN + 1];

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

int f[MAXN + 1][4];

typedef std::vector<int>::iterator Iterator;

void dp(const int u, const int fa) {
    f[u][1] = 1, f[u][2] = 2, f[u][3] = 3;
    for (register int i = 0, v; i < edge[u].size(); i++)
        if ((v = edge[u][i]) != fa) dp(v, u);
    for (register int i = 1; i <= 3; i++) {
        for (register int p = 0, v; p < edge[u].size(); p++) {
            if ((v = edge[u][p]) != fa) {
                register int min = INT_MAX;
                for (register int k = 1; k <= 3; k++)
                    if (i != k) min = std::min(min, f[v][k]);
                f[u][i] += min;
            }
        }
    }
}

using IO::io;

inline void solve() {
    register int n;
    io >> n;
    for (register int i = 1, u, v; i < n; i++) io >> u >> v, addEdge(u, v);
    dp(1, 0);
    io << *std::min_element(f[1] + 1, f[1] + 4);
}
}  // namespace

int main() {
    solve();
    return 0;
}