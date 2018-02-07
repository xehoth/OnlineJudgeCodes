#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <class T>
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
}  // namespace IO

const double PI = acos(-1.0);
const double EPS = 1e-12;
const int MAXN = 6020;

inline int cmp(double x) { return fabs(x) < EPS ? 0 : (x < 0 ? -1 : 1); }

template <class T>
inline int sign(T x) {
    return x == 0 ? 0 : (x < 0 ? -1 : 1);
}

struct Point {
    double x, y, ang;
    Point(double x = 0, double y = 0) : x(x), y(y) {}

    inline Point operator-(const Point &b) const {
        return Point(x - b.x, y - b.y);
    }

    inline Point operator+(const Point &b) const {
        return Point(x + b.x, y + b.y);
    }

    inline Point operator*(const double &k) const {
        return Point(x * k, y * k);
    }

    inline bool operator<(const Point &b) const { return ang < b.ang; }

    inline bool operator==(const Point &b) const {
        return x == b.x && y == b.y;
    }

    inline double len() { return sqrt(x * x + y * y); }

    inline void getAngle() { ang = atan2(y, x); }

    inline void input() {
        static int x, y;
        IO::read(x), IO::read(y);
        this->x = x, this->y = y;
    }
};

inline double dot(const Point &a, const Point &b) {
    return a.x * b.x + a.y * b.y;
}

inline double cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}

inline Point getIntersection(const Point &a1, const Point &a2, const Point &b1,
                             const Point &b2) {
    Point u = a1 - b1, av = a2 - a1, bv = b2 - b1;
    double t = cross(bv, u) / cross(av, bv);
    return a1 + av * t;
}

const Point O = Point(0, 0);
Point base;

struct Line {
    Point u, v;
    int id, in;
    double ang;
    Line() {}
    Line(const Point &u, const Point &v, int in, int id, double ang)
        : u(u), v(v), in(in), id(id), ang(ang) {}

    inline bool operator<(const Line &b) const {
        return u == b.u ? cross(v - u, b.v - u) < 0
                        : getIntersection(u, v, O, base).len() <
                              getIntersection(b.u, b.v, O, base).len();
    }
} line[MAXN];

inline bool comparator(const Line &a, const Line &b) {
    return cmp(a.ang - b.ang) < 0 || cmp(a.ang - b.ang) == 0 && a.in > b.in;
}

int lineCnt, tot;
std::multiset<Line> set;
std::multiset<Line>::iterator it[MAXN];
Point point[MAXN], p[MAXN];
Point tri[MAXN][3], t[MAXN][3];

inline void add(const Point &u, const Point &v, int id) {
    u.ang > v.ang ? (line[lineCnt++] = Line(v, u, 1, id, v.ang),
                     line[lineCnt++] = Line(u, v, 0, id, u.ang))
                  : (line[lineCnt++] = Line(u, v, 1, id, u.ang),
                     line[lineCnt++] = Line(v, u, 0, id, v.ang));
}

inline double calc(const Line &s) {
    Point a = getIntersection(s.u, s.v, O, base);
    return a.len();
}

inline int solve(int n) {
    std::sort(p, p + n);
    std::sort(line, line + lineCnt, comparator);
    set.clear();
    register int ret = 0, i = 0, j = 0;
    for (; i < n; i++) {
        while (j < lineCnt &&
               (cmp(line[j].ang - p[i].ang) < 0 ||
                (cmp(line[j].ang - p[i].ang) == 0 && line[j].in))) {
            base = line[j].u;
            (line[j].in) ? (it[line[j].id] = set.insert(line[j]), 0)
                         : (set.erase(it[line[j].id]), 0);
            j++;
        }
        if (set.empty()) {
            ret++;
            continue;
        }
        base = p[i];
        double dis = calc(*set.begin());
        if (cmp(p[i].len() - dis) <= 0) ret++;
    }
    return ret;
}

int main() {
    // freopen("in.in", "r", stdin);
    register int V, T;
    IO::read(V), IO::read(T);
    for (int i = 1; i <= V; ++i) point[i].input();
    for (int i = 1; i <= T; ++i)
        for (int j = 0; j < 3; ++j) tri[i][j].input();

    int ans = 0;
    for (int i = 1; i <= V; ++i) {
        int cnt = 0;
        for (int j = i + 1; j <= V; ++j) {
            p[cnt++] = point[j] - point[i];
            p[cnt - 1].getAngle();
        }
        tot = lineCnt = 0;
        for (int j = 1; j <= T; ++j) {
            for (int k = 0; k < 3; ++k) t[j][k] = tri[j][k] - point[i];
            t[j][3] = t[j][0];
            Point u, v;
            double mx = 0;
            for (int k = 0; k < 3; ++k) {
                double ang = dot(t[j][k], t[j][k + 1]) / t[j][k].len() /
                             t[j][k + 1].len();
                ang = acos(ang);
                if (ang > mx) mx = ang, u = t[j][k], v = t[j][k + 1];
            }
            u.getAngle(), v.getAngle();
            double d = fabs(u.ang - v.ang);
            if (d < PI) {
                add(u, v, tot++);
            } else {
                Point tmp = getIntersection(u, v, O, Point(-1.0, 0));
                tmp.ang = PI * cmp(u.ang);
                add(u, tmp, tot++);
                tmp.ang = PI * cmp(v.ang);
                add(v, tmp, tot++);
            }
        }
        int ret = solve(cnt);
        ans += ret;
    }
    printf("%d", ans);
    return 0;
}