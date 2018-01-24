/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 4916」神犇和蒟蒻 24-01-2018
 * Extended Eratosthenes Sieve
 * @author xehoth
 */
#include <bits/stdc++.h>

int d, M, n;
const int MOD = 1e9 + 7;
const int INV6 = 166666668;

std::vector<int> pre[3], suc[3], primes;

inline int add(const int x, const int v) {
    return x + v >= MOD ? x + v - MOD : x + v;
}

inline int dec(const int x, const int v) {
    return x - v < 0 ? x - v + MOD : x - v;
}

int rec(int res, int last, int mul) {
    int t = dec((res > M ? suc[2][n / res] : pre[2][res]),
                pre[2][primes[last] - 1]);
    int ret = (unsigned long long)t * mul % MOD;
    for (int i = last, p; i < (int)primes.size(); i++) {
        p = primes[i];
        if (p * p > res) break;
        const int p2 = (unsigned long long)p * p % MOD;
        for (int q = p, nres = res, nmul = p * (p - 1ull) % MOD * mul % MOD;
             (unsigned long long)p * q <= res; q *= p) {
            ret = add(ret, rec(nres /= p, i + 1, nmul));
            nmul = (unsigned long long)nmul * p2 % MOD;
            ret = add(ret, nmul);
        }
    }
    return ret;
}

inline int solve(const int n) {
    M = sqrt(n);
    primes.reserve(M);
    pre[0].resize(M + 1);
    pre[1].resize(M + 1);
    pre[2].resize(M + 1);
    suc[0].resize(M + 1);
    suc[1].resize(M + 1);
    suc[2].resize(M + 1);
    for (int i = 1, t; i <= M; i++) {
        pre[0][i] = i - 1;
        pre[1][i] = (i * (i + 1ull) / 2 - 1 + MOD) % MOD;
        pre[2][i] =
            (i * (i + 1ull) % MOD * (2 * i + 1ull) % MOD * INV6 + MOD - 1) %
            MOD;
        t = n / i;
        suc[0][i] = t - 1;
        suc[1][i] = (t * (t + 1ull) / 2 - 1 + MOD) % MOD;
        suc[2][i] =
            (t * (t + 1ull) % MOD * (2 * t + 1ull) % MOD * INV6 + MOD - 1) %
            MOD;
    }
    for (int p = 2, end; p <= M; p++) {
        if (pre[0][p] == pre[0][p - 1]) continue;
        primes.push_back(p);
        const int q = p * p, m = n / p;
        const int pcnt = pre[0][p - 1], psum = pre[1][p - 1],
                  psum2 = pre[2][p - 1], p2 = (unsigned long long)p * p % MOD;
        end = std::min(M, n / q);
        for (int i = 1, w = M / p; i <= w; i++) {
            suc[0][i] = dec(suc[0][i], dec(suc[0][i * p], pcnt));
            suc[1][i] =
                dec(suc[1][i],
                    (unsigned long long)dec(suc[1][i * p], psum) * p % MOD);
            suc[2][i] =
                dec(suc[2][i],
                    (unsigned long long)dec(suc[2][i * p], psum2) * p2 % MOD);
        }
        for (int i = M / p + 1; i <= end; i++) {
            suc[0][i] = dec(suc[0][i], dec(pre[0][m / i], pcnt));
            suc[1][i] =
                dec(suc[1][i],
                    (unsigned long long)dec(pre[1][m / i], psum) * p % MOD);
            suc[2][i] =
                dec(suc[2][i],
                    (unsigned long long)dec(pre[2][m / i], psum2) * p2 % MOD);
        }
        for (int i = M; i >= q; i--) {
            pre[0][i] = dec(pre[0][i], dec(pre[0][i / p], pcnt));
            pre[1][i] =
                dec(pre[1][i],
                    (unsigned long long)dec(pre[1][i / p], psum) * p % MOD);
            pre[2][i] =
                dec(pre[2][i],
                    (unsigned long long)dec(pre[2][i / p], psum2) * p2 % MOD);
        }
    }
    primes.push_back(M + 1);
    for (int i = 1; i <= M; i++) {
        pre[2][i] = dec(pre[2][i], pre[1][i]);
        suc[2][i] = dec(suc[2][i], suc[1][i]);
    }
    return n > 1 ? 1 + rec(n, 0, 1) : 1;
}

int main() {
    std::cin >> n;
    std::cout << "1\n" << solve(n);
}