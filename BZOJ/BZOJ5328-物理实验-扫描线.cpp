/**
 * Copyright (c) 2018, xehoth
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * 「BZOJ 5328」物理实验 25-05-2018
 * 扫描线
 * @author xehoth
 */
#include <bits/stdc++.h>

struct InputStream {
    enum { SIZE = 1024 * 1024 };
    char ibuf[SIZE], *s, *t;

    inline char read() {
        if (s == t) t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin);
        return s == t ? -1 : *s++;
    }

    inline InputStream &operator>>(int &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        if (iosig) x = -x;
        return *this;
    }
} io;

const int MAXN = 10000;
const double EPS = 1e-9;

inline int sign(double x) { return (x > EPS) && (x < -EPS); }

struct Point {
    double x, y;

    Point() : x(), y() {}

    Point(double x, double y) : x(x), y(y) {}

    inline Point operator-(const Point &p) const { return Point(x - p.x, y - p.y); }

    inline Point &operator-=(const Point &p) {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    inline void read() {
        static int tx, ty;
        io >> tx >> ty;
        x = tx;
        y = ty;
    }

    inline double norm() const { return sqrt(x * x + y * y); }
};

int n, m;

struct Segment {
    Point s, t;

    inline void read() {
        s.read();
        t.read();
    }
} seg[MAXN + 1], track;

struct Line {
    Point s, t;
    int id;

    Line() {}

    Line(const Point &s, const Point &t, int id) : s(s), t(t), id(id) {}

    inline double f(double x) const { return ((x - s.x) * t.y + (t.x - x) * s.y) / (t.x - s.x); }

    inline double len() const {
        return sqrt(1 + ((t.y - s.y) * (t.y - s.y)) / ((t.x - s.x) * (t.x - s.x)));
    }

    inline bool operator<(const Line &b) const {
        double x = std::max(s.x, b.s.x);
        return std::abs(f(x)) < std::abs(b.f(x));
    }
} l[MAXN + 1];

int L;
std::pair<double, int> events[MAXN * 2 + 1];
double x[MAXN * 2 + 1], v[MAXN * 2 + 1], ans;
std::set<Line> d[2];

inline void solve() {
    io >> n;
    for (int i = 1; i <= n; i++) seg[i].read();
    track.read();
    io >> L;
    m = 0;
    ans = 0;
    d[0].clear();
    d[1].clear();
    Point tp = track.t - track.s;
    double len = tp.norm(), p, q;
    for (int i = 1; i <= n; i++) {
        seg[i].s -= track.s;
        p = (tp.x * seg[i].s.x + tp.y * seg[i].s.y) / len;
        q = (tp.x * seg[i].s.y - tp.y * seg[i].s.x) / len;
        seg[i].s = Point(p, q);
        seg[i].t -= track.s;
        p = (tp.x * seg[i].t.x + tp.y * seg[i].t.y) / len;
        q = (tp.x * seg[i].t.y - tp.y * seg[i].t.x) / len;
        seg[i].t = Point(p, q);
        if (seg[i].s.x > seg[i].t.x) std::swap(seg[i].s, seg[i].t);
        l[i] = Line(seg[i].s, seg[i].t, i);
        events[(i - 1) << 1] = std::make_pair(seg[i].s.x, i);
        events[(i - 1) << 1 | 1] = std::make_pair(seg[i].t.x, -i);
    }
    std::sort(events, events + n * 2);
    for (int i = 0; i < n * 2; i++) {
        if (!i || std::abs(events[i].first - x[m - 1]) > EPS) {
            x[m] = events[i].first;
            if (m) {
                v[m - 1] = 0;
                if (!d[0].empty()) v[m - 1] += d[0].begin()->len();
                if (!d[1].empty()) v[m - 1] += d[1].begin()->len();
            }
            m++;
        }
        if (events[i].second > 0) {
            int p = events[i].second;
            if (seg[p].s.y > 0)
                d[0].insert(l[p]);
            else
                d[1].insert(l[p]);
        } else {
            int p = -events[i].second;
            if (seg[p].s.y > 0)
                d[0].erase(l[p]);
            else
                d[1].erase(l[p]);
        }
    }
    double cur = 0;
    for (int i = 0, j = 0; i < m; i++) {
        for (; j + 1 < m && x[j + 1] < x[i] + L; j++) cur += (x[j + 1] - x[j]) * v[j];
        ans = std::max(ans, cur + (x[i] + L - x[j]) * (j == m - 1 ? 0 : v[j]));
        cur -= (x[i + 1] - x[i]) * v[i];
    }
    cur = 0;
    for (int i = m - 1, j = m - 1; i >= 0; i--) {
        for (; j && x[j - 1] > x[i] - L; j--) cur += (x[j] - x[j - 1]) * v[j - 1];
        ans = std::max(ans, cur + (x[j] + L - x[i]) * (!j ? 0 : v[j - 1]));
        if (i) cur -= (x[i] - x[i - 1]) * v[i - 1];
    }
    std::cout << std::fixed << std::setprecision(15) << ans << '\n';
}

int main() {
    int T;
    io >> T;
    while (T--) solve();
    return 0;
}