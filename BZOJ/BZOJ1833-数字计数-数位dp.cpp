/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1833」数字计数 24-09-2017
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

#define long long long

int digit[14];
long f[10][14][14];

inline long dfs(int pos, int num, int status, int limit, bool top) {
    if (pos < 1) return status;
    if (!limit && !top && f[num][pos][status] != -1) return f[num][pos][status];
    register int end = limit ? digit[pos] : 9;
    register long ret =
        ((!top || pos == 1)
             ? dfs(pos - 1, num, status + (num == 0), limit && digit[pos] == 0,
                   false)
             : dfs(pos - 1, num, status, limit && digit[pos] == 0, true));
    for (register int i = 1; i <= end; i++)
        ret +=
            dfs(pos - 1, num, status + (num == i), limit && (i == end), false);
    return (!limit && !top) ? f[num][pos][status] = ret : ret;
}

inline long solve(long n, const int num) {
    if (n < 0) return 0;
    if (n == 0) return num == 0;
    register int len = 0;
    for (; n; n /= 10) digit[++len] = n % 10;
    return dfs(len, num, 0, 1, true);
}

inline void solve() {
    register long a, b;
    io >> a >> b;
    memset(f, -1, sizeof(f));
    for (register int i = 0; i <= 8; i++)
        io << solve(b, i) - solve(a - 1, i) << ' ';
    io << solve(b, 9) - solve(a - 1, 9) << '\n';
}

#undef long
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}