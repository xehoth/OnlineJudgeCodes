#include <bits/stdc++.h>

const int MAXN = 600 + 10;
const int MOD = 1000000000 + 7;

int t, x, y;
int f[MAXN][MAXN];

inline int dfs(int x, int y) {
    if (x == 0) return 1;
    if (y == 0) return x == 1;
    register int &ret = f[x][y];
    if (~ret) return f[x][y];
    ret = 0;
    for (register int i = 1; i <= x; i++)
        ret = (ret + (long long)dfs(i - 1, y - 1) *
                         (long long)dfs(x - i, y - 1) % MOD) %
              MOD;
    return ret;
}

int main() {
    memset(f, -1, sizeof(f));
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &x, &y);
        printf("%d\n", (dfs(x, y) - (y ? dfs(x, y - 1) : 0) + MOD) % MOD);
    }
    return 0;
}