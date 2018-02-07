#include <bits/stdc++.h>
#include <tr1/unordered_map>

std::tr1::unordered_map<long long, int> g;
int T;
long long n, f[100];

int solve(register long long n) {
    register int &ret = g[n];
    if (ret) return ret;
    register int p = std::lower_bound(f, f + 90, n) - f;
    if (f[p] == n) return 1;
    return ret = std::min(solve(n - f[p - 1]), solve(f[p] - n)) + 1;
}

int main() {
    f[0] = 1, f[1] = 1;
    std::cin >> T;
    for (register int i = 2; i <= 90; i++) f[i] = f[i - 1] + f[i - 2];
    while (T--) std::cin >> n, std::cout << solve(n) << '\n';
    return 0;
}