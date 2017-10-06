/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4337」树的同构 06-10-2017
 * 最小表示法
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

inline void print(const char *s) {
    for (; *s; s++) print(*s);
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
}

namespace {

const int MAXN = 55;

using IO::io;

int M, n, f[MAXN], son[MAXN], max;

typedef std::string String;
String h[MAXN], q[MAXN], val[MAXN];

std::vector<int> edge[MAXN + 1];

typedef std::vector<int>::iterator Iterator;

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
}

inline void getRoot(int x, int y) {
    son[x] = 1, f[x] = 0;
    for (Iterator i = edge[x].begin(); i != edge[x].end(); i++) {
        if (*i != y) {
            getRoot(*i, x), son[x] += son[*i];
            if (son[*i] > f[x]) f[x] = son[*i];
        }
    }
    if (n - son[x] > f[x]) f[x] = n - son[x];
    if (f[x] < max) max = f[x];
}

inline void dfs(int x, int y) {
    h[x] = "(";
    for (Iterator i = edge[x].begin(); i != edge[x].end(); i++)
        if (*i != y) dfs(*i, x);
    register int t = 0;
    for (Iterator i = edge[x].begin(); i != edge[x].end(); i++)
        if (*i != y) q[t++] = h[*i];
    if (t > 1) std::sort(q, q + t);
    for (register int i = 0; i < t; i++) h[x] += q[i];
    h[x] += ")";
}

inline String solveCase() {
    register int i;
    String t = "";
    io >> n;
    for (register int i = 0; i <= n; i++) edge[i].clear();
    max = n;
    for (register int i = 1, x; i <= n; i++) {
        io >> x;
        if (x) addEdge(i, x);
    }
    getRoot(1, 0);
    for (register int i = 1; i <= n; i++) {
        if (f[i] == max) {
            dfs(i, 0);
            if (h[i] > t) t = h[i];
        }
    }
    return t;
}

using IO::io;

inline void solve() {
    io >> M;
    for (register int i = 1; i <= M; i++) val[i] = solveCase();
    for (register int i = 1, j = 0, k = 0; i <= M; io << k << '\n', i++)
        for (j = k = i; j; j--)
            if (val[j] == val[i]) k = j;
}
}

int main() {
    // freopen("family.in", "r", stdin);
    // freopen("family.out", "w", stdout);
    solve();
    return 0;
}