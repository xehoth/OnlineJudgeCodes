/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「51 NOD 1239」欧拉函数之和 23-01-2018
 * Extended Eratosthenes Sieve
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MOD = 1000000007;

long long n;
int M;
std::vector<int> pre[2], suc[2], primes;

inline int dec(const int x, const int v) {
    return x - v < 0 ? x - v + MOD : x - v;
}
inline int add(const int x, const int v) {
    return x + v >= MOD ? x + v - MOD : x + v;
}

inline int rec(long long res, int last, int mul) {
    int t = dec((res > M ? suc[1][n / res] : pre[1][res]),
                pre[1][primes[last] - 1]);
    int ret = (long long)mul * t % MOD;
    for (int i = last, p; i < (int)primes.size(); i++) {
        p = primes[i];
        if ((long long)p * p > res) break;
        for (long long q = p, nres = res, nmul = mul * (p - 1ll) % MOD;
             p * q <= res; q *= p) {
            ret = add(ret, rec(nres /= p, i + 1, nmul));
            nmul = nmul * p % MOD;
            ret = add(ret, nmul);
        }
    }
    return ret;
}

inline int solve(const long long n) {
    M = sqrt(n);
    pre[0].clear();
    pre[1].clear();
    suc[0].clear();
    suc[1].clear();
    primes.clear();
    pre[0].resize(M + 1);
    pre[1].resize(M + 1);
    suc[0].resize(M + 1);
    suc[1].resize(M + 1);
    for (int i = 1, t; i <= M; i++) {
        pre[0][i] = i - 1;
        pre[1][i] = ((i * (i + 1ll)) / 2 - 1 + MOD) % MOD;
        t = (n / i) % MOD;
        suc[0][i] = t - 1;
        suc[1][i] = ((t * (t + 1ll)) / 2 - 1 + MOD) % MOD;
    }
    for (int p = 2, end; p <= M; p++) {
        if (pre[0][p] == pre[0][p - 1]) continue;
        primes.push_back(p);
        const long long q = (long long)p * p, m = n / p;
        const int pcnt = pre[0][p - 1], psum = pre[1][p - 1];
        end = std::min<long long>(M, n / q);
        for (int i = 1, w = M / p; i <= w; i++) {
            suc[0][i] = dec(suc[0][i], dec(suc[0][i * p], pcnt));
            suc[1][i] =
                dec(suc[1][i], dec(suc[1][i * p], psum) * (long long)p % MOD);
        }
        for (int i = M / p + 1; i <= end; i++) {
            suc[0][i] = dec(suc[0][i], dec(pre[0][m / i], pcnt));
            suc[1][i] =
                dec(suc[1][i], dec(pre[1][m / i], psum) * (long long)p % MOD);
        }
        for (int i = M; i >= q; i--) {
            pre[0][i] = dec(pre[0][i], dec(pre[0][i / p], pcnt));
            pre[1][i] =
                dec(pre[1][i], dec(pre[1][i / p], psum) * (long long)p % MOD);
        }
    }
    primes.push_back(M + 1);
    for (int i = 1; i <= M; i++) {
        pre[1][i] = dec(pre[1][i], pre[0][i]);
        suc[1][i] = dec(suc[1][i], suc[0][i]);
    }
    return n > 1 ? add(1, rec(n, 0, 1)) : 1;
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    std::cin >> n;
    std::cout << solve(n);
    return 0;
}