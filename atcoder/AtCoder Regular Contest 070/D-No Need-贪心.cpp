/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「ARC 070D」No Need 18-08-2017
 * 贪心
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace Task {

const int MAXN = 5000;

int a[MAXN + 1];

inline void solve() {
    register int n, k, sum = 0, pos;
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    std::cin >> n >> k, pos = n;
    for (register int i = 0; i < n; i++) std::cin >> a[i];
    std::sort(a, a + n);
    for (register int i = n - 1; i >= 0; i--) {
        if (sum + a[i] < k)
            sum += a[i];
        else
            pos = i;
    }
    std::cout << pos;
}
}

int main() {
    Task::solve();
    return 0;
}