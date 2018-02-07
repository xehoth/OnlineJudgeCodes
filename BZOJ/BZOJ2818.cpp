/*
 * created by xehoth on 13-01-2017
 */
#include <bits/stdc++.h>

const int MAXN = 10000001;

long long phi[MAXN + 10], prime[MAXN + 10];
short mu[MAXN + 10];
int tot;
bool vis[MAXN];

inline void fastLinearSieveMethod(int n) {
    mu[1] = phi[1] = 1;
    for (register int i = 2; i <= n; i++) {
        if (!vis[i]) prime[++tot] = i, phi[i] = i - 1, mu[i] = -1;
        for (register int j = 1, tmp; j <= tot && (tmp = i * prime[j]) <= n;
             j++) {
            vis[tmp] = true;
            if (i % prime[j] == 0) {
                phi[tmp] = phi[i] * prime[j], mu[tmp] = 0;
                break;
            } else {
                phi[tmp] = phi[i] * (prime[j] - 1);
                mu[tmp] = -mu[i];
            }
        }
    }
}

int main() {
    // freopen("in.in", "r", stdin);
    int n;
    scanf("%d", &n);
    fastLinearSieveMethod(n);

    for (register int i = 1; i <= n; i++) phi[i] += phi[i - 1];
    long long ans = 0;
    for (register int i = 1; i <= tot; i++) ans += (phi[n / prime[i]] << 1) - 1;
    std::cout << ans;
    return 0;
}