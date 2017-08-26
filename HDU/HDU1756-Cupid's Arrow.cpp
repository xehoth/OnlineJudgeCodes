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
 * 「HDU 1756」Cupid's Arrow 19-05-2017
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

inline void flush() {
    fwrite(obuf, 1, oh - obuf, stdout);
}
}

namespace Task {

const int MAXN = 105;

struct Point {
    int x, y;

    Point(int x = 0, int y = 0) : x(x), y(y) {}

    inline Point operator+(const Point &p) const {
        return Point(x + p.x, y + p.y);
    }

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline int operator*(const Point &p) const {
        return x * p.y - y * p.x;
    }

    inline int operator^(const Point &p) const {
        return x * p.x + y * p.y;
    }
    
    inline void read() {
        IO::read(x), IO::read(y);
    }
} q;

inline bool isPointOnSegment(const Point &a, const Point &b, const Point &p) {
    return (a - p) * (b - p) == 0 && std::min(a.x, b.x) <= p.x &&
           std::min(a.y, b.y) <= p.y && std::max(a.x, b.x) >= p.x &&
           std::max(a.y, b.y) >= p.y;
}

struct Polygon {
    Point p[MAXN];
    int n;

    Polygon(int n = 0) : n(n) {}

    inline int area() {
        register int ret = 0;
        for (register int i = 0; i < n; i++) ret += p[i] * p[i + 1];
        return ret / 2;
    }

    inline void fix() {
        p[n] = p[0];
        area() < 0 ? std::reverse(p, p + n) : (void)0;
        p[n] = p[0];
    }

    inline bool contains(const Point &q) {
        register int cnt = 0;
        for (register int i = 0; i < n; i++) {
            if (isPointOnSegment(p[i], p[i + 1], q)) return true;
            register int det = (p[i] - q) * (p[i + 1] - q);
            register int d1 = p[i].y - q.y, d2 = p[i + 1].y - q.y;
            if ((det >= 0 && d1 < 0 && d2 >= 0) ||
                (det <= 0 && d1 >= 0 && d2 < 0)) cnt++;
        }
        return cnt & 1;
    }

    inline Point &operator[](const int i) {
        return p[i];
    }
} p;

inline void solve() {
    using namespace IO;
    register int n, m;
    while (read(n)) {
        p.n = n;
        for (register int i = 0; i < n; i++) p[i].read();
        p.fix();
        read(m);
        while (m--) {
            q.read();
            p.contains(q) ? (print('Y'), print('e'), print('s')) : 
            (print('N'), print('o'));
            print('\n');
        }
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
