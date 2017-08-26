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
#include <cstdio>
#include <algorithm>
#include <vector>
#include <deque>
#include <cstring>
#include <iostream>
#include <cmath>
/**
 * 「POJ3335」Rotating Scoreboard 22-05-2017
 * 半平面交
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
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read())
        x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
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

namespace HalfPlaneIntersection {

const int MAXN = 110;

const double EPS = 1e-8;

struct Point {
    double x, y;

    Point(double x = 0, double y = 0) : x(x), y(y) {}

    inline Point operator+(const Point &p) const {
        return Point(x + p.x, y + p.y);
    }

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y- p.y);
    }

    inline double operator*(const Point &p) const {
        return x * p.y - y * p.x;
    }

    inline Point operator*(const double i) const {
        return Point(x * i, y * i);
    }

    inline double operator^(const Point &p) const {
        return x * p.x + y * p.y;
    }
    
    inline void read() {
        static int t;
        IO::read(t), x = t, IO::read(t), y = t;
    }
} p[MAXN];

struct Line {
    Point s, t;
    double arg;
    Line(const Point &s, const Point &b) : s(s), t(b - s), arg(atan2(t.y, t.x)) {}

    Line() {}

    inline friend bool onLeft(const Point &p, const Line &l) {
        return l.t * (p - l.s) > -EPS;
    }

    inline bool operator<(const Line &l) const {
        return std::abs(arg - l.arg) < EPS ? onLeft(s, l) : arg < l.arg;
    }

    inline Point intersect(const Line &l) const {
        return s + t * (((s - l.s) * l.t) / (l.t * t));
    }
} l[MAXN];

typedef std::pair<Line, Point> Pair;
typedef std::deque<Pair> Deque; 

inline Deque &halfPlaneIntersection(Line *l, const int n) {
    static Deque q;
    q.clear(), std::sort(l, l + n);
    q.push_back(Pair(l[0], Point()));
    for (register int i = 1; i < n; i++) {
        if (std::abs(l[i].arg - l[i - 1].arg) < EPS) continue;
        while (q.size() > 1 && !onLeft(q.back().second, l[i])) q.pop_back();
        while (q.size() > 1 && !onLeft(q[1].second, l[i])) q.pop_front();
        q.push_back(Pair(l[i], l[i].intersect(q.back().first)));
    }
    while (q.size() > 1 && !onLeft(q.back().second, q.front().first)) q.pop_back();
    q.front().second = q.front().first.intersect(q.back().first);
    return q;
}

inline void solve() {
    using namespace IO;
    register int T, n;
    read(T);
    while (T--) {
        read(n);
        for (register int i = n; i; i--) p[i].read();
        for (register int i = 1; i < n; i++) l[i - 1] = Line(p[i], p[i + 1]);
        l[n - 1] = Line(p[n], p[1]);
        const Deque &q = halfPlaneIntersection(l, n);
        q.size() <= 2 ? print("NO\n") : print("YES\n"); 
    }
}
}

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    HalfPlaneIntersection::solve();
    IO::flush();
    return 0;
}
