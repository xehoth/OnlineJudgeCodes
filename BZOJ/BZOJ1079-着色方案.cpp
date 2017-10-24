/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1079」着色方案 24-10-2017
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MOD = 1e9 + 7;

long long f[16][16][16][16][16][6];
bool vis[16][16][16][16][16][6];

long long dp(int c1, int c2, int c3, int c4, int c5, int last) {
    register long long &ret = f[c1][c2][c3][c4][c5][last];
    if (c1 + c2 + c3 + c4 + c5 == 0) return 1;
    if (vis[c1][c2][c3][c4][c5][last]) return ret;
    if (c1) ret += (c1 - (last == 2)) * dp(c1 - 1, c2, c3, c4, c5, 1);
    if (c2) ret += (c2 - (last == 3)) * dp(c1 + 1, c2 - 1, c3, c4, c5, 2);
    if (c3) ret += (c3 - (last == 4)) * dp(c1, c2 + 1, c3 - 1, c4, c5, 3);
    if (c4) ret += (c4 - (last == 5)) * dp(c1, c2, c3 + 1, c4 - 1, c5, 4);
    if (c5) ret += c5 * dp(c1, c2, c3, c4 + 1, c5 - 1, 5);
    vis[c1][c2][c3][c4][c5][last] = true;
    return ret %= MOD;
}

int main() {
    register int n;
    int cnt[6] = {0};
    std::cin >> n;
    for (register int i = 1, x; i <= n; i++) std::cin >> x, cnt[x]++;
    std::cout << dp(cnt[1], cnt[2], cnt[3], cnt[4], cnt[5], 0);
}