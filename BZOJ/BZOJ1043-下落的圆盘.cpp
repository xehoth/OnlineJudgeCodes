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
 * 「HAOI2008」下落的圆盘 22-05-2017
 * @author xehoth
 */
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
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = '\0';
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
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

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace IO

namespace Task {

const int MAXN = 1005;
const double PI = acos(-1);

struct Point {
    double x, y;

    Point(double x = 0, double y = 0) : x(x), y(y) {}

    inline double angle() const { return atan2(y, x); }

    inline double dis(const Point &b) const { return hypot(x - b.x, y - b.y); }

    inline Point operator+(const Point &p) const {
        return Point(x + p.x, y + p.y);
    }

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline void read() { IO::read(x), IO::read(y); }
};

struct Arc {
    double l, r;

    Arc(double l = 0, double r = 0) : l(l), r(r) {}

    inline bool operator<(const Arc &b) const { return l < b.l; }
};

struct Circle {
    double r;
    Point o;

    Circle() {}

    Circle(const Point &o, const double r) : o(o), r(r) {}

    inline bool contains(const Circle &b) const {
        return r - b.r >= o.dis(b.o);
    }

    inline Arc intersect(const Circle &b) const {
        register double dis = o.dis(b.o);
        register double th =
            acos((r * r - b.r * b.r + dis * dis) / (2 * r * dis));
        register double ath = (b.o - o).angle();
        return Arc(ath - th, ath + th);
    }

    inline void read() { IO::read(r), o.read(); }
} circle[MAXN];

inline void solve() {
    using namespace IO;
    register int n;
    read(n);
    for (register int i = 0; i < n; i++) circle[i].read();
    register double ans = 0;
    for (register int i = 0; i < n; i++) {
        register int cnt = 0;
        static Arc a[MAXN];
        register bool flag = false;
        for (register int j = i + 1; j < n; j++) {
            if (circle[j].contains(circle[i])) {
                flag = true;
                break;
            }
            if (!circle[i].contains(circle[j]) &&
                circle[i].r + circle[j].r > circle[i].o.dis(circle[j].o))
                a[cnt++] = circle[i].intersect(circle[j]);
        }
        if (flag) continue;
        for (register int i = 0; i < cnt; i++) {
            if (a[i].l < 0) a[i].l += 2 * PI;
            if (a[i].r < 0) a[i].r += 2 * PI;
            if (a[i].l > a[i].r) a[cnt++] = Arc(0, a[i].r), a[i].r = 2 * PI;
        }
        std::sort(a, a + cnt);
        register double tmp = 0, curR = 0;
        for (register int i = 0; i < cnt; i++) {
            if (curR < a[i].l)
                tmp += a[i].l - curR, curR = a[i].r;
            else
                curR = std::max(curR, a[i].r);
        }
        ans += (tmp + 2 * PI - curR) * circle[i].r;
    }
    printf("%.3lf\n", ans);
}
}  // namespace Task

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    Task::solve();
    return 0;
}