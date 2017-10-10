/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1021」Debt 循环的债务 10-10-2017
 * DP
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
const int VAL[] = {-1, 1, 5, 10, 20, 50, 100};
const int INF = 0x3f3f3f3f;

using IO::io;

inline void solve() {
    register int a, b, c;
    io >> a >> b >> c;
    if (a > 0 && b > 0 && c > 0) {
        const int min = std::min(std::min(a, b), c);
        a -= min, b -= min, c -= min;
    } else if (a < 0 && b < 0 && c < 0) {
        const int max = std::max(std::max(a, b), c);
        a -= max, b -= max, c -= max;
    }
    register int has[3][6 + 1], sum = 0, sums[3] = {0, 0, 0};
    for (register int i = 0; i < 3; i++) {
        for (register int j = 0; j < 6; j++)
            io >> has[i][6 - j], sums[i] += has[i][6 - j] * VAL[6 - j];
        sum += sums[i];
    }
    static int f[6 + 1][MAXN + 1][MAXN + 1];
    memset(f, 0x3f, sizeof(f));
    f[0][sums[0]][sums[1]] = 0;
    for (int i = 0; i < 6; i++) {
        for (int a = 0; a <= sum; a++) {
            for (int b = 0; b <= sum - a; b++) {
                const int c = sum - a - b;
                if (f[i][a][b] == INF) continue;

                // a -> b, c
                for (register int tb = 0; tb <= has[0][i + 1]; tb++) {
                    for (register int tc = 0; tc <= has[0][i + 1] - tb; tc++) {
                        if (tb + tc == 0) continue;
                        register const int _a = a - VAL[i + 1] * (tb + tc);
                        register const int _b = b + VAL[i + 1] * tb;
                        register const int _c = c + VAL[i + 1] * tc;
                        if (_a > sum || _b < 0 || _c < 0) continue;
                        f[i + 1][_a][_b] =
                            std::min(f[i + 1][_a][_b], f[i][a][b] + tb + tc);
                    }
                }

                // b -> a, c
                for (register int ta = 0; ta <= has[1][i + 1]; ta++) {
                    for (register int tc = 0; tc <= has[1][i + 1] - ta; tc++) {
                        if (ta + tc == 0) continue;
                        register const int _a = a + VAL[i + 1] * ta;
                        register const int _b = b - VAL[i + 1] * (ta + tc);
                        register const int _c = c + VAL[i + 1] * tc;
                        if (_a < 0 || _b > sum || _c < 0) continue;
                        f[i + 1][_a][_b] =
                            std::min(f[i + 1][_a][_b], f[i][a][b] + ta + tc);
                    }
                }

                // c -> a, b
                for (register int ta = 0; ta <= has[2][i + 1]; ta++) {
                    for (register int tb = 0; tb <= has[2][i + 1] - ta; tb++) {
                        if (ta + tb == 0) continue;
                        register const int _a = a + VAL[i + 1] * ta;
                        register const int _b = b + VAL[i + 1] * tb;
                        register const int _c = c - VAL[i + 1] * (ta + tb);
                        if (_a < 0 || _b < 0 || _c > sum) continue;
                        f[i + 1][_a][_b] =
                            std::min(f[i + 1][_a][_b], f[i][a][b] + ta + tb);
                    }
                }

                // a, b -> c
                for (register int fa = 0; fa <= has[0][i + 1]; fa++) {
                    for (register int fb = 0; fb <= has[1][i + 1]; fb++) {
                        if (fa + fb == 0) continue;
                        register const int _a = a - VAL[i + 1] * fa;
                        register const int _b = b - VAL[i + 1] * fb;
                        register const int _c = c + VAL[i + 1] * (fa + fb);
                        if (_a > sum || _b > sum || _c < 0) continue;
                        f[i + 1][_a][_b] =
                            std::min(f[i + 1][_a][_b], f[i][a][b] + fa + fb);
                    }
                }

                // a, c -> b
                for (register int fa = 0; fa <= has[0][i + 1]; fa++) {
                    for (int fc = 0; fc <= has[2][i + 1]; fc++) {
                        if (fa + fc == 0) continue;
                        register const int _a = a - VAL[i + 1] * fa;
                        register const int _b = b + VAL[i + 1] * (fa + fc);
                        register const int _c = c - VAL[i + 1] * fc;
                        if (_a > sum || _b < 0 || _c > sum) continue;
                        f[i + 1][_a][_b] =
                            std::min(f[i + 1][_a][_b], f[i][a][b] + fa + fc);
                    }
                }

                // b, c -> a
                for (register int fb = 0; fb <= has[1][i + 1]; fb++) {
                    for (int fc = 0; fc <= has[2][i + 1]; fc++) {
                        if (fb + fc == 0) continue;
                        register const int _a = a + VAL[i + 1] * (fb + fc);
                        register const int _b = b - VAL[i + 1] * fb;
                        register const int _c = c - VAL[i + 1] * fc;
                        if (_a < 0 || _b > sum || _c > sum) continue;
                        f[i + 1][_a][_b] =
                            std::min(f[i + 1][_a][_b], f[i][a][b] + fb + fc);
                    }
                }

                f[i + 1][a][b] = std::min(f[i + 1][a][b], f[i][a][b]);
            }
        }
    }

    register int resA = sums[0] - a + c, resB = sums[1] - b + a,
                 resC = sums[2] - c + b;

    if (resA < 0 || resB < 0 || resC < 0 || f[6][resA][resB] == INF)
        io << "impossible";
    else
        io << f[6][resA][resB];
}
}

int main() {
    solve();
    return 0;
}