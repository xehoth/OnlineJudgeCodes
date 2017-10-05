/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「UVA 1424」Salesmen 05-10-2017
 * DP
 * @author xehoth
 */
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

inline void flush() {
    fwrite(obuf, 1, oh - obuf, stdout);
}

struct InputOutputStream {
    ~InputOutputStream() {
        flush();
    }

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
}

namespace {

using IO::io;

const int MAXN = 110;

std::vector<int> edge[MAXN + 1];

typedef std::vector<int>::iterator Iterator;

int s[MAXN * 2 + 1], f[MAXN * 2 + 1][MAXN + 1];

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

inline void solve(const int n, const int m) {
    for (register int i = 0; i <= n; i++) edge[i].clear();
    for (register int i = 0, u, v; i < m; i++) io >> u >> v, addEdge(u, v);
    register int len;
    io >> len;
    for (register int i = 1; i <= len; i++) io >> s[i];
    for (register int i = 1; i <= len; i++) {
        memset(f[i], 0x3f, sizeof(int) * (n + 1));
        for (register int u = 1; u <= n; u++) {
            for (Iterator v = edge[u].begin(); v != edge[u].end(); v++)
                f[i][*v] = std::min(f[i][*v], f[i - 1][u] + (s[i] != *v));
            f[i][u] = std::min(f[i][u], f[i - 1][u] + (s[i] != u));
        }
    }
    io << *std::min_element(f[len] + 1, f[len] + n + 1) << '\n';
}

inline void solve() {
    register int T, n, m;
    io >> T;
    while (T--) {
        io >> n >> m;
        solve(n, m);
    }
}
}

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    solve();
    return 0;
}