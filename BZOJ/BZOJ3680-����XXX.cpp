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
 * 「BZOJ3680」吊打XXX 23-05-2017
 * 模拟退火
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
        x < 0 ? (print('0'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() {
    fwrite(obuf, 1, oh - obuf, stdout);
}
}

namespace SimulatedAnnealing {

const int MAXN = 100000;

const double PI2 = 2 * acos(-1);
const double EPS = 1e-3;
const double DROP = 0.97;

struct Point {
    double x, y, w;
    Point(double x = 0, double y = 0, double w = 0) : x(x), y(y), w(w) {}

    inline void operator+=(const Point &p) {
        x += p.x, y += p.y;
    }

    inline void operator/=(const int i) {
        x /= i, y /= i;
    }

    inline double dis(const Point &p) const {
        return hypot(x - p.x, y - p.y);
    }

    inline void read() {
        IO::read(x), IO::read(y), IO::read(w);
    }
} now, ans, p[MAXN];

double total = DBL_MAX;
int n;
 
inline double check(const Point &p) {
    register double ret = 0;
    for (register int i = 0; i < n; i++) 
        ret += p.dis(SimulatedAnnealing::p[i]) * SimulatedAnnealing::p[i].w;
    if (ret < total) total = ret, ans = p;
    return ret;
}

inline void solve() {
    using namespace IO;
    srand(495);
    read(n);
    for (register int i = 0; i < n; i++) p[i].read(), now += p[i];
    now /= n;
    for (double t = 100000; t > EPS; t *= DROP) {
        Point tmp(now.x + t * sin(rand()) , now.y + t * sin(rand()));
        register double delta = check(now) - check(tmp);
        if (delta >= 0 || exp(delta / t) * RAND_MAX >= rand()) now = tmp;
    }
    for (register int i = 1; i <= 1000; i++) {
        check(Point(ans.x + 0.001 * cos(rand()), ans.y + 0.001 * sin(rand())));
    }
    printf("%.3lf %.3lf\n", ans.x, ans.y);
}
}

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    SimulatedAnnealing::solve();
    
    return 0;
}
