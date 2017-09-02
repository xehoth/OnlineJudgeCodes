/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1303」中位数图 02-09-2017
 * 前缀和
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

const int MAXN = 100000;

template <typename T>
struct Array {
    int d[MAXN * 2 + 2];

    inline T &operator[](const int i) { return d[MAXN + i]; }
};

struct Task {
    Array<int> pre, suf;
    int a[MAXN + 1], sum[MAXN + 1];

    inline void solve() {
        register int n, b, pos = 0, ans = 0;
        io >> n >> b;
        for (register int i = 1; i <= n; i++) {
            io >> a[i];
            if (a[i] == b)
                pos = i;
            else
                a[i] = a[i] < b ? -1 : 1;
        }
        pre[0] = suf[0] = 1;
        for (register int i = pos - 1; i; i--)
            sum[i] = sum[i + 1] + a[i], pre[sum[i]]++;
        for (register int i = pos + 1; i <= n; i++)
            sum[i] = sum[i - 1] + a[i], suf[sum[i]]++;
        for (register int i = -n; i <= n; i++) ans += pre[i] * suf[-i];
        io << ans;
    }
} task;
}

int main() {
    task.solve();
    return 0;
}