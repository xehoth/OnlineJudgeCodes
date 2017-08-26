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
#include <bits/stdc++.h>
/**
 * 「BZOJ1337」最小圆覆盖 23-05-2017
 * 随机增量法
 * @author xehoth
 */
namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template<typename T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        iosig ? x = -x : 0;
    }
    for (x = 0; isdigit(c); c = read())
        x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1);
    if (c == -1) {
        *buf = '\0';
        return -1;
    }
    do buf[s++] = c; while (c = read(), !isspace(c) && c != -1);
    buf[s] = '\0';
    return s;
}

inline void read(double &x) {
    static char buf[30];
    read(buf), x = atof(buf);
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template<typename T>
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

inline void flush() {
    fwrite(obuf, 1, oh - obuf, stdout);
}
}

namespace Task {

const int MAXN = 100005;
const double EPS = 1e-8;

template<typename T>
inline T square(const T &x) {
    return x * x;
}

struct Point {
    double x, y;

    Point(double x = 0, double y = 0) : x(x), y(y) {}

    inline double dis(const Point &p) const {
        return sqrt(square(x - p.x) + square(y - p.y));
    }

    inline Point operator+(const Point &p) const {
        return Point(x + p.x, y + p.y);
    }

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline Point operator*(const double i) const {
        return Point(x * i, y * i);
    }

    inline double operator*(const Point &p) const {
        return x * p.y - y * p.x;
    }

    inline double operator^(const Point &p) const {
        return x * p.x + y * p.y;
    }

    inline Point operator/(const double i) const {
        return Point(x / i, y / i);
    }

    inline void read() {
        IO::read(x), IO::read(y);
    }
} p[MAXN];

struct Circle {
    Point o;
    double r;

    Circle(const Point &o, const double r) : o(o), r(r) {}

    Circle() {}
};

struct Line {
    Point s, t;

    Line(const Point &s, const Point &t) : s(s), t(t) {}

    inline Point intersect(const Line &l) const {
        return s + (t - s) * (((s - l.s) * (l.t - l.s)) / ((l.t - l.s) * (t - s)));
    }

    inline Line getPerpendicularBisector() {
        return Line(Point((s.x + t.x) / 2, (s.y + t.y) / 2), 
                    Point((s.x + t.x) / 2 + s.y - t.y, (s.y + t.y) / 2 + t.x - s.x));
    }
};

inline Point getCenter(const Point &a, const Point &b, const Point &c) {
    return Line(a, b).getPerpendicularBisector().
           intersect(Line(a, c).getPerpendicularBisector());
}

inline void solve() {
    using namespace IO;
    srand(495);
    register int n;
    read(n);
    for (register int i = 1; i <= n; i++) p[i].read();
    std::random_shuffle(p + 1, p + n + 1);
    Circle c(p[1], 0);
    for (register int i = 1; i <= n; i++) {
        if (c.o.dis(p[i]) - c.r < EPS) continue;
        c = Circle((p[1] + p[i]) / 2, p[1].dis(p[i]) / 2);
        for (register int j = 2; j < i; j++) {
            if (c.o.dis(p[j]) - c.r < EPS) continue;
            c = Circle((p[i] + p[j]) / 2, p[i].dis(p[j]) / 2);
            for (register int k = 1; k < j; k++) {
                if (c.o.dis(p[k]) - c.r < EPS) continue;
                c = Circle(getCenter(p[i], p[j], p[k]), c.r);
                c.r = c.o.dis(p[k]);
            }
        }
    }
    printf("%.3lf\n", c.r);
}
}

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    Task::solve();
    return 0;
}
