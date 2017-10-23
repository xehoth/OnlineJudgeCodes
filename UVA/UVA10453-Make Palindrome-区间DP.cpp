/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「UVA 10453」Make Palindrome 23-10-2017
 * 区间 DP
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

const int OUT_LEN = 1000000;
char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        return read(x), *this;
    }

    ~InputOutputStream() { flush(); }
} io;
}

namespace {

using IO::io;

const int MAXN = 1010;
const int INF = 0x3f3f3f3f;
char s[MAXN];
int f[MAXN][MAXN];

int dp(int l, int r) {
    if (l >= r) return 0;
    register int &ret = f[l][r];
    if (~ret) return ret;
    ret = INF;
    if (s[l] == s[r]) ret = dp(l + 1, r - 1);
    return ret = std::min(ret, std::min(dp(l + 1, r), dp(l, r - 1)) + 1);
}

void print(int l, int r) {
    if (l > r) return;
    if (l == r) {
        std::cout << s[l];
        return;
    }
    if (s[l] == s[r]) {
        std::cout << s[l], print(l + 1, r - 1), std::cout << s[r];
    } else if (f[l][r] == f[l + 1][r] + 1) {
        std::cout << s[l], print(l + 1, r), std::cout << s[l];
    } else {
        std::cout << s[r], print(l, r - 1), std::cout << s[r];
    }
}

inline void solveCase() {
    register int n = strlen(s);
    for (register int i = 0; i <= n; i++)
        memset(f[i], -1, sizeof(int) * (n + 1));
    std::cout << dp(0, n - 1) << ' ';
    print(0, n - 1), std::cout << '\n';
}

inline void solve() {
    while (std::cin >> s) solveCase();
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}
