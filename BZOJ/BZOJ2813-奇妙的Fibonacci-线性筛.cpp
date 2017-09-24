/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2813」奇妙的 Fibonacci 09-07-2017
 * 线性筛
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace FastLinearSieveMethod {

#define long long long
const int MAXC = 664580, MAXN = 10000001, MOD = 1000000007;
int tot, prime[MAXC], idx[MAXN], d[MAXN], cnt[MAXN], sum[MAXN], ans1, ans2;
bool vis[MAXN];

inline void fastLinearSieveMethod(int C) {
    cnt[1] = sum[1] = 1;
    for (register int i = 2; i <= C; i++) {
        if (!vis[i]) {
            prime[tot++] = i, idx[i] = d[i] = 1;
            cnt[i] = 2, sum[i] = ((long)i * i + 1) % MOD;
        }
        for (register int j = 0, k; j < tot && (k = i * prime[j]) <= C; j++) {
            vis[k] = true;
            if (i % prime[j] == 0) {
                idx[k] = idx[i] + 1;
                cnt[k] = (cnt[i] / (idx[i] + 1)) * (idx[k] + 1), d[k] = d[i];
                sum[k] =
                    (sum[i] * ((long)prime[j] * prime[j] % MOD) + sum[d[i]]) %
                    MOD;
                break;
            } else {
                idx[k] = 1, d[k] = i, cnt[k] = cnt[i] << 1;
                sum[k] = sum[i] * (((long)prime[j] * prime[j] + 1) % MOD) % MOD;
            }
        }
    }
}

inline void solve() {
    int Q, q, A, B, C;
    scanf("%d%d%d%d%d", &Q, &q, &A, &B, &C);
    A %= C, B %= C;
    fastLinearSieveMethod(C);
    while (Q--) {
        ans1 += cnt[q] + (q & 1);
        ans2 += sum[q] + 4 * (q & 1);
        ans1 >= MOD ? ans1 -= MOD : 0;
        ans2 >= MOD ? ans2 -= MOD : 0;
        q = (q * (long)A + B) % C + 1;
    }
    printf("%d\n%d\n", ans1, ans2);
}
}

int main() {
    FastLinearSieveMethod::solve();
    return 0;
}