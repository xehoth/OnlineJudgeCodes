/**
 * Copyright (c) 2016-2018, xehoth
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
 * 「BZOJ 5317」部落战争 19-05-2018
 * MinkowskiAddition
 * @author xehoth
 */
#include <bits/stdc++.h>

#ifdef _DEBUG
struct Debug {
    std::chrono::steady_clock::time_point start, end;

    Debug() { start = std::chrono::steady_clock::now(); }

    ~Debug() {
        end = std::chrono::steady_clock::now();
        auto duration =
            std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        double count = (double)duration.count() *
                       std::chrono::microseconds::period::num /
                       std::chrono::microseconds::period::den;
        std::cerr << std::endl
                  << "time: " << std::fixed << std::setprecision(3)
                  << count * 1000 << "ms" << std::endl;
    }
} debug;
#endif

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };
    char ibuf[SIZE], *s, *t, obuf[SIZE], *oh;

    InputOutputStream() : s(), t(), oh(obuf) {}
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    inline char read() {
        if (s == t) t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin);
        return s == t ? -1 : *s++;
    }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
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

    inline void print(char c) {
        if (oh == obuf + SIZE) {
            fwrite(obuf, 1, SIZE, stdout);
            oh = obuf;
        }
        *oh++ = c;
    }

    template <typename T>
    inline void print(T x) {
        static int buf[21], cnt;
        if (x != 0) {
            if (x < 0) {
                print('-');
                x = -x;
            }
            for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
            while (cnt) print((char)buf[cnt--]);
        } else {
            print('0');
        }
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

struct Point {
    int x, y;
    double arg;
    int id;

    Point(int x, int y) : x(x), y(y) {}
    Point(int x, int y, int id) : x(x), y(y), id(id) { arg = atan2(y, x); }
    Point() {}

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline Point operator+(const Point &p) const {
        return Point(x + p.x, y + p.y);
    }

    inline long long operator*(const Point &p) const {
        return (long long)x * p.y - (long long)y * p.x;
    }

    inline bool operator<(const Point &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }

    inline void reverse() {
        x = -x;
        y = -y;
    }

    inline bool operator==(const Point &p) const {
        return x == p.x && y == p.y;
    }
};

const double EPS = 1e-6;
const int MAXN = 100000;

int n, m, q;
std::vector<Point> a, b, que;
bool ans[MAXN + 1];

inline std::vector<Point> convexHull(std::vector<Point> &p) {
    static std::vector<Point> c;
    c.clear();
    c.resize(2 * p.size());
    std::sort(p.begin(), p.end());
    int top = 0, k;
    for (int i = 0; i < (int)p.size(); i++) {
        while (top > 1 && (c[top - 1] - c[top - 2]) * (p[i] - c[top - 2]) <= 0)
            top--;
        c[top++] = p[i];
    }
    k = top;
    for (int i = (int)p.size() - 2; i >= 0; i--) {
        while (top > k && (c[top - 1] - c[top - 2]) * (p[i] - c[top - 2]) <= 0)
            top--;
        c[top++] = p[i];
    }
    c.resize((int)p.size() > 1 ? top - 1 : top);
    return c;
}

inline std::vector<Point> getMinkowskiAddition(const std::vector<Point> &a,
                                               const std::vector<Point> &b) {
    static std::vector<Point> ret;
    ret.clear();
    int n = a.size(), m = b.size(), ai = 0, bi = 0, ra = 0, rb = 0;
    while (ai < n || bi < m) {
        ret.push_back(a[ra] + b[rb]);
        Point nextA = a[(ra + 1 >= n ? ra + 1 - n : ra + 1)] - a[ra];
        Point nextB = b[(rb + 1 >= m ? rb + 1 - m : rb + 1)] - b[rb];
        if (ai < n && (nextA * nextB >= 0 || bi == m)) {
            ai++;
            if (++ra == n) ra = 0;
        } else {
            bi++;
            if (++rb == m) rb = 0;
        }
    }
    return ret;
}

inline bool cmp(const Point &a, const Point &b) {
    return std::abs(a.arg - b.arg) > EPS ? a.arg < b.arg : a * b > 0;
}

inline void query(const std::vector<Point> &a, const std::vector<Point> &b) {
    static std::vector<Point> v;
    static int id[MAXN + 1];
    int n = a.size(), m = b.size();
    const Point &O = a[0];
    for (register int i = 1; i < n; i++)
        v.push_back(Point(a[i].x - O.x, a[i].y - O.y, i));
    for (register int i = 0; i < m; i++) {
        if (b[i] == O) {
            ans[b[i].id] = 1;
            continue;
        }
        id[i] = b[i].id;
        v.push_back(Point(b[i].x - O.x, b[i].y - O.y, i + n));
    }
    std::sort(v.begin(), v.end(), cmp);
    Point p = a[0] - O, q = a[1] - O;
    for (register int i = 0; i < (int)v.size(); i++) {
        if (v[i].id < n) {
            p = a[v[i].id] - O;
            q = a[(v[i].id + 1) % n] - O;
        } else {
            ans[id[v[i].id - n]] = ((q - p) * (v[i] - p)) >= 0;
        }
    }
}

int main() {
    io >> n >> m >> q;
    a.resize(n);
    for (int i = 0; i < n; i++) io >> a[i].x >> a[i].y;
    a = convexHull(a);
    b.resize(m);
    for (int i = 0; i < m; i++) {
        io >> b[i].x >> b[i].y;
        b[i].reverse();
    }
    b = convexHull(b);
    a = getMinkowskiAddition(a, b);
    a = convexHull(a);
    que.resize(q);
    for (int i = 0; i < q; i++) {
        io >> que[i].x >> que[i].y;
        que[i].id = i + 1;
    }
    query(a, que);
    for (int i = 1; i <= q; i++) io << (int)ans[i] << '\n';
    return 0;
}