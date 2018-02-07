/*
 * created by xehoth on 13-01-2017
 */
#include <bits/stdc++.h>

typedef long long ll;

int main() {
    int n, m;

    std::cin >> n >> m;
    ll sum = (ll)n * m;
    if (n > m) n = m;
    for (register int i = 1, j, l, r; i <= n; i = r + 1) {
        j = m / i, l = m / (j + 1) + 1, r = m / j;
        if (r >= n) r = n;
        sum -= (ll)(l + r) * (r - l + 1) * j >> 1;
    }
    std::cout << sum;
    return 0;
}