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
 * 「HAOI2006」均分数据 23-05-2017
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

const int MAXN = 25;
const double EPS = 0.1;
const double DROP = 0.99;

double a[MAXN], sum[MAXN], min = DBL_MAX, average;
int n, m;
double *pos[MAXN];

template<typename T>
inline T square(const T &x) {
    return x * x;
}

inline double check(double tmp, double **o, double *p) {
    tmp -= square(**o - average) + square(*p - average);
    **o -= a[o - pos], *p += a[o - pos];
    tmp += square(**o - average) + square(*p - average);
    return tmp;
}

inline void simulatedAnnealing() {
    memset(sum, 0, sizeof(double) * (m + 1));
    register double now = 0;
    for (register int i = 1; i <= n; i++)
        pos[i] = sum + rand() % m + 1, *pos[i] += a[i];
    for (register int i = 1; i <= m; i++)
        now += square(sum[i] - average);
    for (double t = 2000; t > EPS; t *= DROP) {
        register double *p, **o = pos + rand() % n + 1;
        if (t > 500) p = std::min_element(sum + 1, sum + m + 1);
        else p = sum + rand() % m + 1;
        if (*o == p) continue;
        register double tmp = check(now, o, p);
        register double delta = now - tmp;
        if (delta >= 0 || exp(delta / t) * RAND_MAX >= rand()) {
            now = tmp, *o = p;
        } else {
            **o += a[o - pos], *p -= a[o - pos];
        }
        min = std::min(min, now);
    }
}

inline void solve() {
    using namespace IO;
    srand(495);
    read(n), read(m);
    for (register int i = 1, t; i <= n; i++)
        read(t), average += (a[i] = t);
    std::random_shuffle(a + 1, a + n + 1);
    average /= m;
    for (register int i = 0; i < 1000; i++)
        simulatedAnnealing();
    printf("%.2f", sqrt(min / m));
}
}

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    SimulatedAnnealing::solve();
    return 0;
}