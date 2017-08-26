/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「ARC 079E」Decrease (Judge ver.) 26-08-2017
 * 贪心
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

typedef unsigned long long ulong;

const int MAXN = 50;
int n;
ulong a[MAXN];

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    std::cin >> n;
    for (register int i = 0; i < n; i++) std::cin >> a[i];
    register ulong ans = 0, cnt, *p;
    for (; *(p = std::max_element(a, a + n)) >= n;) {
        cnt = *p / n, *p %= n, ans += cnt;
        for (register ulong *i = a; i < p; i++) *i += cnt;
        for (register ulong *i = p + 1; i < a + n; i++) *i += cnt;
    }
    std::cout << ans;
}
}

int main() {
    solve();
    return 0;
}