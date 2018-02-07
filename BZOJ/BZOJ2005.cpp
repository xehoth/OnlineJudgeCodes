/*
 * created by xehoth on 13-01-2017
 */
#include <bits/stdc++.h>

const int MAXN = 1000001;

int main() {
    // freopen("in.in", "r", stdin);
    int n, m;
    static long long f[MAXN];

    scanf("%d %d", &n, &m);
    if (n > m) std::swap(n, m);

    long long ans = 0;

    for (register int i = n; i >= 1; i--) {
        f[i] = (long long)(n / i) * (m / i);
        for (register int j = i << 1; j <= n; j += i) f[i] -= f[j];
        ans += f[i] * ((i << 1) - 1);
    }
    std::cout << ans;
    return 0;
}