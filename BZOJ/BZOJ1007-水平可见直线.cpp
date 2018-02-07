/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1007」水平可见直线 27-03-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 1000010;

struct Data {
    double k, b;
    int id;
} l[MAXN];

inline bool operator<(const Data &a, const Data &b) {
    if (a.k != b.k) return a.k < b.k;
    return a.b > b.b;
}

inline double getpos(const Data &a, const Data &b) {
    return (b.b - a.b) / (a.k - b.k);
}

int s[MAXN];
bool ans[MAXN];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%lf%lf", &l[i].k, &l[i].b);
        l[i].id = i;
    }
    std::sort(l + 1, l + 1 + n);

    int top = 1;
    s[1] = 1;
    for (int i = 2; i <= n; i++) {
        if (l[i].k - l[i - 1].k < 1e-8) continue;
        while (top > 1 &&
               getpos(l[i], l[s[top]]) <= getpos(l[s[top]], l[s[top - 1]]))
            top--;
        s[++top] = i;
    }
    for (int i = 1; i <= top; i++) ans[l[s[i]].id] = 1;
    for (int i = 1; i <= n; i++)
        if (ans[i]) printf("%d ", i);

    return 0;
}