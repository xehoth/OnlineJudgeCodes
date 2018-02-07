/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1087」04-04-2017
 *
 * @author xehoth
 */
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;
int n, m, k, stay[101], cnt[101];
long long ans, f[10][101][101];
bool map[101][101];
void dfs(int p, int put, int num) {
    stay[++m] = num;
    cnt[m] = p;
    if (p >= k || p >= (n + 1) / 2) return;
    for (int i = put + 2; i <= n; i++) dfs(p + 1, i, num + (1 << (i - 1)));
}
int main() {
    scanf("%d%d", &n, &k);
    m = 0;
    dfs(0, -1, 0);
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= m; j++)
            map[i][j] = map[j][i] =
                ((stay[i] & stay[j]) || ((stay[i] << 1) & stay[j]) ||
                 ((stay[i] >> 1) & stay[j]))
                    ? 0
                    : 1;
    for (int i = 1; i <= m; i++) f[1][cnt[i]][i] = 1ll;
    for (int i = 2; i <= n; i++)
        for (int j = 0; j <= k; j++)
            for (int now = 1; now <= m; now++) {
                if (cnt[now] > j) continue;
                for (int l = 1; l <= m; l++)
                    if (map[now][l] && cnt[l] + cnt[now] <= j)
                        f[i][j][now] += f[i - 1][j - cnt[now]][l];
            }
    ans = 0;
    for (int i = 1; i <= m; i++) ans += f[n][k][i];
    printf("%lld", ans);
    return 0;
}