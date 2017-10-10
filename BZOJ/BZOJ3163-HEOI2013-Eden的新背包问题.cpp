/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3163」Eden的新背包问题 10-10-2017
 * DP
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

const int MAXN = 1000;
const int MAXM = 1000;
const int MAXLOGN = 10;
const int MAXQ = 300000;

using IO::io;

inline void solve() {
    register int n;
    static int w[MAXN], v[MAXN], cnt[MAXN];
    io >> n;
    for (register int i = 0; i < n; i++) io >> w[i] >> v[i] >> cnt[i];
    static std::pair<int, int> r[MAXN];
    static int W[MAXN * MAXLOGN], V[MAXN * MAXLOGN];
    register int N = 0;
    for (register int i = 0; i < n; i++) {
        r[i].first = N + 1;
        for (register int j = 1; j <= cnt[i]; cnt[i] -= j, j *= 2) {
            N++;
            W[N] = w[i] * j;
            V[N] = v[i] * j;
        }
        if (cnt[i]) {
            N++;
            W[N] = w[i] * cnt[i];
            V[N] = v[i] * cnt[i];
        }
        r[i].second = N;
    }

    static int f[MAXN * MAXLOGN + 1][MAXM + 1], g[MAXN * MAXLOGN + 2][MAXM + 1];

    for (register int i = 1; i <= N; i++) {
        for (register int j = 0; j <= MAXM; j++) {
            if (j < W[i])
                f[i][j] = f[i - 1][j];
            else
                f[i][j] = std::max(f[i - 1][j], f[i - 1][j - W[i]] + V[i]);
        }
    }

    for (register int i = N; i >= 1; i--) {
        for (register int j = 0; j <= MAXM; j++) {
            if (j < W[i])
                g[i][j] = g[i + 1][j];
            else
                g[i][j] = std::max(g[i + 1][j], g[i + 1][j - W[i]] + V[i]);
        }
    }

    register int q;
    io >> q;
    for (register int i = 0; i < q; i++) {
        register int id, m;
        io >> id >> m;

        register int ans = 0, a = r[id].first - 1, b = r[id].second + 1;
        for (register int j = 0; j <= m; j++)
            ans = std::max(ans, f[a][j] + g[b][m - j]);
        io << ans << '\n';
    }
}
}

int main() {
    solve();
    return 0;
}