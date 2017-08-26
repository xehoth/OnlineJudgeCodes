/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4805」欧拉函数求和 18-08-2017
 * 杜教筛
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace Task {

#define long long long
const int MAXN = 1587402 + 1;

int prime[MAXN], cnt;
long phi[MAXN];
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
    for (register int i = 1; i <= n; i++) phi[i] += phi[i - 1];
}

const int MAX_BLOCK_SIZE = 100000 + 10;
const int blockSize = 44722, sieveBlockSize = 1587402, bound = 2e9;
long buc1[MAX_BLOCK_SIZE], buc2[MAX_BLOCK_SIZE];
const int INV_TWO = 500000004;

inline long &get(int x) { return x < blockSize ? buc1[x] : buc2[bound / x]; }

inline long sieve(int x) {
    if (x <= sieveBlockSize) return phi[x];
    register long &cur = get(x);
    if (cur) return cur;
    register long ret = (x * (x + 1ll)) / 2;
    for (register int i = 2, pos; i <= x; i = pos + 1)
        pos = x / (x / i), ret -= (pos - i + 1) * (long)sieve(x / i);
    return cur = ret;
}

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register int n;
    std::cin >> n;
    fastLinearSieveMethod(sieveBlockSize);
    std::cout << sieve(n) << "\n";
}
#undef long
}

int main() {
    Task::solve();
    return 0;
}