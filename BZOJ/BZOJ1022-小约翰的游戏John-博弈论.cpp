/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1022」小约翰的游戏John 06-03-2017
 * 博弈论
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 50;

int main() {
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
#endif
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    register int t;
    std::cin >> t;
    while (t--) {
        register int n;
        std::cin >> n;
        register int s = 0;
        register bool flag = false;
        while (n--) {
            register int x;
            std::cin >> x;
            s ^= x;
            if (x > 1) flag = true;
        }
        std::cout << ((!flag && s == 0) || (flag && s != 0) ? "John\n"
                                                            : "Brother\n");
    }
    return 0;
}