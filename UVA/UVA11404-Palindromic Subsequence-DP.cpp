/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「UVA 11404」Palindromic Subsequence 18-10-2017
 * DP
 * @author xehoth
 */
#include <bits/extc++.h>

namespace {

typedef __gnu_cxx::__rc_string String;

const int MAXN = 1000;

char s[MAXN + 1], revs[MAXN + 1];

int f[MAXN + 1][MAXN + 1];
String str[MAXN + 1][MAXN + 1];

inline void solveCase() {
    register int len = strlen(s + 1);
    std::reverse_copy(s + 1, s + len + 1, revs + 1);
    for (register int i = 0; i <= len; i++) f[0][i] = 0, str[0][i].clear();
    for (register int i = 1; i <= len; i++) {
        for (register int j = 1; j <= len; j++) {
            if (s[i] == revs[j]) {
                f[i][j] = f[i - 1][j - 1] + 1;
                str[i][j] = str[i - 1][j - 1] + s[i];
            } else {
                if (f[i - 1][j] > f[i][j - 1]) {
                    f[i][j] = f[i - 1][j], str[i][j] = str[i - 1][j];
                } else if (f[i - 1][j] < f[i][j - 1]) {
                    f[i][j] = f[i][j - 1], str[i][j] = str[i][j - 1];
                } else {
                    f[i][j] = f[i - 1][j];
                    str[i][j] = std::min(str[i - 1][j], str[i][j - 1]);
                }
            }
        }
    }
    register int maxLen = f[len][len];
    const String &ans = str[len][len];
    if (maxLen & 1) {
        for (register int i = 0; i < maxLen / 2; i++) std::cout << ans[i];
        for (register int i = maxLen / 2; i >= 0; i--) std::cout << ans[i];
        std::cout << '\n';
    } else {
        for (register int i = 0; i < maxLen / 2; i++) std::cout << ans[i];
        for (register int i = maxLen / 2 - 1; i >= 0; i--) std::cout << ans[i];
        std::cout << '\n';
    }
}

inline void solve() {
    while (std::cin >> (s + 1)) solveCase();
}
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    solve();
    return 0;
}