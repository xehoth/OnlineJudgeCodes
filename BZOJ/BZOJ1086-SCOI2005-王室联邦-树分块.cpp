/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1086」王室联邦 12-10-2017
 * 树分块
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
int n, b;

const int MAXN = 100000;

std::vector<int> edge[MAXN + 1];

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

typedef std::vector<int>::iterator Iterator;

std::vector<int> st;
bool vis[MAXN + 1];

int top[MAXN + 1], id[MAXN + 1], cnt;

inline void dfs(const int u) {
    register size_t status = st.size();
    vis[u] = true;
    for (register Iterator p = edge[u].begin(); p != edge[u].end(); p++) {
        if (!vis[*p]) {
            dfs(*p);
            if ((int)(st.size() - status) >= b) {
                top[++cnt] = u;
                while (st.size() != status) id[st.back()] = cnt, st.pop_back();
            }
        }
    }
    st.push_back(u);
}

inline void solve() {
    io >> n >> b;
    for (register int i = 1, u, v; i < n; i++) io >> u >> v, addEdge(u, v);
    dfs(1);
    while (!st.empty()) id[st.back()] = cnt, st.pop_back();
    io << cnt << '\n';
    for (register int i = 1; i <= n; i++) io << id[i] << ' ';
    io << '\n';
    for (register int i = 1; i <= cnt; i++) io << top[i] << ' ';
}
}

int main() {
    solve();
    return 0;
}