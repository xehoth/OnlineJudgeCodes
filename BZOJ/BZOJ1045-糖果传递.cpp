/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1045」糖果传递 24-03-2017
 *
 * @author xehoth
 */
#include <algorithm>
#include <cstdio>

const int MAXN = 1000000;

int main() {
    int n;
    scanf("%d", &n);

    static long long a[MAXN];
    long long sum = 0;
    for (int i = 0; i < n; i++) {
        scanf("%lld", &a[i]);
        sum += a[i];
    }

    long long avg = sum / n;
    static long long c[MAXN];
    c[0] = 0;
    for (int i = 1; i < n; i++) c[i] = c[i - 1] - a[i] + avg;

    std::sort(c, c + n);

    long long mid = c[n / 2], ans = 0;
    for (int i = 0; i < n; i++) ans += llabs(c[i] - mid);

    printf("%lld\n", ans);

    return 0;
}