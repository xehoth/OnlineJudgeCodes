/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1038」瞭望塔 26-03-2017
 * 模拟退火
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 305;

int n;
typedef std::pair<double, double> Point;
Point p[MAXN];

inline bool check(const Point& point) {
    for (register int i = 2; i <= n; i++)
        if ((point.first - p[i].first) * (point.second - p[i - 1].second) >
            (point.first - p[i - 1].first) * (point.second - p[i].second))
            return false;
    return true;
}

inline double getHeight(double x) {
    for (register int i = 2; i <= n; i++)
        if (p[i - 1].first <= x && p[i].first >= x)
            return p[i - 1].second + (p[i].second - p[i - 1].second) /
                                         (p[i].first - p[i - 1].first) *
                                         (x - p[i - 1].first);
    return -1;
}

const double EPS = 1e-5;
const double BOUND = 1e11;
const double DROP = 0.998;

inline double divide(double x) {
    double l = 0, r = BOUND;
    double mid = (l + r) / 2;
    while (r - l > 1e-5) {
        if (check(Point(x, mid)))
            r = mid;
        else
            l = mid;
        mid = (l + r) / 2;
    }
    return mid - getHeight(x);
    ;
}

inline double get() { return (rand() % 10000) / 10000.0; }

inline void simulatedAnnealing() {
    double now = (p[1].first + p[n].first) / 2, ans, pans;
    ans = divide(now);
    for (double t = (p[n].first - p[1].first); t > EPS; t *= DROP) {
        double tmp = now + t * sin(rand());
        if (tmp < p[1].first || tmp > p[n].first) continue;
        pans = divide(tmp);
        double delta = ans - pans;
        if (delta > 0 || std::exp(delta / t) > get()) now = tmp, ans = pans;
    }
    std::cout << std::fixed << std::setprecision(3) << ans;
}

int main() {
    // freopen("in.in", "r", stdin);
    srand(495);
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    std::cin >> n;
    for (register int i = 1; i <= n; i++) std::cin >> p[i].first;
    for (register int i = 1; i <= n; i++) std::cin >> p[i].second;
    simulatedAnnealing();
    return 0;
}
