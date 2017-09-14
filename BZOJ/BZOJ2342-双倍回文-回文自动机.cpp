/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2342」双倍回文 14-09-2017
 * 回文自动机
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
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
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

const int MAX_SIGMA = 26;
const int MAXN = 500010;
const int SIGMA = 26;

struct Node {
    int len, cnt, fail, c[MAX_SIGMA];
};

struct Graph {
    typedef std::vector<int> Vector;
    Vector edge[MAXN + 2];

    inline void addEdge(const int u, const int v) { edge[u].push_back(v); }

    inline Vector &operator[](const int i) { return edge[i]; }
};

struct PalindromicTree {
    static const int even = 0;
    static const int odd = 1;

    Node d[MAXN + 1];
    int cur, last, size;
    char s[MAXN + 1];

    PalindromicTree() {
        d[odd].len = -1, d[even].fail = d[odd].fail = odd, s[size = 0] = -1;
        cur = 2;
    }

    Graph g;

    inline void extend(char c) {
        s[++size] = c;
        register int &p = last;
        while (s[size - d[p].len - 1] != s[size]) p = d[p].fail;
        if (!d[p].c[c]) {
            register int np = cur++, k = d[p].fail;
            d[np].len = d[p].len + 2;
            while (s[size - d[k].len - 1] != s[size]) k = d[k].fail;
            d[np].fail = d[k].c[c], d[p].c[c] = np;
            g.addEdge(d[np].fail, np);
        }
        p = d[p].c[c], d[p].cnt++;
    }

    int ans, vis[MAXN + 1];

    inline void dfs(const int u) {
        if (d[u].len % 4 == 0 && vis[d[u].len / 2])
            ans = std::max(ans, d[u].len);
        vis[d[u].len]++;
        for (register int i = 0; i < g[u].size(); i++) dfs(g[u][i]);
        vis[d[u].len]--;
    }

    inline void solve() {
        register int n;
        io >> n;
        static char buf[MAXN + 1];
        io >> buf;
        for (register int i = 0; i < n; i++) extend(buf[i] - 'a');
        dfs(0);
        io << ans;
    }
} task;
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    task.solve();
    return 0;
}
