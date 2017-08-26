/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「ARC 079D」Decrease (Contestant ver.) 26-08-2017
 * 构造
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

typedef unsigned long long ulong;

const int MAXN = 50;
ulong a[MAXN];

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register ulong n;
    std::cin >> n;
    register ulong steps = n / MAXN, remain = n % MAXN;
    for (register int i = 0; i < MAXN; i++) a[i] = i + steps;
    for (register int i = 0; i < remain; i++) {
        a[i] += MAXN;
        for (register int j = 0; j < i; j++) a[j]--;
        for (register int j = i + 1; j < MAXN; j++) a[j]--;
    }
    std::cout << "50\n";
    for (register int i = 0; i < MAXN; i++) std::cout << a[i] << ' ';
}
}

int main() {
    solve();
    return 0;
}