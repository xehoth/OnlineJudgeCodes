/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1090」字符串折叠 10-10-2017
 * 区间DP
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

const int MAXN = 100;

inline int len(int x) {
    register int ret = 0;
    do
        ret++;
    while (x /= 10);
    return ret;
}

int f[MAXN + 1][MAXN + 1];
bool vis[MAXN + 1][MAXN + 1];
char s[MAXN + 1];

inline int dp(int l, int r) {
    register int &ret = f[l][r];
    if (vis[l][r]) return ret;
    vis[l][r] = true;
    if (l == r) return ret = 1;
    if (l > r) return ret = 0;
    ret = r - l + 1;
    for (register int i = 1, pre, pos, cnt; i <= r - l + 1; i++) {
        pre = dp(l, l + i - 1), pos = l + i, cnt = 1;
        while (pos + i - 1 <= r && memcmp(s + l, s + pos, i) == 0)
            pos += i, cnt++;
        ret = std::min(ret, dp(pos, r) + len(cnt) + pre + 2);
    }
    for (register int i = 1, pre, pos, cnt; i <= r - l + 1; i++) {
        pre = dp(r - i + 1, r), pos = r - i - i + 1, cnt = 1;
        while (pos >= l && memcmp(s + r - i + 1, s + pos, i) == 0)
            pos -= i, cnt++;
        ret = std::min(ret, dp(l, pos + i - 1) + len(cnt) + pre + 2);
    }
    for (register int i = 1; i <= r - l + 1 - 1; i++)
        ret = std::min(ret, dp(l, i) + dp(i + 1, r));
    return ret;
}

inline void solve() {
    register int n = IO::read(s);
    io << dp(0, n - 1);
}
}

int main() {
    solve();
    return 0;
}