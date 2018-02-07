#include <bits/stdc++.h>

using namespace std;

const int mod = 100000000;

int n, m;

inline void dp(int *map, int size) {
    static int f[13][4096], ans;
    for (register int i = 0; i <= size; i++)
        if (!(i & (i >> 1)) && (i | map[1]) == map[1]) f[1][i] = 1;
    for (register int i = 2; i <= m; i++)
        for (register int j = 0; j <= size; j++)
            if (f[i - 1][j])
                for (register int k = 0; k <= size; k++)
                    if (!(j & k) && (k | map[i]) == map[i] && !(k & (k >> 1)))
                        f[i][k] = (f[i][k] + f[i - 1][j]) % mod;
    for (register int i = 0; i <= size; i++) ans = (ans + f[m][i]) % mod;
    cout << ans;
}

int main() {
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
#endif
    ios::sync_with_stdio(0);
    cin.tie(0);
    cin >> m >> n;
    static int map[13];
    for (register int i = 1, x; i <= m; i++)
        for (register int j = 1; j <= n; j++)
            cin >> x, map[i] <<= 1, map[i] += x;
    dp(map, (1 << n) - 1);
    return 0;
}
