#include <bits/stdc++.h>

#define long long long
const int MAXN = 505, MOD = 1000000007;
int l[MAXN], r[MAXN], b[MAXN << 1], tot, f[2][MAXN], g[MAXN], inv[MAXN];
int main() {
    int n, L, *A, *B, cnt, ans = 0;
    scanf("%d", &n);
    for (register int i = 1; i <= n; i++)
        scanf("%d%d", l + i, r + i), b[++tot] = l[i], b[++tot] = ++r[i];
    std::sort(b + 1, b + 1 + tot), tot = std::unique(b + 1, b + 1 + tot) - b;
    for (register int i = 1; i <= n; i++) {
        l[i] = std::lower_bound(b + 1, b + tot, l[i]) - b;
        r[i] = std::lower_bound(b + 1, b + tot, r[i]) - b;
    }
    inv[1] = 1, tot -= 2, A = f[0], B = f[1];
    for (register int i = 2; i <= n; i++)
        inv[i] = (long)(MOD - MOD / i) * inv[MOD % i] % MOD;
    for (register int j = 1; j <= tot; j++) {
        L = b[j + 1] - b[j], std::swap(A, B);
        memset(g, 0, sizeof(int) * (n + 1));
        cnt = 0;
        g[0] = 1;
        for (register int i = 1; i <= n; i++) {
            if (l[i] <= j && j < r[i]) {
                for (register int k = std::min(++cnt, L); k; k--) {
                    (A[i] +=
                     (long)g[k - 1] * (L - k + 1) % MOD * inv[k] % MOD) %= MOD;
                    (g[k] +=
                     (long)g[k - 1] * (L - k + 1) % MOD * inv[k] % MOD) %= MOD;
                }
            }
            (A[i] += B[i]) %= MOD;
            (g[0] += B[i]) %= MOD;
            B[i] = 0;
        }
    }
    for (register int i = 1; i <= n; i++) (ans += A[i]) %= MOD;
    printf("%d", ans);
    return 0;
}