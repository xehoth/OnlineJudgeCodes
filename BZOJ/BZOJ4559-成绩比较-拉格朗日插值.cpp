/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4559」成绩比较 18-09-2017
 * 拉格朗日插值
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
const int MOD = 1000000007;
const int MAXN = 110;

#define long long long

inline int modPow(int a, int b) {
    register int ret = 1;
    for (; b; b >>= 1, a = (long)a * a % MOD)
        (b & 1) ? ret = (long)ret * a % MOD : 0;
    return ret;
}

struct Task {
    int inv[MAXN + 1], n, m, k;
    int c[MAXN + 1][MAXN + 1];
    int u[MAXN + 1], r[MAXN + 1];
    int f[MAXN + 1][MAXN + 1];
    int s[MAXN + 1];

    inline void init(const int n) {
        inv[0] = inv[1] = 1;
        for (register int i = 2; i <= n; i++)
            inv[i] = (long)i * inv[i - 1] % MOD;
        inv[n] = modPow(inv[n], MOD - 2);
        for (register int i = n - 1; i >= 0; i--)
            inv[i] = inv[i + 1] * (i + 1ll) % MOD;
        for (register int i = 0; i <= n; i++) {
            c[i][0] = 1;
            for (register int j = 1; j <= i; j++)
                c[i][j] = (c[i - 1][j] + c[i - 1][j - 1]) % MOD;
        }
    }

    inline int interpolation(int u, int r, int n) {
        static int f[MAXN + 3], pre[MAXN + 3], suf[MAXN + 3];
        f[0] = 0;
        for (register int i = 1; i <= n + 1; i++) {
            f[i] = (f[i - 1] + (long)modPow(i, n - r) * modPow(u - i, r - 1)) %
                   MOD;
            if (u == i) return f[i];
        }
        pre[0] = suf[n + 2] = 1;
        for (register int i = 1; i <= n + 1; i++)
            pre[i] = (long)pre[i - 1] * (u - i) % MOD;
        for (register int i = n + 1; i; i--)
            suf[i] = (long)suf[i + 1] * (u - i) % MOD;
        register int ret = 0;
        for (register int i = 1, tmp; i <= n + 1; i++) {
            tmp = (long)f[i] * pre[i - 1] % MOD * suf[i + 1] % MOD *
                  inv[i - 1] % MOD * inv[n + 1 - i] % MOD;
            if ((n + 1 - i) % 2) tmp = MOD - tmp;
            ret = (ret + tmp) % MOD;
        }
        return ret;
    }

    inline void solve() {
        io >> n >> m >> k;
        for (register int i = 1; i <= m; i++) io >> u[i];
        for (register int i = 1; i <= m; i++) io >> r[i];
        init(n);
        for (register int i = 1; i <= m; ++i)
            s[i] = interpolation(u[i], r[i], n);
        f[0][0] = 1;
        for (register int i = 0; i < m; i++) {
            for (register int j = 0; j <= n; j++) {
                if (f[i][j] != 0) {
                    for (register int k = j, ex, old; k <= n; k++) {
                        ex = k - j, old = r[i + 1] - 1 - ex;
                        if (old < 0) break;
                        f[i + 1][k] =
                            (f[i + 1][k] +
                             (long)c[j][old] * c[n - j - 1][ex] % MOD *
                                 s[i + 1] % MOD * f[i][j] % MOD) %
                            MOD;
                    }
                }
            }
        }
        io << f[m][n - k - 1];
    }
} task;

#undef long
}

int main() {
    task.solve();
    return 0;
}