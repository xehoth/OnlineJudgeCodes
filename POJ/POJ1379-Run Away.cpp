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
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <climits>
#include <cfloat>
/**
 * 「POJ1379」Run Away 23-05-2017
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

namespace SimulatedAnnealing {

const int MAXN = 1005;
const int MAXP = 20;
const double EPS = 1e-2;
const double DROP = 0.9;

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

    inline void read() {
        static int t;
        IO::read(t), x = t, IO::read(t), y = t;
    }
} a[MAXN], ans[MAXN];

double x, y;
int n;

inline double get(const Point &p) {
    register double ret = DBL_MAX;
    for (register int i = 0; i < n; i++)
        ret = std::min(ret, p.dis(a[i]));
    return ret;
}

inline void simulatedAnnealing() {
    static double dis[MAXP + 5];
    for (register int i = 0; i < MAXP; i++)
        ans[i] = Point((rand() + 1.0) / RAND_MAX, (rand() + 1.0) / RAND_MAX), 
        dis[i] = get(ans[i]);
    for (double t = std::max(x, y) / sqrt((double)n); t > EPS; t *= DROP) {
        for (register int i = 0; i < MAXP; i++) {
            for (register int j = 0; j < MAXP; j++) {
                Point tmp = ans[i];
                tmp.x += t * cos((double)rand()), tmp.y += t * sin((double)rand());
                if (tmp.x < 0 || tmp.x > x || tmp.y < 0 || tmp.y > y) continue;
                register double now = get(tmp);
                register double delta = now - dis[i];
                if (delta >= 0 || (t > 500 && exp(delta / t) * RAND_MAX >= rand()))
                    dis[i] = now, ans[i] = tmp; 
            }
        }
    }
    for (register int times = 0; times < 100; times++) {
        for (register int i = 0; i < MAXP; i++) {
            for (register int j = 0; j < MAXP; j++) {
                Point tmp = ans[i];
                tmp.x += EPS * cos((double)rand()), tmp.y += EPS * sin((double)rand());
                if (tmp.x < 0 || tmp.x > x || tmp.y < 0 || tmp.y > y) continue;
                register double now = get(tmp);
                register double delta = now - dis[i];
                if (delta >= 0)
                    dis[i] = now, ans[i] = tmp;
            }
        }
    }
    Point *cur = std::max_element(dis, dis + MAXP) - dis + ans;
    printf("The safest point is (%.1f, %.1f).\n", cur->x, cur->y);
}

inline void solve() {
    using namespace IO;
    srand(495);
    register int t, tmp;
    for (read(t); t--;) {
        read(tmp), x = tmp, read(tmp), y = tmp, read(n);
        for (register int i = 0; i < n; i++) a[i].read();
            simulatedAnnealing();
    }
}
}

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    SimulatedAnnealing::solve();
    return 0;
}
