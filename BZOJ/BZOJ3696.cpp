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

inline void read(char &x) {
    while (x = read(), isspace(x) && x != -1)
        ;
}

const int OUT_LEN = 100000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, oh - obuf, stdout), oh = obuf) : 0;
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
    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    ~InputOutputStream() { flush(); }
} io;
}  // namespace IO

const int MAXN = 100005;

std::vector<int> edge[MAXN + 1];

int n, cnt;
int a[MAXN][505];
int ans[512], dep[MAXN];

inline void addEdge(const int u, const int v) { edge[u].push_back(v); }

inline void dfs(int x) {
    a[x][0] = 1;
    for (register int i = 0, v; i < edge[x].size(); i++) {
        dfs(v = edge[x][i]);
        for (int j = 0; j <= dep[x]; j++)
            for (int k = 0; k <= dep[v]; k++)
                ans[j ^ (k + 1)] += a[x][j] * a[v][k];
        dep[x] = std::max(dep[x], dep[v] + 1);
        for (int j = 0; j <= dep[v]; j++) a[x][j + 1] += a[v][j];
    }
}

using IO::io;

int main() {
    io >> n;
    for (int i = 2, x; i <= n; i++) {
        io >> x;
        addEdge(x, i);
    }
    dfs(1);
    register int mx = 512;
    for (; mx; mx--)
        if (ans[mx]) break;
    for (int i = 0; i <= mx; i++) io << ans[i] << '\n';
    return 0;
}
