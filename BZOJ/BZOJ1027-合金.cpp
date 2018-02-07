/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1027」合金 23-11-2017
 * 计算几何
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

inline void read(double &t) {
    static char c;
    static bool iosig;
    register int x = 0;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    if (c == '.') {
        register int y = 0, l = 1;
        for (c = read(); isdigit(c); c = read())
            y = y * 10 + (c ^ '0'), l *= 10;
        t = x + (double)y / l;
    } else {
        t = x;
    }
    iosig ? t = -t : 0;
}

struct InputStream {
    template <typename T>
    inline InputStream &operator>>(T &x) {
        return read(x), *this;
    }
} io;
}  // namespace IO

namespace {

const int MAXN = 500 + 9;

using IO::io;

struct Point {
    double x, y;

    Point() {}

    Point(const double x, const double y) : x(x), y(y) {}

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline double operator*(const Point &p) const { return x * p.y - y * p.x; }

    inline double dot(const Point &p) const { return x * p.x + y * p.y; }
} a[MAXN], b[MAXN];

int m, n;

const int INF = 0x3f3f3f3f;
const double EPS = 1e-8;

int map[MAXN][MAXN], f[MAXN][MAXN];

inline void solve() {
    io >> m >> n;
    memset(map, 0x3f, sizeof(map));
    register double tmp;
    for (register int i = 1; i <= m; i++) io >> a[i].x >> a[i].y >> tmp;
    for (register int i = 1; i <= n; i++) io >> b[i].x >> b[i].y >> tmp;
    for (register int i = 1; i <= m; i++) {
        for (register int j = 1, k; j <= m; j++) {
            for (k = 1; k <= n; k++) {
                register double c = (a[i] - b[k]) * (a[j] - b[k]);
                if (c > EPS) break;
                if (fabs(c) < EPS && (a[i] - b[k]).dot(a[j] - b[k]) > EPS)
                    break;
            }
            if (k == n + 1) map[i][j] = 1;
        }
    }
    memcpy(f, map, sizeof(f));
    for (register int k = 1; k <= m; k++)
        for (register int i = 1; i <= m; i++)
            if (f[i][k] < INF)
                for (register int j = 1; j <= m; j++)
                    f[i][j] = std::min(f[i][j], f[i][k] + f[k][j]);
    register int ans = INF;
    for (register int i = 1; i <= m; i++) ans = std::min(ans, f[i][i]);
    if (ans == INF) ans = -1;
    std::cout << ans;
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}
