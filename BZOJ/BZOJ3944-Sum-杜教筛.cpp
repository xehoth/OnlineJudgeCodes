/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3944」Sum 21-07-2017
 * 杜教筛
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace Task {

#define long long long
const int MAXN = 2000000 + 100;

int prime[MAXN], mu[MAXN], phi[MAXN], cnt;
long muSum[MAXN], phiSum[MAXN];
bool vis[MAXN];

long blockSize, bound, sieveBlockSize;
const int MAX_BLOCK_SIZE = 46341 + 10;
long buc1[MAX_BLOCK_SIZE], buc2[MAX_BLOCK_SIZE];

inline void fastLinearSieveMethod(const int n) {
    mu[1] = 1, phi[1] = 1;
    for (register int i = 2; i <= n; i++) {
        if (!vis[i]) prime[cnt++] = i, mu[i] = -1, phi[i] = i - 1;
        for (register int j = 0, tmp; j < cnt && (tmp = i * prime[j]) <= n;
             j++) {
            vis[tmp] = true;
            if (i % prime[j] == 0) {
                mu[tmp] = 0, phi[tmp] = phi[i] * prime[j];
                break;
            } else {
                mu[tmp] = -mu[i], phi[tmp] = phi[i] * (prime[j] - 1);
            }
        }
    }
    for (register int i = 1; i <= n; i++) muSum[i] = muSum[i - 1] + mu[i];
    for (register int i = 1; i <= n; i++) phiSum[i] = phiSum[i - 1] + phi[i];
}

inline long &get(long x) { return x < blockSize ? buc1[x] : buc2[bound / x]; }

inline long sieveMuMain(long x) {
    if (x <= sieveBlockSize) return muSum[x];
    register long &cur = get(x);
    if (cur != LLONG_MAX) return cur;
    register long ret = 1;
    for (register long i = 2, pos; i <= x; i = pos + 1)
        pos = x / (x / i), ret -= (pos - i + 1) * sieveMuMain(x / i);
    return cur = ret;
}

inline long sievePhiMain(long x) {
    if (x <= sieveBlockSize) return phiSum[x];
    register long &cur = get(x);
    if (cur != LLONG_MAX) return cur;
    register long ret = x * (x + 1) >> 1;
    for (register long i = 2, pos; i <= x; i = pos + 1)
        pos = x / (x / i), ret -= (pos - i + 1) * sievePhiMain(x / i);
    return cur = ret;
}

inline void initSieve(int x) {
    blockSize = sqrt(x) + 1, bound = x;
    std::fill(buc1, buc1 + blockSize + 1, LLONG_MAX);
    std::fill(buc2, buc2 + blockSize + 1, LLONG_MAX);
}

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    sieveBlockSize = 2000000 + 1;
    fastLinearSieveMethod(sieveBlockSize);
    register int T;
    std::cin >> T;
    while (T--) {
        register long n;
        std::cin >> n;
        initSieve(n);
        std::cout << sievePhiMain(n) << ' ';
        initSieve(n);
        std::cout << sieveMuMain(n) << '\n';
    }
}
}

int main() {
    Task::solve();
    return 0;
}