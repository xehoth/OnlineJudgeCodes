#include <bits/stdc++.h>

const int MAXN = 100000;
const int MOD = 1e9 + 7;

int a[MAXN], b[MAXN], c[MAXN];

int f(int i, int j, int x) {
    return c[i] < j ? ((unsigned long long)a[i] * x + b[i]) % MOD
                    : ((unsigned long long)a[i] * f(c[i], j, x) + b[i]) % MOD;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    for (int i = 1; i <= n; i++) std::cin >> a[i] >> b[i] >> c[i];
    for (int i, j, x; m--;) {
        std::cin >> i >> j >> x;
        std::cout << f(i, j, x) << '\n';
    }
    return 0;
}