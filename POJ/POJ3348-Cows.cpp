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
 * 「POJ 3348」Cows 19-05-2017
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

const int MAXN = 10005;

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

    inline bool operator<(const Point &p) const {
        return x < p.x || (fabs(x - p.x) < 1e-8 && y < p.y);
    }

    inline void read() {
        std::cin >> x >> y;
    }
} p[MAXN], con[MAXN];

inline int andrew(Point *p, const int n, Point *con) {
    std::sort(p, p + n);
    register int top = 0, k;
    for (register int i = 0; i < n; i++) {
        while (top > 1 && (con[top - 1] - con[top - 2]) * (p[i] - con[top - 2]) <= 0) top--;
        con[top++] = p[i];
    }
    k = top;
    for (register int i = n - 2; i >= 0; i--) {
        while (top > k && (con[top - 1] - con[top - 2]) * (p[i] - con[top - 2]) <= 0) top--;
        con[top++] = p[i];
    }
    return n > 1 ? --top : top; 
}

inline double area(Point *p, const int n) {
    register double ret = 0;
    p[n] = p[0];
    for (register int i = 0; i < n; i++) ret += p[i] * p[i + 1];
    return fabs(ret) / 2;
}

inline void solve() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    register int n;
    std::cin >> n;
    for (register int i = 0; i < n; i++) p[i].read();
    std::cout << (int)(area(con, andrew(p, n, con)) / 50);
}
}

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    Task::solve();
    return 0;
}
