/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1799」self 同类分布 26-09-2017
 * 数位 dp
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
    for (iosig = false, c = read(); !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
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

const int OUT_LEN = 10000000;

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

#define long long long

using IO::io;

const int MAX_SUM = 162;

long f[20][MAX_SUM + 2][MAX_SUM + 2];
int now;
int digit[20];

inline long dfs(int pos, int sum, int mod, int limit) {
    if (pos < 1) return sum == now && mod == 0;
    if (!limit && f[pos][sum][mod] != -1) return f[pos][sum][mod];
    register int end = limit ? digit[pos] : 9;
    register long ret = 0;
    for (register int i = 0; i <= end; i++)
        ret += dfs(pos - 1, sum + i, (mod * 10 + i) % now, limit && i == end);
    return !limit ? f[pos][sum][mod] = ret : ret;
}

inline long solve(long x) {
    register int len = 0;
    for (; x; x /= 10) digit[++len] = x % 10;
    return len * 9 < now ? 0 : dfs(len, 0, 0, 1);
}

inline void solve() {
    register long l, r, ans = 0;
    io >> l >> r;
    for (register int i = 1; i <= MAX_SUM; i++) {
        memset(f, -1, sizeof(f));
        now = i;
        ans += solve(r) - solve(l - 1);
    }
    io << ans;
}

#undef long
}

int main() {
    solve();
    return 0;
}