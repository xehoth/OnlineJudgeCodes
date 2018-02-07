/*
 * created by xehoth on 05-04-2017
 */
#include <bits/stdc++.h>

int t[10000][600], a[10005], n, K, ans, max, tmp;

inline int query(int x, int y) {
    register int sum = 0;
    for (register int i = x; i; i ^= i & -i)
        for (register int j = y; j; j ^= j & -j) sum = std::max(t[i][j], sum);
    return sum;
}

inline void modify(int x, int y, int k) {
    for (register int i = x; i <= max + K; i += i & -i)
        for (register int j = y; j <= 1 + K; j += j & -j)
            t[i][j] = std::max(t[i][j], k);
}

int main() {
    scanf("%d%d", &n, &K);
    for (register int i = 1; i <= n; i++)
        scanf("%d", &a[i]), max = std::max(max, a[i]);
    for (register int i = 1; i <= n; i++) {
        for (register int j = K; ~j; j--) {
            tmp = query(a[i] + j, j + 1) + 1;
            ans = std::max(ans, tmp);
            modify(a[i] + j, j + 1, tmp);
        }
    }
    std::cout << ans;
    return 0;
}