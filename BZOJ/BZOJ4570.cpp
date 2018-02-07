#include <bits/stdc++.h>
#define long long long

inline char read() {
    static const int IN_LEN = 100000;
    static char buf[IN_LEN], *s, *t;
    if (s == t) {
        t = (s = buf) + fread(buf, 1, IN_LEN, stdin);
        if (s == t) return -1;
    }
    return *s++;
}

template <class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == '-') iosig = true;
        if (c == -1) return;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    if (iosig) x = -x;
}

const int MAXN = 1000000 + 9;
const double INF = 1e100;

struct Point {
    int x, y;

    inline Point() {}

    inline Point(int x, int y) : x(x), y(y) {}

    inline bool operator<(const Point &b) const {
        return (x < b.x) || (x == b.x && y < b.y);
    }

    inline Point operator-(const Point &b) { return Point(x - b.x, y - b.y); }
    inline long operator*(const Point &b) {
        return (long)x * b.y - (long)y * b.x;
    }
} p[MAXN], con[MAXN];

inline void convexHull(int &cnt, const int n, Point *p, Point *con) {
    con[++cnt] = p[n];
    for (int i = n - 1; i; i--) {
        while (cnt > 1 &&
               (con[cnt] - con[cnt - 1]) * (p[i] - con[cnt - 1]) <= 0)
            cnt--;
        con[++cnt] = p[i];
    }
    while (cnt > 1 && con[cnt].y <= con[cnt - 1].y) cnt--;
}

inline int andrewScanConvexHull(Point *p, int n, Point *con) {
    std::sort(p, p + n);
    register int top = 0, k;
    for (register int i = 0; i < n; i++) {
        while (top > 1 &&
               (con[top - 1] - con[top - 2]) * (p[i] - con[top - 2]) <= 0)
            top--;
        con[top++] = p[i];
    }
    k = top;
    for (register int i = n - 2; i >= 0; i--) {
        while (top > k &&
               (con[top - 1] - con[top - 2]) * (p[i] - con[top - 2]) <= 0)
            top--;
        con[top++] = p[i];
    }
    return n > 1 ? --top : top;
}

int main() {
    // freopen("in.in", "r", stdin);
    int n, cnt = 0;
    read(n);
    for (register int i = 1; i <= n; i++) read(p[i].x), read(p[i].y);
    std::sort(p + 1, p + 1 + n);
    convexHull(cnt, n, p, con);
    double r, l = INF, max;
    double ans = INF;
    for (register int i = 1; i <= cnt; i++) {
        Point v = con[i + 1] - con[i];
        std::swap(l, r);
        l = (i != cnt ? (double)-v.y / v.x : 0);
        max = sqrt((double)con[i].y / con[i].x);
        if (r < max)
            ans = std::min(ans,
                           con[i].x * r + con[i].y / r + con[i].x + con[i].y);
        else if (l > max)
            ans = std::min(ans,
                           con[i].x * l + con[i].y / l + con[i].x + con[i].y);
        else
            ans = std::min(
                ans, con[i].x * max + con[i].y / max + con[i].x + con[i].y);
    }
    printf("%.4lf\n", ans);
    return 0;
}