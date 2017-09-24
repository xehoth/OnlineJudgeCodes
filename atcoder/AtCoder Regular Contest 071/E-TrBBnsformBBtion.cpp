/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「ARC 071E」TrBBnsformBBtion 20-08-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace Task {

#define long long long

const int MAXN = 1000000;

char s[MAXN + 1], t[MAXN + 1];

const int MOD = 1e9 + 7;
int qa[MAXN], qb[MAXN], n, x, y, X, Y;
char a[MAXN], b[MAXN];

inline void solve() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    std::cin >> a + 1 >> b + 1;
    for (int i = 1; a[i]; i++) qa[i] = qa[i - 1] + a[i] - 'A' + 1;
    for (int i = 1; b[i]; i++) qb[i] = qb[i - 1] + b[i] - 'A' + 1;
    for (std::cin >> n; n--;) {
        std::cin >> x >> y >> X >> Y;
        puts((qa[y] - qa[x - 1]) % 3 == (qb[Y] - qb[X - 1]) % 3 ? "YES" : "NO");
    }
}

#undef long
}

int main() {
    Task::solve();
    return 0;
}