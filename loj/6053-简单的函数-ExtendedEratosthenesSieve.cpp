/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「LOJ 6053」简单的函数 24-01-2018
 * Extended Eratosthenes Sieve
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

const int MOD = 1e9 + 7;
typedef unsigned int uint;

typedef unsigned long long ulong;

long long n;
uint M;

inline int add(const int x, const int v) {
    return x + v >= MOD ? x + v - MOD : x + v;
}

inline int dec(const int x, const int v) {
    return x - v < 0 ? x - v + MOD : x - v;
}

std::vector<int> pre[2], suc[2], primes;

int rec(ulong res, int last, int mul) {
    int t =
        dec(res > M ? suc[1][n / res] : pre[1][res], pre[1][primes[last] - 1]);
    int ret = (ulong)t * mul % MOD;
    for (int i = last, p; i < (int)primes.size(); i++) {
        p = primes[i];
        if ((ulong)p * p > res) break;
        ulong q = p, nres = res;
        for (int nmul, e = 1; q * p <= res; q *= p) {
            nmul = (ulong)mul * (p ^ e) % MOD;
            ret = add(ret, rec(nres /= p, i + 1, nmul));
            ret = add(ret, (ulong)mul * (p ^ (++e)) % MOD);
        }
    }
    return ret;
}

inline int solve(const ulong n) {
    M = sqrt(n);
    pre[0].resize(M + 1);
    pre[1].resize(M + 1);
    suc[0].resize(M + 1);
    suc[1].resize(M + 1);
    primes.reserve(M + 1);
    for (uint i = 1; i <= M; i++) {
        pre[0][i] = i - 1;
        pre[1][i] = (i * (i + 1ull) / 2 - 1) % MOD;
        const ulong t = (n / i) % MOD;
        suc[0][i] = t - 1;
        suc[1][i] = (t * (t + 1ull) / 2 - 1) % MOD;
    }
    for (uint p = 2, end; p <= M; p++) {
        if (pre[0][p] == pre[0][p - 1]) continue;
        primes.push_back(p);
        const ulong q = (ulong)p * p, m = n / p;
        const int pcnt = pre[0][p - 1], psum = pre[1][p - 1];
        end = std::min<long long>(M, n / q);
        for (uint i = 1, w = M / p; i <= w; i++) {
            suc[0][i] = dec(suc[0][i], dec(suc[0][i * p], pcnt));
            suc[1][i] =
                dec(suc[1][i], dec(suc[1][i * p], psum) * (ulong)p % MOD);
        }
        for (uint i = M / p + 1; i <= end; i++) {
            suc[0][i] = dec(suc[0][i], dec(pre[0][m / i], pcnt));
            suc[1][i] =
                dec(suc[1][i], dec(pre[1][m / i], psum) * (ulong)p % MOD);
        }
        for (uint i = M; i >= q; i--) {
            pre[0][i] = dec(pre[0][i], dec(pre[0][i / p], pcnt));
            pre[1][i] =
                dec(pre[1][i], dec(pre[1][i / p], psum) * (ulong)p % MOD);
        }
    }
    primes.push_back(M + 1);
    for (uint i = 1; i <= M; i++) {
        pre[1][i] = (i >= 2 ? add(2, dec(pre[1][i], pre[0][i]))
                            : dec(pre[1][i], pre[0][i]));
        suc[1][i] = (n >= i * 2 ? add(2, dec(suc[1][i], suc[0][i]))
                                : dec(suc[1][i], suc[0][i]));
    }

    return n > 1 ? 1 + rec(n, 0, 1) : 1;
}
}  // namespace

int main() {
    std::cin >> n;
    std::cout << solve(n);
}