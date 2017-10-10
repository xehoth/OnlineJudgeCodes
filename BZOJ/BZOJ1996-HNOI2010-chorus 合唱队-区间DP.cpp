/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1996」chorus 合唱队 10-10-2017
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

const int MAXN = 1000;

using IO::io;

int f[MAXN + 1][MAXN + 1][3], a[MAXN + 1];
bool vis[MAXN + 1][MAXN + 1][3];

const int STATUS_L = 0;
const int STATUS_R = 1;
const int STATUS_LR = 2;
const int MOD = 19650827;

inline int dp(int l, int r, int flag) {
    register int &ret = f[l][r][flag];
    if (vis[l][r][flag]) return ret;
    vis[l][r][flag] = true;
    if (r - l + 1 == 2) {
        if (a[l] > a[r])
            return ret = 0;
        else if (flag == STATUS_LR)
            return ret = 2;
        else
            return ret = 1;
    }
    if (flag != STATUS_L) {
        register const int L = l, R = r - 1;
        if (a[r] > a[L] && a[r] > a[R])
            ret += dp(L, R, STATUS_LR);
        else if (a[r] > a[L])
            ret += dp(L, R, STATUS_L);
        else if (a[r] > a[R])
            ret += dp(L, R, STATUS_R);
    }
    if (flag != STATUS_R) {
        register const int L = l + 1, R = r;
        if (a[l] < a[L] && a[l] < a[R])
            ret += dp(L, R, STATUS_LR);
        else if (a[l] < a[L])
            ret += dp(L, R, STATUS_L);
        else if (a[l] < a[R])
            ret += dp(L, R, STATUS_R);
    }
    return ret %= MOD;
}

inline void solve() {
    register int n;
    io >> n;
    for (register int i = 0; i < n; i++) io >> a[i];
    io << dp(0, n - 1, STATUS_LR);
}
}

int main() {
    solve();
    return 0;
}