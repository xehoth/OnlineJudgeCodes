/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4521」手机号码 26-09-2017
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

int f[12][10][10][2][2][2];

int digit[12];

inline long dfs(int pos, int pre, int last, int status, int has8, int has4,
                int limit, bool top) {
    if (pos < 1) return status && !(has8 && has4);
    if (!limit && !top && f[pos][pre][last][status][has8][has4] != -1)
        return f[pos][pre][last][status][has8][has4];
    register int end = limit ? digit[pos] : 9;
    register long ret =
        ((!top || pos == 1)
             ? dfs(pos - 1, last, 0, status || (last == 0 && pre == 0), has8,
                   has4, limit && digit[pos] == 0, false)
             : dfs(pos - 1, last, 0, status, has8, has4,
                   limit && digit[pos] == 0, true));
    for (register int i = 1; i <= end; i++)
        ret += dfs(pos - 1, last, i, status || (i == last && i == pre),
                   has8 || i == 8, has4 || i == 4, limit && i == end, false);
    return (!limit && !top) ? f[pos][pre][last][status][has8][has4] = ret : ret;
}

inline long solve(long x) {
    register int len = 0;
    for (; x; x /= 10) digit[++len] = x % 10;
    return dfs(len, 0, 0, 0, 0, 0, 1, true);
}

inline void solve() {
    register long l, r;
    io >> l >> r;
    memset(f, -1, sizeof(f));
    io << solve(r) - solve(l - 1);
}

#undef long
}

int main() {
    solve();
    return 0;
}