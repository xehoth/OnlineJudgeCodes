/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「HDU 5608」function 22-07-2017
 * 莫比乌斯反演 + 杜教筛
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace Task {

#define long long long

const int MOD = 1e9 + 7;
const int MAXN = 1000000 + 10;
const int INV_THREE = 333333336;

int sieveBlockSize, blockSize, bound;
int buc1[MAXN], buc2[MAXN], f[MAXN], mu[MAXN], cnt, prime[MAXN];
bool vis[MAXN];

inline int &get(int x) { return x < blockSize ? buc1[x] : buc2[bound / x]; }

inline int g(int x) { return (long)(x - 1) * (x - 2) % MOD; }

inline void fastLinearSieveMathod(const int n) {
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
    for (register int i = 1; i <= n; i++)
        for (register int j = i; j <= n; j += i)
            f[j] = (f[j] + (long)g(i) * mu[j / i] % MOD + MOD) % MOD;
    for (register int i = 1; i <= n; i++) f[i] = (f[i] + f[i - 1]) % MOD;
}

inline int sieveMain(int x) {
    if (x <= sieveBlockSize) return f[x];
    register int &cur = get(x);
    if (cur != INT_MAX) return cur;
    register int ret =
        (long)x * (x - 1) % MOD * (long)(x - 2) % MOD * INV_THREE % MOD;
    for (register int i = 2, pos; i <= x; i = pos + 1)
        pos = x / (x / i),
        ret = (ret - (long)(pos - i + 1) * sieveMain(x / i) % MOD + MOD) % MOD;
    return cur = ret;
}

inline int sieve(int x) {
    blockSize = sqrt(x) + 1, bound = x;
    std::fill(buc1, buc1 + blockSize + 1, INT_MAX);
    std::fill(buc2, buc2 + blockSize + 1, INT_MAX);
    return sieveMain(x);
}

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    sieveBlockSize = 1000000, fastLinearSieveMathod(sieveBlockSize);
    register int T, n;
    std::cin >> T;
    while (T--) std::cin >> n, std::cout << sieve(n) << '\n';
}

#undef long
}

int main() {
    Task::solve();
    return 0;
}