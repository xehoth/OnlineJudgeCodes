/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2228」礼物 05-10-2017
 * DP + 单调栈
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
}

namespace {

using namespace IO;

const int MAXN = 150;

int f[MAXN + 1][MAXN + 1][MAXN + 1];
char map[MAXN + 1][MAXN + 1][MAXN + 1];

inline char get1(int x, int y, int z) { return map[x][y][z]; }
inline char get2(int x, int y, int z) { return map[y][x][z]; }
inline char get3(int x, int y, int z) { return map[y][z][x]; }

int sum[MAXN + 1][MAXN + 1];

template <typename T, typename Func>
inline void dp(T &f, int x, int n, int m, const Func &get) {
    for (register int i = 1; i <= n; i++)
        for (register int j = 1; j <= m; j++)
            sum[i][j] = sum[i - 1][j] + sum[i][j - 1] - sum[i - 1][j - 1] +
                        (get(x, i, j) == 'P');
    for (register int i = 1; i <= n; i++) {
        for (register int j = 1; j <= m; j++) {
            for (register int k = f[i - 1][j - 1] + 1; k >= 0; k--) {
                if (sum[i][j] - sum[i - 1][j] - sum[i][j - k] +
                            sum[i - 1][j - k] ==
                        0 &&
                    sum[i][j] - sum[i][j - 1] - sum[i - k][j] +
                            sum[i - k][j - 1] ==
                        0) {
                    f[i][j] = k;
                    break;
                }
            }
        }
    }
}

template <typename T>
inline int solve(int x, int y, int z, const T &get) {
    for (register int i = 1; i <= x; i++) dp(f[i], i, y, z, get);
    static int lBound[MAXN + 1], rBound[MAXN + 1];
    register int ans = 0;
    for (register int i = 1; i <= y; i++) {
        for (register int j = 1; j <= z; j++) {
            static int st[MAXN + 1];
            register int *top = st;
            *top = 0;
            for (register int k = 1; k <= x; k++) {
                while (top != st && f[k][i][j] <= f[*top][i][j]) top--;
                lBound[k] = *top, *++top = k;
            }
            *(top = st) = x + 1;
            for (register int k = x; k; k--) {
                while (top != st && f[k][i][j] <= f[*top][i][j]) top--;
                rBound[k] = *top, *++top = k;
            }
            for (register int k = 1; k <= x; k++)
                ans =
                    std::max(ans, 4 * (rBound[k] - lBound[k] - 1) * f[k][i][j]);
        }
    }
    return ans;
}

inline void solve() {
    register int p, q, r;
    read(q), read(p), read(r);
    for (register int i = 1; i <= p; i++)
        for (register int j = 1; j <= q; j++) read(map[i][j] + 1);
    std::cout << std::max(std::max(solve(p, q, r, get1), solve(q, p, r, get2)),
                          solve(r, p, q, get3));
}
}

int main() {
    solve();
    return 0;
}