/*
 * created by xehoth on 30-04-2017
 */
#include <bits/stdc++.h>

#define long long long

const int MAXN = 55;

int n, p, k, r, f[MAXN], ans[MAXN], tmp[MAXN];

inline int get(int i) { return i >= k ? i - k : i; }

inline void mul(int *a, const int *b, const int *c) {
    memset(tmp, 0, sizeof(int) * k);
    for (register int i = 0; i < k; i++)
        for (register int j = 0; j < k; j++)
            tmp[get(i + j)] = (tmp[get(i + j)] + (long)b[i] * c[j]) % p;
    memcpy(a, tmp, sizeof(int) * k);
}

inline void modPow(int *a, long b) {
    for (; ~b & 1; b >>= 1, mul(a, a, a))
        ;
    for (; b; b >>= 1, mul(a, a, a)) (b & 1) ? (mul(ans, ans, a), 0) : 0;
}

int main() {
    std::ios::sync_with_stdio(false), std::cin >> n >> p >> k >> r;
    f[0] = ans[0] = 1, f[k == 1 ? 0 : 1]++;
    modPow(f, (long)n * k);
    std::cout << ans[r];
    return 0;
}
