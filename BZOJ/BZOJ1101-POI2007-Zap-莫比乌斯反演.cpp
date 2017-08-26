/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1101」 18-07-2017
 * 莫比乌斯反演
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
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
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
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, oh - obuf, stdout), oh = obuf) : 0;
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
}

namespace Task {

const int MAXN = 50010;

int prime[MAXN], mu[MAXN], cnt, sum[MAXN];
bool vis[MAXN];

inline void fastLinearSieveMethod(const int n) {
    mu[1] = 1;
    for (register int i = 2; i <= n; i++) {
        if (!vis[i]) prime[cnt++] = i, mu[i] = -1;
        for (register int j = 0, tmp; j < cnt && (tmp = i * prime[j]) <= n;
             j++) {
            vis[tmp] = true;
            if (i % prime[j] == 0) {
                mu[tmp] = 0;
                break;
            } else {
                mu[tmp] = -mu[i];
            }
        }
    }
    for (register int i = 1; i <= n; i++) sum[i] = sum[i - 1] + mu[i];
}

inline int solve(int n, int m) {
    if (n > m) std::swap(n, m);
    register int ans = 0, pos;
    for (register int i = 1; i <= n; i = pos + 1) {
        pos = std::min(n / (n / i), m / (m / i));
        ans += (sum[pos] - sum[i - 1]) * (n / i) * (m / i);
    }
    return ans;
}

inline void solve() {
    using namespace IO;
    register int t;
    fastLinearSieveMethod(50000);
    read(t);
    for (register int i = 0, a, b, c; i < t; i++) {
        read(a), read(b), read(c);
        print(solve(a / c, b / c)), print('\n');
    }
}
}

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    Task::solve();
    IO::flush();
    return 0;
}