#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
inline ll gcd(ll x, ll y) {
    if (!x) return y;
    if (!y) return x;
    register int i, j;
    for (i = 0; !(x & 1); i++) x >>= 1;
    for (j = 0; !(y & 1); j++) y >>= 1;
    i = min(i, j);
    while (true) {
        if (x < y) x ^= y ^= x ^= y;
        if (!(x -= y)) return y << i;
        while (!(x & 1)) x >>= 1;
    }
}
int m, n;
ll ans, tmp;
ll f[1000005][4];
inline void init() {
    n++, m++;
    f[0][0] = 1;
    for (register int i = 1, range = n * m + 1; i < range; i++) {
        f[i][0] = 1;
        for (register int j = 1; j < 4; j++)
            f[i][j] = f[i - 1][j - 1] + f[i - 1][j];
    }
}
int main() {
    cin >> n >> m;
    init();
    ans = f[n * m][3] - n * f[m][3] - m * f[n][3];
    for (register int i = 1; i < n; i++) {
        for (register int j = 1; j < m; j++) {
            tmp = gcd(i, j) + 1;
            if (tmp > 2) ans -= (tmp - 2) * (n - i) * (m - j) << 1;
        }
    }
    cout << ans;
    return 0;
}