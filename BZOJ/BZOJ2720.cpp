#include <bits/stdc++.h>

const int MAXN = 300;
typedef long double ld;

int n, h[MAXN + 1], tmp[MAXN + 1];
ld fac[MAXN + 1][MAXN + 1];

inline void init() {
    scanf("%d", &n);
    for (register int i = 1; i <= n; i++) scanf("%d", &h[i]), tmp[i] = h[i];
    std::sort(tmp + 1, tmp + 1 + n);
    for (int i = 1; i <= n; i++)
        h[i] = std::lower_bound(tmp + 1, tmp + 1 + n, h[i]) - tmp - 1;
    memset(fac, 0, sizeof(fac));
    fac[0][0] = 1;
    for (register int i = 1; i <= n; i++) {
        fac[i][0] = 1;
        for (register int j = 1; j <= i; j++)
            fac[i][j] = fac[i][j - 1] * (i - j + 1);
    }
}

inline void solve() {
    ld ans = 0;
    for (register int i = 1; i <= n; i++) {
        ld s = 0;
        for (register int j = 1; j <= n; j++) {
            ans += (s + fac[h[i]][j - 1] / fac[n - 1][j - 1] * j) / n;
            s += (n - h[i] - 1) * fac[h[i]][j - 1] / fac[n - 1][j] * j;
        }
    }
    printf("%.2f\n", (double)ans);
}

int main() {
    init(), solve();
    return 0;
}