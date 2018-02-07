#include <bits/stdc++.h>

const int MAXN = 40010;
int prime[MAXN], phi[MAXN], cnt;
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
}

int main() {
    register int n, ans = 0;
    scanf("%d", &n);
    fastLinearSieveMethod(n);
    for (register int i = 1; i < n; i++) ans += phi[i];
    printf("%d", ans << 1 | 1);
    return 0;
}