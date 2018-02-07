#include <bits/stdc++.h>
#define MAXN 100010
#define MAXM 10000010
#define INF 1000000000000000000ll
#define MOD 1000000007
#define eps 1e-8
#define ll long long
int n;
int memp[MAXM];
ll cal(int *a, int n, bool flag, bool flag2) {
    ll i, j;
    ll re = INF;
    if (n == 1) {
        if (!a[1] && !flag) {
            return 1;
        }
        bool flag = 0;
        re = 0;
        for (i = 1; i <= 9; i++) {
            if (a[1] & (1 << i)) {
                flag = 1;
                re = i;
                a[1] ^= (1 << i);
                break;
            }
        }
        if (!flag && (a[1] & 1)) {
            re = 1;
        }
        for (i = 0; i <= 9; i++) {
            if (a[1] & (1 << i)) {
                re *= 10;
                re += i;
            }
        }
        return re;
    }
    for (i = 0; i <= 9; i++) {
        if (i == 9 && !flag2) {
            break;
        }
        int now = i;
        int N = 1;
        int *r = a + n;
        r[1] = 0;
        bool f = 0;
        for (j = 1; j <= n; j++) {
            r[N] |= a[j] ^ (a[j] & (1 << now));
            if (!now && (a[j] & 1)) {
                f = 1;
            }
            if (j != n && ++now == 10) {
                r[++N] = 0;
                now = 0;
            }
        }
        ll t = cal(a + n, N, flag || i, i != 9 || n > 2) * 10 + i;
        if (!t && f) {
            t = 10;
        }
        re = std::min(re, t);
    }
    return re;
}

int main() {
    ll i, x;
    scanf("%lld", &n);
    for (i = 1; i <= n; i++) {
        scanf("%lld", &x);
        memp[i] = 1 << x;
    }
    printf("%lld\n", cal(memp, n, 0, 1));
    return 0;
}