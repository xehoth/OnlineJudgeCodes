/*
 * created by xehoth 06-05-2017
 */
#include <bits/stdc++.h>

char ibuf[1000000], *ih = ibuf;

template <class T>
inline void read(T &x) {
    static char c;
    for (c = *ih++; !isdigit(c); c = *ih++)
        ;
    for (x = 0; isdigit(c); c = *ih++) x = (x + (x << 2) << 1) + (c ^ '0');
}

const int MAXN = 100001, MOD = 100003;

int n, k, a[MAXN], status[MAXN], t, inv[MAXN] = {0, 1}, g[MAXN], fac = 1;

inline void solve() {
    read(n), read(k), g[n] = 1;
    for (register int i = 1; i <= n; i++)
        read(a[i]), fac = (long long)fac * i % MOD;
    for (register int i = n, p, j; i >= 1; i--) {
        for (p = a[i], j = i + i; j <= n; j += i) (status[j]) ? (p ^= 1) : 0;
        (p) ? (status[i] = 1, t++) : 0;
    }
    if (t <= k) std::cout << (long long)t * fac % MOD, exit(0);
    for (register int i = 2; i <= n; i++)
        inv[i] = ((long long)MOD - MOD / i) * inv[MOD % i] % MOD;
    for (register int i = n - 1; i > k; i--)
        g[i] = (((long long)n - i) * g[i + 1] % MOD + n) * inv[i] % MOD;
    std::cout << std::accumulate(g + k + 1, g + t + 1, (long long)k) * fac %
                     MOD;
}

int main() {
    // freopen("trennen.in", "r", stdin);
    // freopen("trennen.out", "w", stdout);
    fread(ibuf, 1, 1000000, stdin), solve();
    return 0;
}