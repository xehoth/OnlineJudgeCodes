/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4338」糖果 06-10-2017
 *
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

using IO::io;

const int MAXN = 100010;

#define long long long

int pre[MAXN + 1], suc[MAXN + 1], prime[MAXN + 1], cnt, n, m, k, mod;
bool vis[MAXN + 1];

inline void init() {
    vis[1] = true, prime[++cnt] = 2;
    for (register int i = 3; i <= MAXN; i += 2) {
        if (!vis[i]) prime[++cnt] = i;
        for (register int j = 1, t; j <= cnt && (t = i * prime[j]) <= MAXN;
             j++) {
            vis[t] = true;
            if (i % prime[j] == 0) break;
        }
    }
}

inline int calc(int a, int b) {
    register int ret = 0;
    while (b) ret += (b /= a);
    return ret;
}

inline int nCr(int a, int b) {
    register int ret = 1;
    for (register int i = 1; i <= b; i++) pre[i] = a - i + 1;
    for (register int i = 1; prime[i] <= b; i++) suc[i] = calc(prime[i], b);
    for (register int i = 1; prime[i] <= b; i++)
        for (register int j = a % prime[i] + 1; j <= b; j += prime[i])
            while (suc[i] && pre[j] % prime[i] == 0)
                pre[j] /= prime[i], suc[i]--;
    for (register int i = 1; i <= b; i++) ret = ((long)ret * pre[i]) % mod;
    return ret;
}

inline int solve(int a, int b) {
    register int ret = 1;
    for (register int i = 1; i <= b; i++) ret = (ret * (a - i + 1ll)) % mod;
    return ret;
}

inline void solve() {
    init();
    io >> n >> m >> k >> mod;
    io << solve(nCr(k + m - 1, m), n);
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
}