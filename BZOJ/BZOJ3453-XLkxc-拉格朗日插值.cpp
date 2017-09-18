/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3453」XLkxc 18-09-2017
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
typedef unsigned int uint;

const uint MOD = 1234567891;
const int MAXN = 123;

typedef unsigned long long ulong;

inline int modPow(int a, int b) {
    register int ret = 1;
    for (; b; b >>= 1, a = (ulong)a * a % MOD)
        (b & 1) ? ret = (ulong)ret * a % MOD : 0;
    return ret;
}

struct Task {
    uint inv[MAXN + 10];
    int k, a, n, d;

    inline void init(const int n) {
        inv[0] = inv[1] = 1;
        for (register int i = 2; i <= n; i++)
            inv[i] = (ulong)i * inv[i - 1] % MOD;
        inv[n] = modPow(inv[n], MOD - 2);
        for (register int i = n - 1; i >= 0; i--)
            inv[i] = inv[i + 1] * (i + 1ll) % MOD;
    }

    inline uint interpolation(uint *f, uint u, int n) {
        static uint pre[MAXN + 10], suf[MAXN + 10];
        pre[0] = suf[n + 2] = 1;
        for (register int i = 1; i <= n + 1; i++)
            pre[i] = (ulong)pre[i - 1] * (u - i + MOD) % MOD;
        for (register int i = n + 1; i; i--)
            suf[i] = (ulong)suf[i + 1] * (u - i + MOD) % MOD;
        register uint ret = 0, tmp;
        for (register int i = 1; i <= n + 1; i++) {
            tmp = (ulong)f[i] * pre[i - 1] % MOD * suf[i + 1] % MOD *
                  inv[i - 1] % MOD * inv[n + 1 - i] % MOD;
            if ((n + 1 - i) % 2) tmp = MOD - tmp;
            ret = (ret + tmp) % MOD;
        }
        return ret;
    }

    inline void solve() {
        register int T;
        io >> T;
        init(MAXN + 5);
        static uint g[MAXN + 10], f[MAXN + 10];
        while (T--) {
            io >> k >> a >> n >> d;
            for (register int i = 0; i <= k + 3; i++) g[i] = modPow(i, k);
            for (register int i = 1; i <= k + 3; i++)
                g[i] = (g[i] + g[i - 1]) % MOD;
            for (register int i = 1; i <= k + 3; i++)
                g[i] = (g[i] + g[i - 1]) % MOD;
            f[0] = interpolation(g, a, k + 2);
            for (register int i = 1; i <= k + 5; i++)
                f[i] = (f[i - 1] +
                        interpolation(g, (a + (ulong)d * i) % MOD, k + 2)) %
                       MOD;
            io << interpolation(f, n, k + 4) << '\n';
        }
    }
} task;
}

int main() {
    task.solve();
    return 0;
}