/*
 * created by xehoth on 13-01-2017
 */
#include <bits/stdc++.h>

typedef long long ll;

inline ll getPhi(ll x) {
    register ll ret = x;
    for (register ll i = 2; i * i <= x; i++) {
        if (x % i == 0) {
            ret = ret / i * (i - 1);
            for (; x % i == 0; x /= i)
                ;
        }
    }
    if (x != 1) ret = ret / x * (x - 1);
    return ret;
}

int main() {
    ll n, m, ans = 0;

    std::cin >> n;
    m = sqrt(n);
    for (register int i = 1; i <= m; i++) {
        if (n % i == 0) {
            ans += (ll)i * getPhi(n / i);
            if (i * i < n) ans += (ll)(n / i) * getPhi(i);
        }
    }
    std::cout << ans;
    return 0;
}