/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1249」动态凸包 15-09-2017
 * 动态凸包
 * @author xehoth
 */
#include <bits/stdc++.h>

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
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
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
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }

    ~InputOutputStream() { flush(); }
} io;
}

namespace {

using IO::io;

#define long long long

struct Point {
    int x, y;

    Point(int x = 0, int y = 0) : x(x), y(y) {}

    inline bool operator<(const Point &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }

    inline bool operator==(const Point &p) const {
        return x == p.x && y == p.y;
    }

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline long operator*(const Point &p) const {
        return (long)x * p.y - (long)y * p.x;
    }

    inline void reverse() { x = -x, y = -y; }

    inline void read() { IO::read(x), IO::read(y); }
};

struct Task {
    Point x, y, z;
    typedef std::set<Point> Set;
    Set top, bottom;
    Set::iterator l, r, it;
    long ans;

    inline void eraseL(std::set<Point> &set) {
        while (l != set.begin() && (*--(it = l) - *l) * (x - *l) >= 0)
            ans -= *it * *l, set.erase(l), l = it;
    }

    inline void eraseR(std::set<Point> &set) {
        while (++(it = r) != set.end() && (x - *r) * (*it - *r) >= 0)
            ans -= *r * *it, set.erase(r), r = it;
    }

    inline void insert(const Point &x, Set &set) {
        if ((r = set.lower_bound(x)) == set.end()) {
            --(l = r), eraseL(set);
            set.insert(set.end(), x), ans += *l * x;
            return;
        }
        if (x == *r) return;
        if (r != set.begin()) {
            if ((*--(l = r) - x) * (*r - x) >= 0) return;
            ans -= *l * *r, eraseL(set), ans += *l * x;
        }
        eraseR(set), ans += x * *r, set.insert(r, x);
    }

    inline void insert(Point &x) {
        insert(x, bottom), x.reverse(), insert(x, top);
    }

    inline void solve() {
        x.read(), y.read(), z.read();
        bottom.insert(y), bottom.insert(z);
        y.reverse(), z.reverse();
        top.insert(y), top.insert(z);
        insert(x);
        register int n;
        io >> n;
        while (n--) {
            x.read(), insert(x);
            io << ans << '\n';
        }
    }
} task;

#undef long
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    task.solve();
    return 0;
}