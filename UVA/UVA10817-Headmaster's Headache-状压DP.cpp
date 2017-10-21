/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「UVA 10817」Headmaster's Headache 21-10-2017
 * 状压 DP
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

const int MAXN = 150;
const int INF = 0x3f3f3f3f;

int s, n, m, c[MAXN + 1], cnt[10], p[MAXN + 1], f[1 << 8][1 << 8];

inline void solveCase() {
    static char buf[1001];
    memset(cnt, 0, sizeof(cnt));
    register int sum = 0, st1 = 0, st2 = 0;
    for (register int i = 1; i <= m + n; i++) {
        std::cin >> c[i];
        std::cin.getline(buf, 1000);
        p[i] = 0;
        for (register int j = 0, end = strlen(buf); j < end; j++) {
            if (isdigit(buf[j])) {
                register int num = buf[j] - '0';
                p[i] |= 1 << (num - 1);
                if (i <= m) cnt[num - 1]++;
            }
        }
        if (i <= m) sum += c[i], st1 |= p[i];
    }
    for (register int i = 0; i < s; i++)
        if (cnt[i] > 1) st2 |= 1 << i;
    memset(f, 0x3f, sizeof(f));
    register int max = (1 << s) - 1;
    f[st1][st2] = sum;
    for (register int i = m + 1; i <= n + m; i++) {
        for (register int s1 = max, n1, n2; s1 >= 0; s1--) {
            for (register int s2 = max; s2 >= 0; s2--) {
                if (f[s1][s2] >= INF) continue;
                n1 = s1 | p[i], n2 = s2 | (s1 & p[i]);
                f[n1][n2] = std::min(f[n1][n2], f[s1][s2] + c[i]);
            }
        }
    }
    std::cout << f[max][max] << '\n';
}

inline void solve() {
    while (std::cin >> s >> m >> n && s && n && m) solveCase();
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    solve();
    return 0;
}