/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「ARC 081C」Make a Rectangle 21-08-2017
 * 贪心
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace Task {

#define long long long

std::map<int, int> map;

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register int n, cnt = 2;
    register long ans = 1;
    std::cin >> n;
    for (register int i = 0, t; i < n; i++) std::cin >> t, map[t]++;
    for (auto it = map.rbegin(); it != map.rend(); it++) {
        if (it->second > 1) {
            ans *= it->first, cnt--;
            if (it->second > 3 && cnt == 1) ans *= it->first, cnt--;
        }
        if (cnt == 0) break;
    }
    if (cnt == 0)
        std::cout << ans;
    else
        std::cout << '0';
}

#undef long
}

int main() {
    Task::solve();
    return 0;
}