/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1055」玩具取名 10-10-2017
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

const int MAXN = 200;

using IO::io;

bool map[4][4][4];

inline int id(char c) {
    switch (c) {
        case 'W':
            return 0;
        case 'I':
            return 1;
        case 'N':
            return 2;
        case 'G':
            return 3;
    }
}

char s[MAXN + 1], a[MAXN + 1];

int f[MAXN + 1][MAXN + 1];
bool vis[MAXN + 1][MAXN + 1];

inline int dp(int l, int r) {
    register int &ret = f[l][r];
    if (vis[l][r]) return ret;
    vis[l][r] = true;
    if (l == r) return ret = 1 << a[l];
    for (register int i = l, a, b; i < r; i++) {
        a = dp(l, i), b = dp(i + 1, r);
        for (register int j = 0; j < 4; j++) {
            if (a & (1 << j)) {
                for (register int k = 0; k < 4; k++) {
                    if (b & (1 << k)) {
                        for (register int t = 0; t < 4; t++) {
                            if (map[j][k][t]) ret |= 1 << t;
                        }
                    }
                }
            }
        }
    }
    return ret;
}

inline void solve() {
    register int cnt[4];
    io >> cnt[0] >> cnt[1] >> cnt[2] >> cnt[3];
    for (register int j = 0; j < 4; j++)
        for (register int i = 1; i <= cnt[j]; i++)
            io >> s, map[id(s[0])][id(s[1])][j] = true;
    register int n = IO::read(s);
    std::transform(s, s + n, a, id);
    register int res = dp(0, n - 1);
    register bool flag = false;
    for (register int i = 0; i < 4; i++)
        if (res & (1 << i)) io << ("WING"[i]), flag = true;
    if (!flag) io << "The name is wrong!";
}
}

int main() {
    solve();
    return 0;
}