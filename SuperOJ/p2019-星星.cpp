/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 2019」星星 30-10-2017
 *
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

const int MAXN = 100000;
int n, m;

struct Data {
    int deg, id;
    inline bool operator<(const Data &d) const { return deg > d.deg; }
} d[MAXN + 1];

std::vector<int> edge[MAXN + 1];

typedef std::vector<int>::iterator Iterator;
bool vis[MAXN + 1];
int tag[MAXN + 1];

inline void solveCase() {
    for (register int i = 1, u, v; i <= m; i++) {
        io >> u >> v, edge[u].push_back(v), edge[v].push_back(u);
        d[u].deg++, d[v].deg++;
    }
    for (register int i = 1; i <= n; i++) d[i].id = i;
    memset(vis, false, sizeof(bool) * (n + 1));
    memset(tag, 0, sizeof(int) * (n + 1));
    register long long ans = 0;
    std::sort(d + 1, d + n + 1);
    for (register int i = 1, cur, cnt; i <= n; i++) {
        vis[cur = d[i].id] = true;
        for (register Iterator p = edge[cur].begin(); p != edge[cur].end(); p++)
            tag[*p] = cur;
        for (register Iterator p = edge[cur].begin(); p != edge[cur].end();
             p++) {
            cnt = 0;
            if (!vis[*p])
                for (register Iterator k = edge[*p].begin();
                     k != edge[*p].end(); k++)
                    if (tag[*k] == cur) cnt++;
            ans += ((long long)cnt * (long long)(cnt - 1)) / 2;
        }
    }
    io << ans << '\n';
    for (register int i = 1; i <= n; i++) edge[i].clear();
    for (register int i = 1; i <= n; i++) d[i].deg = 0;
}

inline void solve() {
    register int T;
    for (io >> T, T--; T--;) io >> n >> m, solveCase();
    io >> n >> m, solveCase();
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}