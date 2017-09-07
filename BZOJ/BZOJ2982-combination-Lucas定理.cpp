/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2982」combination 07-09-2017
 * Lucas 定理
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

const int MOD = 10007;

struct Task {
    int fac[MOD + 1], inv[MOD + 1];

    inline int modPow(int a, int b) {
        register int ret = 1;
        for (; b; b >>= 1, a = a * a % MOD) (b & 1) ? ret = ret * a % MOD : 0;
        return ret;
    }

    inline void init() {
        fac[0] = 1;
        for (register int i = 1; i < MOD; i++) fac[i] = fac[i - 1] * i % MOD;
        inv[MOD - 1] = modPow(fac[MOD - 1], MOD - 2);
        for (register int i = MOD - 2; i >= 0; i--)
            inv[i] = inv[i + 1] * (i + 1) % MOD;
    }

    inline int nCr(int n, int r) {
        if (n < r) return 0;
        if (n < MOD && r < MOD) return fac[n] * inv[r] % MOD * inv[n - r] % MOD;
        return nCr(n % MOD, r % MOD) * nCr(n / MOD, r / MOD) % MOD;
    }

    inline void solve() {
        register int T;
        io >> T, init();
        for (register int n, m; T--;) {
            io >> n >> m;
            io << nCr(n, m) << '\n';
        }
    }
} task;
}

int main() {
    task.solve();
    return 0;
}