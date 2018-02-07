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
 * 「BZOJ 1185」22-05-2017
 *
 * @author xehoth
 */
namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
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
    static char buf[20];
    read(buf), x = atof(buf);
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
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace IO

namespace Task {

const int MAXN = 51000;
const double EPS = 1e-10;

template <typename T>
inline T square(const T &x) {
    return x * x;
}

struct Point {
    double x, y;

    Point(double x = 0, double y = 0) : x(x), y(y) {}

    inline Point operator+(const Point &p) const {
        return Point(x + p.x, y + p.y);
    }

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline double operator*(const Point &p) const { return x * p.y - y * p.x; }

    inline double operator^(const Point &p) const { return x * p.x + y * p.y; }

    inline Point operator*(const double i) const { return Point(x * i, y * i); }

    inline bool operator<(const Point &p) const {
        return x < p.x || (std::abs(x - p.x) < EPS && y < p.y);
    }

    inline double dis(const Point &p) const { return hypot(x - p.x, y - p.y); }

    inline void read() { IO::read(x), IO::read(y); }
} ans[10], p[MAXN], con[MAXN];

inline int andrew(Point *p, const int n, Point *con) {
    std::sort(p, p + n);
    register int top = 0, k;
    for (register int i = 0; i < n; i++) {
        while (top > 1 &&
               (con[top - 1] - con[top - 2]) * (p[i] - con[top - 2]) < EPS)
            top--;
        con[top++] = p[i];
    }
    k = top;
    for (register int i = n - 2; i >= 0; i--) {
        while (top > k &&
               (con[top - 1] - con[top - 2]) * (p[i] - con[top - 2]) < EPS)
            top--;
        con[top++] = p[i];
    }
    return n > 1 ? --top : top;
}

inline double rotatingCalispers(Point *p, const int n) {
    register double dis, height, width, min = DBL_MAX, now;
    for (register int i = 0, up = 1, l = 1, r = 1; i < n; i++) {
        while ((p[i + 1] - p[i]) * (p[up] - p[i]) <
               (p[i + 1] - p[i]) * (p[up + 1] - p[i]) + EPS)
            ++up == n ? up = 0 : 0;
        while (((p[i + 1] - p[i]) ^ (p[r] - p[i])) <
               ((p[i + 1] - p[i]) ^ (p[r + 1] - p[i])) + EPS)
            ++r == n ? r = 0 : 0;
        if (i == 0) l = r;
        while (((p[i + 1] - p[i]) ^ (p[l] - p[i])) >
               ((p[i + 1] - p[i]) ^ (p[l + 1] - p[i])) - EPS)
            ++l == n ? l = 0 : 0;
        // std::cerr << l << " " << r << " " << up << std::endl;
        dis = p[i].dis(p[i + 1]);
        height = (p[up] - p[i + 1]) * (p[i] - p[i + 1]) / dis;
        width = std::abs(((p[i + 1] - p[i]) ^ (p[l] - p[i])) / dis) +
                std::abs(((p[i + 1] - p[i]) ^ (p[r] - p[i])) / dis);
        now = width * height;
        // std::cerr << dis << " " << height << " " << width << " " << now <<
        // std::endl;
        if (now < min) {
            min = now;
            ans[0] =
                p[i] +
                (p[i + 1] - p[i]) *
                    ((std::abs((p[i + 1] - p[i]) ^ (p[r] - p[i + 1])) / dis +
                      dis) /
                     dis);
            ans[1] = ans[0] + (p[r] - ans[0]) * (height / p[r].dis(ans[0]));
            ans[2] = ans[1] + (p[up] - ans[1]) * (width / p[up].dis(ans[1]));
            ans[3] = ans[2] + (p[l] - ans[2]) * (height / p[l].dis(ans[2]));
        }
        /*for (register int i = 0; i < 4; i++) std::cerr << ans[i].x << " " <<
        ans[i].y << std::endl; std::cerr << std::endl;*/
        // exit(0);
    }
    return min;
}

inline bool cmp(const Point &a, const Point &b) {
    return a.y < b.y || (std::abs(a.y - b.y) < EPS && a.x < b.x);
}

inline void solve() {
    using namespace IO;
    register int n;
    read(n);
    for (register int i = 0; i < n; i++) p[i].read();
    printf("%.5lf\n", rotatingCalispers(con, andrew(p, n, con)));
    register int tmp = std::min_element(ans, ans + 4, cmp) - ans;
    for (register int i = 0; i < 4; i++)
        printf("%.5lf %.5lf\n", ans[i + tmp & 3].x, ans[i + tmp & 3].y);
}
}  // namespace Task

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    Task::solve();
    return 0;
}
