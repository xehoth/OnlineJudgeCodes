/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「SPOJ DIVCNT3」23-01-2018
 * Extended Eratosthenes Sieve
 * @author xehoth
 */
#include <bits/stdc++.h>

std::vector<long long> pre, suc;
std::vector<int> primes;
int M;
long long n;

long long rec(long long res, int last, long long mul) {
    long long t = (res > M ? suc[n / res] : pre[res]) - pre[primes[last] - 1];
    long long ret = mul * t * 4;
    for (int i = last, p; i < (int)primes.size(); i++) {
        p = primes[i];
        if ((long long)p * p > res) break;
        for (long long q = p, nrest = res, nmul = mul * 4; q * p <= res;
             q *= p) {
            ret += rec(nrest /= p, i + 1, nmul);
            nmul += mul * 3;
            ret += nmul;
        }
    }
    return ret;
}

inline long long extEratosthenesSieve(const long long n) {
    M = sqrt(n);
    pre.clear();
    suc.clear();
    primes.clear();
    pre.resize(M + 1);
    suc.resize(M + 1);
    for (int i = 1; i <= M; i++) {
        pre[i] = i - 1;
        suc[i] = n / i - 1;
    }
    for (int p = 2, end; p <= M; p++) {
        if (pre[p] == pre[p - 1]) continue;
        primes.push_back(p);
        const long long pcnt = pre[p - 1], q = (long long)p * p, m = n / p;
        end = std::min<long long>(M, n / q);
        for (int i = 1, w = M / p; i <= w; i++) suc[i] -= suc[i * p] - pcnt;
        for (int i = M / p + 1; i <= end; i++) suc[i] -= pre[m / i] - pcnt;
        for (int i = M; i >= q; i--) pre[i] -= pre[i / p] - pcnt;
    }
    primes.push_back(M + 1);
    return n > 1 ? 1 + rec(n, 0, 1) : 1;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    int T;
    for (std::cin >> T; T--;) {
        std::cin >> n;
        std::cout << extEratosthenesSieve(n) << '\n';
    }
    return 0;
}