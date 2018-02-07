#include <bits/stdc++.h>
#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) > (b)) ? (b) : (a))
#define sign(x) ((x) > eps ? 1 : ((x) < -eps ? (-1) : (0)))
#define square(x) (x * x)
const double eps = 1e-6;
#define MAXN 10000
using namespace std;
struct point {
    double x, y;
    point(double x1 = 0.0, double y1 = 0.0) : x(x1), y(y1) {}
    inline point operator-(const point& p) const {
        return point(x - p.x, y - p.y);
    }
    inline point operator+(const point& p) const {
        return point(x + p.x, y + p.y);
    }
    inline double operator*(const point& p) const { return x * p.x, y * p.y; }
    inline point operator*(double p) const { return point(x * p, y * p); }
    inline point operator/(double p) const { return point(x / p, y / p); }
    inline double operator^(const point& p) const { return x * p.y - y * p.x; }
} points[MAXN];
inline double cross(const point& a, const point& b) { return a ^ b; }
inline double angle(const point& a, const point& b) {
    return atan2((a - b).y, (a - b).x);
}
struct line {
    point st, ed;
    double ang;
    line() {}
    line(const point& a, const point& b) : st(a), ed(b), ang(angle(a, b)) {}
};
vector<line> lines;
int n;
inline point lineIntersection(const line& a, const line& b) {
    double x = cross(a.ed - a.st, b.ed - b.st);
    double y = cross(b.st - a.st, a.ed - a.st);
    return b.st + (b.ed - b.st) * y / x;
}
inline bool cmp(const line& a, const line& b) {
    if (!sign(a.ang - b.ang)) return sign(cross(a.ed - a.st, b.ed - a.st)) > 0;
    return sign(a.ang - b.ang) < 0;
}
#define onLeft(a, b) (sign(cross(b - a.st, a.ed - a.st)) <= 0)
typedef vector<point> convex;
inline convex halfPlaneIntersection(vector<line> lines) {
    register int h = 1, t = 0;
    convex polygon;
    polygon.reserve(n);
    vector<line> q(n + 1);
    register int tot = 0;
    sort(lines.begin() + 1, lines.begin() + n + 1, cmp);
    for (register int i = 1; i <= n; i++) {
        if (sign(lines[i].ang - lines[i - 1].ang)) tot++;
        lines[tot] = lines[i];
    }
    q[++t] = lines[1], q[++t] = lines[2];
    for (register int i = 1; i <= tot; i++) {
        while (h < t && !onLeft(lines[i], lineIntersection(q[t - 1], q[t])))
            t--;
        if (h < t &&
            !sign(fabs(cross(q[t].ed - q[t].st, q[t - 1].ed - q[t - 1].st)))) {
            if (onLeft(q[t], lines[i].st)) q[t] = lines[i];
        } else
            q[++t] = lines[i];
    }
    while (t - h >= 2 && !onLeft(q[t], lineIntersection(q[h], q[h + 1]))) h++;
    while (t - h >= 2 && !onLeft(q[h], lineIntersection(q[t], q[t - 1]))) t--;
    polygon.push_back(point());
    for (register int i = h; i < t; i++)
        polygon.push_back(lineIntersection(q[i], q[i + 1]));
    polygon.push_back(lineIntersection(q[t], q[h]));
    return polygon;
}
inline double getSqrOfPolygon(convex polygon) {
    double sqr = 0;
    polygon.push_back(polygon[1]);
    for (register int i = 1, range = polygon.size() - 1; i < range; i++)
        sqr += cross(polygon[i], polygon[i + 1]);
    return fabs(sqr) / 2;
}
int main() {
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
#endif
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int m;
    cin >> m;
    lines.push_back(line());
    for (register int i = 1; i <= m; i++) {
        register int cnt;
        cin >> cnt;
        point p1, p2, tmp;
        cin >> p1.x >> p1.y, tmp = p1;
        for (register int j = 2; j <= cnt; j++)
            cin >> p2.x >> p2.y, lines.push_back(line(p1, p2)), n++, p1 = p2;
        lines.push_back(line(p1, tmp)), n++;
    }
    cout << fixed << setprecision(3)
         << getSqrOfPolygon(halfPlaneIntersection(lines));
    return 0;
}
