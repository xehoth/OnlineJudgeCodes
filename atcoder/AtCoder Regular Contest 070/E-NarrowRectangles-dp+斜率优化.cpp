/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「ARC 070E」NarrowRectangles 20-08-2017
 * dp + 斜率优化
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace Task {

const int MAXN = 100000;
#define long long long

std::multiset<long> setL, setR;
int a[MAXN + 1], b[MAXN + 1];
long f[MAXN + 1];

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register int n;
    std::cin >> n;
    for (register int i = 1; i <= n; i++) std::cin >> a[i] >> b[i];
    setL.insert(a[1]), setR.insert(a[1]);
    register long tl = 0, tr = 0, ans = 0, nl, nr;
    for (register int i = 2; i <= n; i++) {
        tl += b[i] - a[i], tr += b[i - 1] - a[i - 1];
        nl = *setL.rbegin(), nr = *setR.begin();
        if (nl - tl > a[i]) {
            ans += std::abs(nl - tl - a[i]), setL.erase(--setL.end());
            setL.insert(setL.insert(a[i] + tl), a[i] + tl);
            setR.insert(nl - tl - tr);
        } else if (a[i] > nr + tr) {
            ans += std::abs(nr + tr - a[i]), setR.erase(setR.begin());
            setR.insert(setR.insert(a[i] - tr), a[i] - tr);
            setL.insert(nr + tr + tl);
        } else {
            setL.insert(a[i] + tl), setR.insert(a[i] - tr);
        }
    }
    std::cout << ans;
}

#undef long
}

int main() {
    Task::solve();
    return 0;
}