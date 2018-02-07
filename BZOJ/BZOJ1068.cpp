/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1068」16-01-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 50 + 10;
int f[MAXN][MAXN][2];
char s[100];

inline bool isSame(int l, int r) {
    register int n = r - l + 1;
    if (n & 1) return 0;
    register int mid = l + r >> 1;
    for (register int i = l, j = mid + 1; i <= mid; i++, j++)
        if (s[i] != s[j]) return 0;
    return 1;
}

inline int dfs(int l, int r, bool t) {
    if (l == r) return 1;
    if (f[l][r][t]) return f[l][r][t];
    register int &res = f[l][r][t];
    res = r - l + 1;
    if (t) {
        for (register int i = l; i < r; i++) {
            res = std::min(res, dfs(l, i, 1) + dfs(i + 1, r, 1) + 1);
        }
    }
    for (register int i = l; i < r; i++)
        res = std::min(res, dfs(l, i, t) + r - i);
    if (isSame(l, r)) res = std::min(res, dfs(l, (l + r) >> 1, 0) + 1);
    return res;
}

int main() {
    scanf("%s", s + 1);
    printf("%d", dfs(1, strlen(s + 1), 1));
    return 0;
}