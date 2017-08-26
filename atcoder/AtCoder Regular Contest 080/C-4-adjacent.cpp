/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「ARC 080C」4-adjacent 24-08-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace Task {

const int MAXN = 200000;
int a[MAXN + 1];

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register int n;
    std::cin >> n;
    for (register int i = 1; i <= n; i++) std::cin >> a[i];
    register int cnt4 = 0, cnt2 = 0, cnt = 0;
    for (register int i = 1; i <= n; i++) {
        if (a[i] % 4 == 0) {
            cnt4++;
        } else if (a[i] % 2 == 0) {
            cnt2++;
        } else {
            cnt++;
        }
    }
    if (cnt2 == 0 && cnt <= cnt4 + 1) {
        std::cout << "Yes";
    } else if (cnt <= cnt4) {
        std::cout << "Yes";
    } else {
        std::cout << "No";
    }
}
}

int main() {
    Task::solve();
    return 0;
}