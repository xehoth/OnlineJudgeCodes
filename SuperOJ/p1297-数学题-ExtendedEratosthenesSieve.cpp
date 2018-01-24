/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「SuperOJ 1297」数学题 24-01-2018
 * Extended Eratosthenes Sieve
 * @author xehoth
 */
#include <bits/stdc++.h>

int d, M, n;
const int MOD = 1e9 + 7;

std::vector<int> pre[2], suc[2], primes;

inline int add(const int x, const int v) {
    return x + v >= MOD ? x + v - MOD : x + v;
}

inline int dec(const int x, const int v) {
    return x - v < 0 ? x - v + MOD : x - v;
}

int rec(int res, int last, int mul) {
    int t = dec((res > M ? suc[1][n / res] : pre[1][res]),
                pre[1][primes[last] - 1]);
    int ret = (unsigned long long)t * mul % MOD;
    for (int i = last, p; i < (int)primes.size(); i++) {
        p = primes[i];
        if (p * p > res) break;
        for (int q = p, nres = res, nmul, pk = p;
             (unsigned long long)p * q <= res; q *= p) {
            nmul = mul * ((unsigned long long)pk + d) % MOD;
            ret = add(ret, rec(nres /= p, i + 1, nmul));
            pk = (unsigned long long)pk * p % MOD;
            ret = add(ret, mul * ((unsigned long long)pk + d) % MOD);
        }
    }
    return ret;
}

inline int solve(const int n) {
    M = sqrt(n);
    primes.reserve(M);
    pre[0].resize(M + 1);
    pre[1].resize(M + 1);
    suc[0].resize(M + 1);
    suc[1].resize(M + 1);
    for (int i = 1, t; i <= M; i++) {
        pre[0][i] = i - 1;
        pre[1][i] = (i * (i + 1ull) / 2 - 1 + MOD) % MOD;
        t = n / i;
        suc[0][i] = t - 1;
        suc[1][i] = (t * (t + 1ull) / 2 - 1 + MOD) % MOD;
    }
    for (int p = 2, end; p <= M; p++) {
        if (pre[0][p] == pre[0][p - 1]) continue;
        primes.push_back(p);
        const int q = p * p, m = n / p;
        const int pcnt = pre[0][p - 1], psum = pre[1][p - 1];
        end = std::min(M, n / q);
        for (int i = 1, w = M / p; i <= w; i++) {
            suc[0][i] = dec(suc[0][i], dec(suc[0][i * p], pcnt));
            suc[1][i] =
                dec(suc[1][i],
                    (unsigned long long)dec(suc[1][i * p], psum) * p % MOD);
        }
        for (int i = M / p + 1; i <= end; i++) {
            suc[0][i] = dec(suc[0][i], dec(pre[0][m / i], pcnt));
            suc[1][i] =
                dec(suc[1][i],
                    (unsigned long long)dec(pre[1][m / i], psum) * p % MOD);
        }
        for (int i = M; i >= q; i--) {
            pre[0][i] = dec(pre[0][i], dec(pre[0][i / p], pcnt));
            pre[1][i] =
                dec(pre[1][i],
                    (unsigned long long)dec(pre[1][i / p], psum) * p % MOD);
        }
    }
    primes.push_back(M + 1);
    for (int i = 1; i <= M; i++) {
        pre[1][i] = (pre[1][i] + (unsigned long long)d * pre[0][i]) % MOD;
        suc[1][i] = (suc[1][i] + (unsigned long long)d * suc[0][i]) % MOD;
    }
    return n > 1 ? 1 + rec(n, 0, 1) : 1;
}
int main() {
    std::cin >> n >> d;
    std::cout << solve(n);
}