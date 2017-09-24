/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「51 NOD 1244」莫比乌斯函数之和 20-07-2017
 * 杜教筛
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace Task {

#define long long long
const int MAXN = 4641588 + 100;

int prime[MAXN], mu[MAXN], cnt;
long sum[MAXN];
bool vis[MAXN];

long blockSize, bound, sieveBlockSize;
const int MAX_BLOCK_SIZE = 100000 + 10;
long buc1[MAX_BLOCK_SIZE], buc2[MAX_BLOCK_SIZE];

inline void fastLinearSieveMethod(const int n) {
    mu[1] = 1;
    for (register int i = 2; i <= n; i++) {
        if (!vis[i]) prime[cnt++] = i, mu[i] = -1;
        for (register int j = 0, tmp; j < cnt && (tmp = i * prime[j]) <= n;
             j++) {
            vis[tmp] = true;
            if (i % prime[j] == 0) {
                mu[tmp] = 0;
                break;
            } else {
                mu[tmp] = -mu[i];
            }
        }
    }
    for (register int i = 1; i <= n; i++) sum[i] = sum[i - 1] + mu[i];
}

inline long &get(long x) { return x < blockSize ? buc1[x] : buc2[bound / x]; }

inline long sieveMain(long x) {
    if (x <= sieveBlockSize) return sum[x];
    register long &cur = get(x);
    if (cur != LLONG_MAX) return cur;
    register long ret = 1;
    for (register long i = 2, pos; i <= x; i = pos + 1)
        pos = x / (x / i), ret -= (pos - i + 1) * sieveMain(x / i);
    return cur = ret;
}

inline long sieve(long x) {
    blockSize = sqrt(x) + 1, bound = x;
    std::fill(buc1, buc1 + blockSize + 1, LLONG_MAX);
    std::fill(buc2, buc2 + blockSize + 1, LLONG_MAX);
    return sieveMain(x);
}

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register long l, r;
    std::cin >> l >> r;
    sieveBlockSize = pow(r, 2.0 / 3.0) + 1;
    fastLinearSieveMethod(sieveBlockSize);
    std::cout << sieve(r) - sieve(l - 1);
}
}

int main() {
    Task::solve();
    return 0;
}