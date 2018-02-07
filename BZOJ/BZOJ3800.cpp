#include <bits/stdc++.h>

inline int read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *h, *t;
    if (h == t) {
        t = (h = buf) + fread(buf, 1, IN_LEN, stdin);
        if (h == t) return -1;
    }
    return *h++;
}
template <class T>
inline bool read(T &x) {
    static bool iosig = 0;
    static char c;
    for (iosig = 0, c = read(); !isdigit(c); c = read()) {
        if (c == -1) return false;
        if (c == '-') iosig = 1;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    if (iosig) x = -x;
    return true;
}

const int OUT_LEN = 10000000;
char obuf[OUT_LEN], *oh = obuf;
inline void print(const char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}

template <class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (!x)
        print('0');
    else {
        if (x < 0) print('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print((char)buf[cnt--]);
    }
}
inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

const double EPS = 1e-8;
const int S = 10000;

template <class T>
inline int sign(const T &x) {
    return x > EPS ? 1 : (x < -EPS ? -1 : 0);
}

struct Point {
    double x, y;

    Point(double x = 0.0, double y = 0.0) : x(x), y(y) {}

    inline bool operator<(const Point &p) const {
        return x == p.x ? y < p.y : x < p.x;
    }

    inline Point operator+(const Point &p) const {
        return Point(x + p.x, y + p.y);
    }

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline Point operator*(double d) const { return Point(x * d, y * d); }

    inline friend double dot(const Point &a, const Point &b) {
        return a.x * b.x + a.y * b.y;
    }

    inline friend double cross(const Point &a, const Point &b) {
        return a.x * b.y - a.y * b.x;
    }

    inline friend Point rotate(const Point &p, double a) {
        return Point(p.x * cos(a) - p.y * sin(a), p.x * sin(a) + p.y * cos(a));
    }

    inline void read() {
        register int i;
        ::read(i), x = i, ::read(i), y = i;
    }
};

struct Line {
    Point p, v;
    double t;

    Line() {}

    Line(const Point &p, const Point &v) : p(p), v(v) { t = atan2(v.y, v.x); }

    inline bool operator<(const Line &l) const {
        return sign(t - l.t) == 0 ? onleft(p, l) : t < l.t;
    }

    inline friend bool onleft(const Point &p, const Line &l) {
        return cross(l.v, p - l.p) > -EPS;
    }

    inline friend Point intersection(const Line &a, const Line &b) {
        return a.p + a.v * (cross(b.v, a.p - b.p) / cross(a.v, b.v));
    }
};

typedef std::pair<Line, Point> Pair;
typedef std::deque<Pair> Deque;

inline Deque halfPlaneIntersection(const int n, Line *l) {
    std::sort(l, l + n);
    Deque q;
    q.push_back(Pair(l[0], Point()));
    for (register int i = 1; i < n; i++) {
        if (!sign(l[i].t - l[i - 1].t)) continue;
        while (q.size() > 1 && !onleft(q.back().second, l[i])) q.pop_back();
        while (q.size() > 1 && !onleft(q[1].second, l[i])) q.pop_front();
        q.push_back(Pair(l[i], intersection(l[i], q.back().first)));
    }
    while (q.size() > 1 && !onleft(q.back().second, q.front().first))
        q.pop_back();
    q.front().second = intersection(q.front().first, q.back().first);
    return q;
}

int n;
double v[105], u[105], w[105];
Line l[105];

int main() {
    // freopen("in.in", "r", stdin);
    read(n);
    for (register int i = 1, t; i <= n; i++)
        read(t), v[i] = t, read(t), u[i] = t, read(t), w[i] = t;
    for (register int i = 1; i <= n; i++) {
        register bool flag = true;
        register int tot = 0;
        for (register int j = 1; j <= n; j++) {
            if (i == j) continue;
            if (v[i] <= v[j] && u[i] <= u[j] && w[i] <= w[j]) {
                flag = false;
                break;
            }
            if (v[i] >= v[j] && u[i] >= u[j] && w[i] >= w[j]) continue;
            double a = (S / v[j] - S / w[j]) - (S / v[i] - S / w[i]),
                   b = (S / u[j] - S / w[j]) - (S / u[i] - S / w[i]),
                   c = S / w[j] - S / w[i];
            l[++tot] =
                Line(fabs(a) > fabs(b) ? Point(-c / a, 0) : Point(0, -c / b),
                     Point(b, -a));
        }
        if (flag) {
            l[++tot] = Line(Point(0, 0), Point(0, -1));
            l[++tot] = Line(Point(0, 0), Point(1, 0));
            l[++tot] = Line(Point(0, 1), Point(-1, 1));
            if (halfPlaneIntersection(tot, l + 1).size() <= 2) flag = false;
        }
        flag ? (print('Y'), print('e'), print('s')) : (print('N'), print('o'));
        print('\n');
    }
    flush();
    return 0;
}
