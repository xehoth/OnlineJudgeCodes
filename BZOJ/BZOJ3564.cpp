#include <bits/stdc++.h>
using namespace std;
const int MAXN = 50005;
const double PI = acos(-1.0);
const double EPS = 1E-6;
struct Point {
    double x;
    double y;
    Point(double a = 0, double b = 0) {
        x = a;
        y = b;
    }
};
Point operator+(Point p1, Point p2) { return Point(p1.x + p2.x, p1.y + p2.y); }
Point operator-(Point p1, Point p2) { return Point(p1.x - p2.x, p1.y - p2.y); }
double operator*(Point p1, Point p2) { return p1.x * p2.y - p1.y * p2.x; }
Point operator/(Point p1, double x) { return Point(p1.x / x, p1.y / x); }
Point operator*(Point p, double x) { return Point(p.x * x, p.y * x); }
struct Segment {
    Point s;
    Point e;
    Segment(Point a, Point b) {
        s = a;
        e = b;
    }
    Segment() {}
};
struct Line {
    double a;
    double b;
    double c;
    Line(double d1 = 1, double d2 = -1, double d3 = 0) {
        a = d1;
        b = d2;
        c = d3;
    }
};
Point rotate(Point o, double alpha, Point p) {
    Point tp;
    p.x -= o.x;
    p.y -= o.y;
    tp.x = p.x * cos(alpha) - p.y * sin(alpha) + o.x;
    tp.y = p.y * cos(alpha) + p.x * sin(alpha) + o.y;
    return tp;
}
int n;
Point po[MAXN], O;
double r;
double dis(Point A, Point B) {
    return sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
}
inline Point circumcenter(const Point &a, const Point &b, const Point &c) {
    Point ret;
    double a1 = b.x - a.x, b1 = b.y - a.y, c1 = (a1 * a1 + b1 * b1) / 2.0;
    double a2 = c.x - a.x, b2 = c.y - a.y, c2 = (a2 * a2 + b2 * b2) / 2.0;
    double d = a1 * b2 - a2 * b1;
    ret.x = a.x + (c1 * b2 - c2 * b1) / d;
    ret.y = a.y + (a1 * c2 - a2 * c1) / d;
    return ret;
}
void solve(Point *p, int n, Point &c, double &r) {
    std::random_shuffle(p, p + n);
    c = p[0];
    r = 0;
    for (int i = 1; i < n; i++) {
        if (dis(p[i], c) > r + EPS) {
            c = p[i];
            r = 0;
            for (int j = 0; j < i; j++) {
                if (dis(p[j], c) > r + EPS) {
                    c.x = (p[i].x + p[j].x) / 2;
                    c.y = (p[i].y + p[j].y) / 2;
                    r = dis(p[j], c);
                    for (int k = 0; k < j; k++) {
                        if (dis(p[k], c) > r + EPS) {
                            c = circumcenter(p[i], p[j], p[k]);
                            r = dis(p[i], c);
                        }
                    }
                }
            }
        }
    }
}
int main() {
    srand(772002);
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        scanf("%lf%lf", &po[i].x, &po[i].y);
    }
    double a, p;
    scanf("%lf%lf", &a, &p);
    double ang = -a / 180.0 * PI;
    for (int i = 0; i < n; i++) {
        po[i] = rotate(Point(0, 0), ang, po[i]);
        po[i].x /= p;
    }
    solve(po, n, O, r);
    printf("%.3f\n", r);
}