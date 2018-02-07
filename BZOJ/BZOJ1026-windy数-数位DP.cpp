/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1026」windy数 15-01-2017
 * 数位DP
 * @author xehoth
 */
#include <bits/stdc++.h>

using namespace std;

#define inline __attribute__((always_inline)) __attribute__((optimize("O3")))

int dp[20][20][2], digits[20];

int dfs(int pos, int pre, int status, int limit) {
    if (pos < 1) return status;
    if (limit && ~dp[pos][pre][status] && status) return dp[pos][pre][status];
    register int end = limit ? 9 : digits[pos], ret = 0;
    for (register int i = 0; i <= end; i++)
        if (!status || abs(pre - i) >= 2)
            ret += dfs(pos - 1, i, status || i > 0, limit || i < end);

    if (limit && status) dp[pos][pre][status] = ret;

    return ret;
}

inline int solve(int x) {
    register int len = 0;
    while (x) {
        digits[++len] = x % 10;
        x /= 10;
    }
    return dfs(len, 11, 0, 0);
}

inline int main(int argc, char const *argv[]) {
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
#endif
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int a, b;
    cin >> a >> b;
    memset(dp, -1, sizeof(dp));
    cout << solve(b) - solve(a - 1);
    return 0;
}