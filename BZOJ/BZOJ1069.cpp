/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1069」31-03-2017
 *
 * @author xehoth
 */
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>
#define eps 1e-4
using namespace std;
struct point {
    double x;
    double y;
};
point p[5010], stack[5010];
double Ans = 0;
int n, t;
double dis(point x, point y) {
    double ret = 0;
    ret += (x.x - y.x) * (x.x - y.x);
    ret += (x.y - y.y) * (x.y - y.y);
    return ret;
}
double calc(point x, point y, point z) {
    point A, B;
    A.x = x.x - z.x;
    A.y = x.y - z.y;
    B.x = y.x - z.x;
    B.y = y.y - z.y;
    return A.x * B.y - A.y * B.x;
}
bool comp(const point &x, const point &y) {
    double z = calc(x, y, p[1]);
    if (z == 0)
        return dis(x, p[1]) < dis(y, p[1]);
    else
        return z > 0;
}
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%lf%lf", &p[i].x, &p[i].y);
    t = 1;
    for (int i = 2; i <= n; i++)
        if (p[i].y < p[t].y || (p[i].y == p[t].y && p[i].x < p[t].x)) t = i;
    swap(p[1], p[t]);
    sort(p + 2, p + n + 1, comp);
    int head = 1, tail = 0;
    for (int i = 1; i <= n; i++) {
        while (head < tail && calc(p[i], stack[tail], stack[tail - 1]) > -eps)
            tail--;
        stack[++tail] = p[i];
    }
    for (int i = 1; i <= tail; i++) {
        int M = i + 1, N = i + 1;
        for (int j = i + 1; j <= tail; j++) {
            while (N < tail && calc(stack[j], stack[N], stack[i]) <
                                   calc(stack[j], stack[N + 1], stack[i]))
                N++;
            while (M < j && calc(stack[M], stack[j], stack[i]) <
                                calc(stack[M + 1], stack[j], stack[i]))
                M++;
            Ans = max(Ans, calc(stack[j], stack[N], stack[i]) / 2.0 +
                               calc(stack[M], stack[j], stack[i]) / 2.0);
        }
    }
    printf("%0.3f", Ans);
}