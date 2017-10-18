/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「UVA 11552」Fewest Flops 18-10-2017
 * DP
 * @author xehoth
 */
#include <bits/extc++.h>

namespace {

const int MAXN = 1000;

char s[MAXN + 1];
int f[MAXN + 1][MAXN + 1];

inline void solveCase() {
    register int k, len, block;
    std::cin >> k >> s;
    len = strlen(s);
    block = len / k;
    for (register int i = 0; i < block; i++)
        memset(f[i], 0x3f, sizeof(int) * k);
    static std::bitset<127> vis;

    for (register int i = 0, chunks; i < block; i++) {
        chunks = 0, vis.reset();
        for (register int j = i * k; j < (i + 1) * k; j++) vis.set(s[j]);
        for (register int j = 'a'; j <= 'z'; j++) chunks += vis.test(j);
        if (i == 0) {
            for (register int j = 0; j < k; j++) f[i][j] = chunks;
            continue;
        }
        for (register int end = i * k, j = 0; j < k; end++, j++) {
            for (register int pre = (i - 1) * k, l = 0; l < k; pre++, l++) {
                if (vis.test(s[pre]) && (chunks == 1 || s[pre] != s[end])) {
                    f[i][j] = std::min(f[i][j], f[i - 1][l] + chunks - 1);
                } else {
                    f[i][j] = std::min(f[i][j], f[i - 1][l] + chunks);
                }
            }
        }
    }
    std::cout << *std::min_element(f[len / k - 1], f[len / k - 1] + k) << '\n';
}

inline void solve() {
    register int T;
    for (std::cin >> T; T--;) solveCase();
}
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    solve();
    return 0;
}