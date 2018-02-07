/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1053」反素数ant 24-03-2017
 * 搜索
 * @author xehoth
 */
#include <cstdio>

const int MAXN = 2e9;

const int PRIMES[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31};
const int PRIMES_CNT = 11;

int n, cntAns;
long long ans;

inline void search(int i, long long x, int cnt) {
    if (i == PRIMES_CNT) {
        if ((cnt == cntAns && x < ans) || (cnt > cntAns)) {
            ans = x;
            cntAns = cnt;
        }
        return;
    }

    long long t = 1;
    for (int j = 0; x * t <= n; j++) {
        search(i + 1, x * t, cnt * (j + 1));
        t *= PRIMES[i];
    }
}

int main() {
    scanf("%d", &n);
    search(0, 1, 1);
    printf("%lld\n", ans);

    return 0;
}