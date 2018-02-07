/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1088」12-03-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>
const int MAXN = 10000 + 10;
int n, a[MAXN], b[MAXN], cnt = 0;

inline bool check() {
    for (register int j = 2; j <= n; j++) b[j + 1] = a[j] - b[j] - b[j - 1];
    for (register int i = 1; i <= n; i++)
        if (b[i] != 1 && b[i] != 0) return false;
    if (b[n + 1]) return false;
    return true;
}

int main() {
    scanf("%d", &n);
    for (register int i = 1; i <= n; ++i) scanf("%d", &a[i]);
    for (register int i = 0; i <= a[1]; i++)
        memset(b, 0, sizeof(b)), b[1] = i, b[2] = a[1] - i,
                                 (check() ? cnt++ : 0);
    return std::cout << cnt, 0;
}
