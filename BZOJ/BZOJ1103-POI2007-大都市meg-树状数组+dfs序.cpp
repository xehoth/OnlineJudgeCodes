/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1103」大都市meg 15-10-2017
 * 树状数组 + dfs 序
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
}

namespace {

using IO::io;

const int MAXN = 250010;

int d[MAXN + 1], n, m;

inline void modify(register int k, const register int v) {
    for (; k <= n; k += k & -k) d[k] += v;
}

inline int query(register int k) {
    register int ret = 0;
    for (; k; k ^= k & -k) ret += d[k];
    return ret;
}

std::vector<int> edge[MAXN + 1];

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

typedef std::vector<int>::iterator Iterator;
int in[MAXN + 1], out[MAXN + 1], idx;

void dfs(const int u, const int fa) {
    in[u] = ++idx;
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); p++)
        if (*p != fa) dfs(*p, u);
    out[u] = idx;
}

inline void solve() {
    io >> n;
    for (register int i = 1, u, v; i < n; i++) io >> u >> v, addEdge(u, v);
    dfs(1, 0);
    for (register int i = 2; i <= n; i++)
        modify(in[i], 1), modify(out[i] + 1, -1);
    io >> m, m = m + n - 1;
    register char cmd;
    for (register int a, b; m--;) {
        io >> cmd;
        switch (cmd) {
            case 'W':
                io >> a, io << query(in[a]) << '\n';
                break;
            case 'A':
                io >> a >> b, modify(in[b], -1), modify(out[b] + 1, 1);
                break;
        }
    }
}
}

int main() {
    solve();
    return 0;
}