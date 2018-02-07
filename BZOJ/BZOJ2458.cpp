#include <bits/stdc++.h>
inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    if (s == t) {
        t = (s = buf) + fread(buf, 1, IN_LEN, stdin);
        if (s == t) return -1;
    }
    return *s++;
}
template <class T>
inline void read(T &x) {
    static bool iosig;
    static char ch;
    for (iosig = 0, ch = read(); !isdigit(ch); ch = read()) {
        if (ch == -1) return;
        if (ch == '-') iosig = 1;
    }
    for (x = 0; isdigit(ch); ch = read()) x = (x << 1) + (x << 3) + (ch ^ '0');
    if (iosig) x = -x;
}
const int MAXN = 200005;
const double INF = 210000000000.0;
struct Point {
    int x, y;
} a[MAXN], num[MAXN];
int n;
inline bool cmpx(const Point &a, const Point &b) { return a.x < b.x; }
inline bool cmpy(const Point &a, const Point &b) { return a.y < b.y; }
inline double dis(const Point &a, const Point &b) {
    return sqrt((a.x - b.x) * 1. * (a.x - b.x) +
                (a.y - b.y) * 1. * (a.y - b.y));
}
inline double divide(int l, int r) {
    if (l == r) return INF;
    if (l + 1 == r) return INF;
    if (l + 2 == r)
        return dis(a[l], a[l + 1]) + dis(a[l + 1], a[r]) + dis(a[l], a[r]);
    register int mid = l + r >> 1;
    double d1 = divide(l, mid), d2 = divide(mid + 1, r);
    double d = std::min(d1, d2), ans = d, bound = d / 2.0;
    int cnt = 0;
    for (register int i = l; i <= r; i++)
        if (fabs(a[mid].x - a[i].x) <= bound) num[++cnt] = a[i];
    std::sort(num + 1, num + cnt + 1, cmpy);
    for (register int i = 1; i < cnt - 1; i++) {
        for (register int j = i + 1; j < cnt; j++) {
            if (num[j].y - num[i].y > bound) break;
            for (register int k = j + 1; k <= cnt; k++) {
                if (num[k].y - num[i].y > bound) break;
                double tmp = dis(num[i], num[j]) + dis(num[i], num[k]) +
                             dis(num[j], num[k]);
                if (tmp < ans) ans = tmp;
            }
        }
    }
    return ans;
}
int main() {
    read(n);
    for (register int i = 1; i <= n; i++) read(a[i].x), read(a[i].y);
    std::sort(a + 1, a + n + 1, cmpx);
    printf("%.6lf", divide(1, n));
    return 0;
}
