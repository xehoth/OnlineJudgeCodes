/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 2001」Fibonacci 23-10-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

const int MAXN = 100;

long long f[MAXN + 1];

std::vector<long long> vc;

inline void solve() {
    f[0] = 0, f[1] = 1;
    for (register int i = 2; i < 47; i++) f[i] = f[i - 1] + f[i - 2];
    for (register int i = 0; i < 47; i++)
        for (register int j = 0; j < 47; j++) vc.push_back(f[i] * f[j]);
    std::sort(vc.begin(), vc.end()),
        vc.erase(std::unique(vc.begin(), vc.end()), vc.end());
    register int T;
    std::cin >> T;
    for (register long long x; T--;)
        std::cin >> x,
            std::cout << (std::binary_search(vc.begin(), vc.end(), x) ? "Yes\n"
                                                                      : "No\n");
}
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    solve();
    return 0;
}