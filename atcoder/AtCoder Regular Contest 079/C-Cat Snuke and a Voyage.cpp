/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「ARC 079C」Cat Snuke and a Voyage 26-08-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

const int MAXN = 200000;
bool vis[MAXN + 1];

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register int n, m, u, v;
    for (std::cin >> n >> m; m--;) {
        std::cin >> u >> v;
        if (u == 1) {
            if (vis[v]) std::cout << "POSSIBLE", exit(0);
            vis[v] = true;
        } else if (v == n) {
            if (vis[u]) std::cout << "POSSIBLE", exit(0);
            vis[u] = true;
        }
    }
    std::cout << "IMPOSSIBLE";
}
}

int main() {
    solve();
    return 0;
}