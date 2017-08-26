/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「ARC 081D」Coloring Dominoes 21-08-2017
 * dp
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace Task {

const int MAXN = 52;
const int MOD = 1e9 + 7;

char s[2][MAXN];

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register int n;
    std::cin >> n >> s[0] >> s[1];
    register int ans, tot = 0;
    if (s[0][0] == s[1][0])
        ans = 3, tot = 1;
    else if (s[0][0] == s[0][1])
        ans = 6, tot = 2;
    for (; tot < n;) {
        if (s[0][tot] == s[1][tot]) {
            if (s[0][tot - 1] == s[1][tot - 1]) ans = ans * 2ll % MOD;
            tot++;
        } else if (s[0][tot] == s[0][tot + 1]) {
            if (s[0][tot - 1] == s[1][tot - 1])
                ans = ans * 2ll % MOD;
            else if (s[0][tot - 1] == s[0][tot - 2])
                ans = ans * 3ll % MOD;
            tot += 2;
        }
    }
    std::cout << ans;
}
}

int main() {
    Task::solve();
    return 0;
}