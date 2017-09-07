/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「UVA 10766」Organising the Organisation 07-09-2017
 * 矩阵树定理
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

const int MAXN = 60;
typedef long double ld;
const ld EPS = 1e-9;

struct Task {
#define long long long
    ld a[MAXN + 1][MAXN + 1];
    bool vis[MAXN + 1][MAXN + 1];

    template <typename T, size_t size>
    inline bool gaussJordan(T a[size][size], const int n) {
        for (register int i = 0, idx; idx = i, i < n; i++) {
            for (register int j = i + 1; j < n; j++)
                if (fabs(a[j][i]) > fabs(a[idx][i])) idx = j;
            if (fabs(a[idx][i]) < EPS) return false;
            if (i != idx)
                for (register int j = i; j <= n; j++)
                    std::swap(a[i][j], a[idx][j]);
            for (register int j = 0; j < n; j++)
                if (i != j)
                    for (register int k = n; k >= i; k--)
                        a[j][k] -= a[i][k] / a[i][i] * a[j][i];
        }
    }

    template <typename T, size_t size>
    inline long getDetVal(T a[size][size], const int n) {
        gaussJordan(a, n);
        register ld ans = 1;
        for (register int i = 0; i < n - 1; i++) ans *= a[i][i];
        return round(fabs(ans));
    }

    inline void solve() {
        register int n, m, k;
        while (IO::read(n) && IO::read(m) && IO::read(k)) {
            memset(vis, 0, sizeof(vis));
            memset(a, 0, sizeof(a));
            for (register int i = 0, u, v; i < m; i++)
                io >> u >> v, u--, v--, vis[u][v] = vis[v][u] = true;
            for (register int i = 0; i < n; i++)
                for (register int j = i + 1; j < n; j++)
                    if (!vis[i][j]) a[i][i]++, a[j][j]++, a[i][j]--, a[j][i]--;
            io << getDetVal(a, n) << '\n';
        }
    }
#undef long
} task;
}

int main() {
    task.solve();
    return 0;
}
