/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 1991」豆豆游戏 17-10-2017
 * 区间 DP
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

const int MAXN = 200;

int f[MAXN + 1][MAXN + 1], num[MAXN + 1];
bool vis[MAXN + 1][MAXN + 1];
char s[MAXN + 1], ch[MAXN + 1];

int dp(int l, int r) {
    register int &ret = f[l][r];
    if (vis[l][r]) return ret;
    vis[l][r] = true;
    if (l == r) return ret = 3 - num[l];
    if (ch[l] == ch[r]) {
        ret = dp(l + 1, r - 1) + std::max(0, 3 - num[l] - num[r]);
        if (num[l] + num[r] < 4) {
            for (register int k = l + 2; k < r; k += 2) {
                if (num[k] == 1) {
                    ret = std::min(ret, dp(l + 1, k - 1) + dp(k + 1, r - 1));
                }
            }
        }
    }
    for (register int k = l; k < r; k++)
        ret = std::min(ret, dp(l, k) + dp(k + 1, r));
    return ret;
}

inline void solveCase() {
    register int n = IO::read(s + 1);
    register int cnt = 0;
    memset(num, 0, sizeof(int) * (n + 1));
    for (register int i = 1; i <= n; i++) {
        if (s[i] != s[i - 1])
            ch[++cnt] = s[i], num[cnt] = 1;
        else
            num[cnt]++;
    }
    for (register int i = 0; i <= cnt; i++) {
        memset(f[i], 0x3f, sizeof(int) * (cnt + 1));
        memset(vis[i], 0, sizeof(bool) * (cnt + 1));
    }
    io << dp(1, cnt) << '\n';
}

inline void solve() {
    register int T;
    io >> T;
    while (T--) solveCase();
}
}

int main() {
    solve();
    return 0;
}