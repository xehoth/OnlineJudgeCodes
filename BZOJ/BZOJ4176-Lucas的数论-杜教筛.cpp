/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4176」Lucas的数论 18-08-2017
 * 杜教筛
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace Task {

#define long long long

const int MAX_SIEVE_N = 1000000;
const int MOD = 1000000007;

int mu[MAX_SIEVE_N + 1], prime[MAX_SIEVE_N], cnt;
bool vis[MAX_SIEVE_N + 1];

inline void fastLinearSieveMethod(const int n) {
    mu[1] = 1;
    for (register int i = 2; i <= n; i++) {
        if (!vis[i]) prime[cnt++] = i, mu[i] = -1;
        for (register int j = 0, t; j < n && (t = i * prime[j]) <= n; j++) {
            vis[t] = true;
            if (i % prime[j] == 0) {
                mu[t] = 0;
                break;
            } else {
                mu[t] = -mu[i];
            }
        }
    }
    for (register int i = 2; i <= n; i++) mu[i] += mu[i - 1];
}

const int BLOCK_SIZE = 31623, BOUND = 1e9;
int buc1[BLOCK_SIZE + 1], buc2[BLOCK_SIZE + 1];

inline int &get(int x) { return x < BLOCK_SIZE ? buc1[x] : buc2[BOUND / x]; }

inline int sieve(int x) {
    if (x <= MAX_SIEVE_N) return mu[x];
    register int &cur = get(x);
    if (cur) return cur;
    register int ret = 1;
    for (register int i = 2, pos; i <= x; i = pos + 1) {
        pos = x / (x / i);
        ret = (ret - (pos - i + 1) * (long)sieve(x / i) % MOD + MOD) % MOD;
    }
    return cur = ret;
}

inline int getF(int x) {
    register int ret = 0;
    for (register int i = 1, pos; i <= x; i = pos + 1) {
        pos = x / (x / i);
        ret = (ret - (pos - i + 1ll) * (x / i) % MOD + MOD) % MOD;
    }
    return (long)ret * ret % MOD;
}

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register int n, ans = 0;
    std::cin >> n;
    fastLinearSieveMethod(MAX_SIEVE_N);
    for (register int i = 1, pos; i <= n; i = pos + 1) {
        pos = n / (n / i),
        ans = (ans +
               (sieve(pos) - sieve(i - 1) + (long)MOD) % MOD * getF(n / i) %
                   MOD) %
              MOD;
    }
    std::cout << ans;
}

#undef long
}

int main() {
    Task::solve();
    return 0;
}