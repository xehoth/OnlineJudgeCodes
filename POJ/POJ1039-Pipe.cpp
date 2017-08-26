/*******************************************************************************
 * Copyright (c) 2016-2017, xehoth
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name xehoth, nor the names of its contributors may be used
 *       to endorse or promote products derived from this software without
 *       specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY XEHOTH AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEHOTH AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/
// #include <bits/stdc++.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <cstring>
/**
 * 「POJ 1039」Pipe 19-05-2017
 * 计算几何
 * @author xehoth
 */
namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template<class T>
inline bool read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return false;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read())
        x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
    return true;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template<class T>
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

inline void flush() {
    fwrite(obuf, 1, oh - obuf, stdout);
}
}

namespace Task {

const int MAXN = 105;

struct Point {
    double x, y;

    Point(double x = 0, double y = 0) : x(x), y(y) {}

    inline Point operator+(const Point &p) const {
        return Point(x + p.x, y + p.y);
    }

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline double operator*(const Point &p) const {
        return x * p.y - y * p.x;
    }

    inline double operator^(const Point &p) const {
        return x * p.x + y * p.y;
    }

    inline Point operator*(const double t) const {
        return Point(x * t, y * t);
    }

    inline void read() {
        std::cin >> x >> y;
    }
} up[MAXN], down[MAXN];

template<typename T>
inline int sign(const T &x) {
    return x > 1e-8 ? 1 : (x < -1e-8 ? -1 : 0);
}

struct Line {
    Point s, t;

    Line() {}

    Line(const Point &s, const Point &t) : s(s), t(t) {}

    inline friend bool isIntersectLineSegment(const Line &l, const Line &seg) {
        return sign((l.t - l.s) * (seg.s - l.s)) * sign((l.t - l.s) * (seg.t - l.s)) <= 0;
    }

    inline Point intersect(const Line &l) const {
        return sign((t - s) * (l.t - l.s)) == 0 ? s :
               (s + (t - s) * (((l.t - s) * (l.s - s)) / ((l.t - l.s) * (t - s))));
    }
};

inline void solve() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    register int n;
    while (std::cin >> n && n) {
        for (register int i = 0; i < n; i++)
            up[i].read(), down[i] = up[i], down[i].y--;
        register bool flag = false;
        register double ans = -10000000.0;
        for (register int i = 0, k; i < n; i++) {
            for (register int j = i + 1; j < n; j++) {
                for (k = 0; k < n; k++)
                    if (!isIntersectLineSegment(Line(up[i], down[j]),
                                                Line(up[k], down[k]))) break;
                if (k >= n) {
                    flag = true;
                    break;
                }
                if (k > std::max(i, j)) {
                    if (isIntersectLineSegment(Line(up[i], down[j]),
                                               Line(up[k - 1], up[k]))) {
                        ans = std::max(ans, Line(up[i], down[j])
                                       .intersect(Line(up[k - 1], up[k])).x);
                    }
                    if (isIntersectLineSegment(Line(up[i], down[j]),
                                               Line(down[k - 1], down[k]))) {
                        ans = std::max(ans, Line(up[i], down[j])
                                       .intersect(Line(down[k - 1], down[k])).x);
                    }
                }

                for (k = 0; k < n; k++)
                    if (!isIntersectLineSegment(Line(down[i], up[j]),
                                                Line(up[k], down[k]))) break;
                if (k >= n) {
                    flag = true;
                    break;
                }
                if (k > std::max(i, j)) {
                    if (isIntersectLineSegment(Line(down[i], up[j]),
                                               Line(up[k - 1], up[k]))) {
                        ans = std::max(ans, Line(down[i], up[j])
                                       .intersect(Line(up[k - 1], up[k])).x);
                    }
                    if (isIntersectLineSegment(Line(down[i], up[j]),
                                               Line(down[k - 1], down[k]))) {
                        ans = std::max(ans, Line(down[i], up[j])
                                       .intersect(Line(down[k - 1], down[k])).x);
                    }
                }
            }
            if (flag) break;
        }
        if (flag) std::cout << "Through all the pipe.\n";
        else std::cout << std::fixed << std::setprecision(2) << ans << '\n';
    }
}
}

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    Task::solve();
    IO::flush();
    return 0;
}
