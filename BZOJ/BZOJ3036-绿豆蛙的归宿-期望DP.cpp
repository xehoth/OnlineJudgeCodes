/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3036」绿豆蛙的归宿 27-09-2017
 * 期望 DP
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
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (iosig = false, c = read(); !isdigit(c); c = read()) {
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

const int OUT_LEN = 10000000;

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

inline void print(double x) {
    static char buf[30];
    sprintf(buf, "%.2f", x);
    print((const char *)buf);
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

#define long long long

using IO::io;

const int MAXN = 100000;

struct Node {
    int v, w;

    Node(int v, int w) : v(v), w(w) {}
};

typedef std::vector<Node>::iterator Iterator;

std::vector<Node> edge[MAXN + 1];

int in[MAXN + 1], out[MAXN + 1];
double f[MAXN + 1];

inline void addEdge(const int u, const int v, const int w) {
    edge[u].push_back(Node(v, w));
    in[v]++, out[u]++;
}

inline void topoSort(const int n) {
    static int q[MAXN + 1];
    register int *l = q, *r = q;
    for (register int i = 1; i <= n; i++)
        if (in[i] == 0) *++r = i;
    while (l < r) {
        register int u = *++l;
        for (Iterator p = edge[u].begin(); p != edge[u].end(); p++)
            if (--in[p->v] == 0) *++r = p->v;
    }
    f[n] = 0;
    for (register int i = n - 1, u; i; i--) {
        u = q[i], f[u] = 0;
        for (Iterator p = edge[u].begin(); p != edge[u].end(); p++)
            f[u] += (f[p->v] + p->w) / out[u];
    }
}

inline void solve() {
    register int n, m;
    io >> n >> m;
    for (register int i = 0, u, v, w; i < m; i++)
        io >> u >> v >> w, addEdge(u, v, w);
    topoSort(n);
    io << f[1];
}

#undef long
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}
