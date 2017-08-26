/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「51 NOD 1239」欧拉函数之和 20-07-2017
 * 杜教筛
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace Task {

#define long long long
const int MAXN = 4641588 + 100;
const int MOD = 1000000007;

int prime[MAXN], phi[MAXN], cnt, sum[MAXN];
bool vis[MAXN];

inline void fastLinearSieveMethod(const int n) {
    phi[1] = 1;
    for (register int i = 2; i <= n; i++) {
        if (!vis[i]) prime[cnt++] = i, phi[i] = i - 1;
        for (register int j = 0, tmp; j < cnt && (tmp = i * prime[j]) <= n;
             j++) {
            vis[tmp] = true;
            if (i % prime[j] == 0) {
                phi[tmp] = phi[i] * prime[j];
                break;
            } else {
                phi[tmp] = phi[i] * (prime[j] - 1);
            }
        }
    }
    for (register int i = 1; i <= n; i++) sum[i] = (sum[i - 1] + phi[i]) % MOD;
}

const int MAX_BLOCK_SIZE = 100000 + 10;

long blockSize, sieveBlockSize, bound;

int buc1[MAX_BLOCK_SIZE], buc2[MAX_BLOCK_SIZE];
const int INV_TWO = 500000004;

inline int &get(long x) { return x < blockSize ? buc1[x] : buc2[bound / x]; }

inline int sieveMain(long x) {
    if (x <= sieveBlockSize) return sum[x];
    register int &cur = get(x);
    if (cur != INT_MAX) return cur;
    register int ret = ((x % MOD) * ((x + 1) % MOD) % MOD) * INV_TWO % MOD;
    for (register long i = 2, pos; i <= x; i = pos + 1)
        pos = x / (x / i),
        ret = (ret - (pos - i + 1) * (long)sieveMain(x / i) % MOD + MOD) % MOD;
    ret = (ret % MOD + MOD) % MOD;
    return cur = ret;
}

inline int sieve(long x) {
    blockSize = sqrt(x) + 1, bound = x;
    std::fill(buc1, buc1 + blockSize + 1, INT_MAX);
    std::fill(buc2, buc2 + blockSize + 1, INT_MAX);
    return sieveMain(x);
}

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register long n;
    std::cin >> n;
    sieveBlockSize = pow(n, 2.0 / 3.0) + 1;
    fastLinearSieveMethod(sieveBlockSize);
    std::cout << sieve(n) << "\n";
}
#undef long
}

int main() {
    Task::solve();
    return 0;
}