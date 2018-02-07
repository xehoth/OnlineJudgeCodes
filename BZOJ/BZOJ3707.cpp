#include <bits/stdc++.h>

namespace {

const int MAXN = 1010;

struct Point {
    double x, y;

    Point() {}

    Point(const double x, const double y) : x(x), y(y) {}

    inline bool operator<(const Point &b) const {
        return x < b.x || (x == b.x && y < b.y);
    }

    inline Point operator-(const Point &b) const {
        return Point(x - b.x, y - b.y);
    }

    inline double operator*(const Point &b) const { return x * b.y - y * b.x; }
} p[MAXN];

struct Line {
    int x, y;
    double k;
    bool operator<(const Line &b) const { return k < b.k; }
} l[MAXN * MAXN];

double ans = DBL_MAX;

int n, pos[MAXN], id[MAXN], cnt;

inline void solve() {
    scanf("%d", &n);
    for (register int i = 1; i <= n; i++) scanf("%lf%lf", &p[i].x, &p[i].y);
    std::sort(p + 1, p + n + 1);
    for (register int i = 1; i <= n; i++) id[i] = pos[i] = i;
    for (register int i = 1; i < n; i++) {
        for (register int j = i + 1; j <= n; j++) {
            l[++cnt].x = i, l[cnt].y = j;
            l[cnt].k = atan2(p[j].y - p[i].y, p[j].x - p[i].x);
        }
    }
    std::sort(l + 1, l + cnt + 1);
    for (register int i = 1; i <= cnt; i++) {
        register int a = l[i].x, b = l[i].y;
        if (id[a] > id[b]) std::swap(a, b);
        if (id[a] > 1)
            ans = fmin(ans, fabs((p[b] - p[a]) * (p[b] - p[pos[id[a] - 1]])));
        if (id[b] < n)
            ans = fmin(ans, fabs((p[b] - p[a]) * (p[b] - p[pos[id[b] + 1]])));
        std::swap(id[a], id[b]);
        std::swap(pos[id[a]], pos[id[b]]);
    }
    printf("%.2f\n", ans / 2.0);
}
}  // namespace

int main() {
    solve();
    return 0;
}