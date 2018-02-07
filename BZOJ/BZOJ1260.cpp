#include <bits/stdc++.h>

namespace {

const int MAXN = 1010;

int f[MAXN][MAXN];
char s[MAXN];

int dp(int l, int r) {
    if (l >= r) return l == r;
    register int &ret = f[l][r];
    if (ret) return ret;
    ret = r - l + 1;
    for (register int i = l; i < r; i++)
        ret = std::min(ret, dp(l, i) + dp(i + 1, r));
    if (s[l] == s[r])
        ret = std::min(ret, std::min(std::min(dp(l + 1, r), dp(l, r - 1)),
                                     dp(l + 1, r - 1) + 1));
    return ret;
}

inline void solve() {
    std::cin >> s + 1;
    std::cout << dp(1, strlen(s + 1));
}
}  // namespace

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    solve();
    return 0;
}