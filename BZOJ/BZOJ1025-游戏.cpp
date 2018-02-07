/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1025」 11-10-2017
 *
 * @author xehoth
 */
#include <cstdio>

const int MAXN = 1000;

int primes[MAXN], isNotPrime[MAXN + 1], cnt;

inline void getPrimes() {
    isNotPrime[0] = isNotPrime[1] = true;
    for (int i = 2; i <= MAXN; i++) {
        if (!isNotPrime[i]) primes[++cnt] = i;
        for (int j = 1; j <= cnt && i * primes[j] <= MAXN; j++) {
            isNotPrime[i * primes[j]] = true;
            if (i % primes[j] == 0) break;
        }
    }
}

int main() {
    int n;
    scanf("%d", &n);
    getPrimes();
    static long long f[MAXN + 1][MAXN + 1];
    f[0][0] = 1;
    for (register int i = 1; i <= cnt; i++) {
        for (register int k = 0; k <= n; k++) f[i][k] = f[i - 1][k];
        for (register int p = primes[i]; p <= n; p *= primes[i])
            for (register int k = p; k <= n; k++) f[i][k] += f[i - 1][k - p];
    }
    register long long ans = 0;
    for (register int i = 0; i <= n; i++) ans += f[cnt][i];
    printf("%lld\n", ans);
    return 0;
}