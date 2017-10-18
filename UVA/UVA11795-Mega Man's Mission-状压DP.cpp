/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「UVA 11795」Mega Man's Mission 18-10-2017
 * 状压 DP
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

#define long long long

const int MAXN = 16;
const int MAXM = 1 << MAXN;
char s[MAXN + 1];
int weapons[MAXN + 1], init, n;
std::bitset<MAXM + 1> vis;
long f[MAXM + 1];

inline long dp(const long s) {
    if (s == 0) return 1;
    register long &ret = f[s];
    if (vis.test(s)) return ret;
    vis.set(s);
    register int able = init;
    for (register int i = 0; i < n; i++)
        if (((s >> i) & 1) == 0) able |= weapons[i];
    for (register int i = 0; i < n; i++)
        if (((s >> i) & 1) && ((able >> i) & 1)) ret += dp(s ^ (1 << i));
    return ret;
}

inline void solveCase() {
    std::cin >> n >> s, init = 0;
    memset(f, 0, sizeof(long) * (1 << n));
    vis.reset();
    for (register int i = 0; i < n; i++) init |= (s[i] - '0') << i;
    for (register int i = 0; i < n; i++) {
        std::cin >> s, weapons[i] = 0;
        for (register int j = 0; j < n; j++) weapons[i] |= (s[j] - '0') << j;
    }
    std::cout << dp((1 << n) - 1) << '\n';
}

inline void solve() {
    register int T;
    std::cin >> T;
    for (register int i = 1; i <= T; i++) {
        std::cout << "Case " << i << ": ";
        solveCase();
    }
}
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    solve();
    return 0;
}