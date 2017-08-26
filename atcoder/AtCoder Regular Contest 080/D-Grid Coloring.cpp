/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「ARC 080D」Grid Coloring 25-08-2017
 * 结论
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 10000;

int main() {
    register int h, w, n, m, r = 0;
    static int d[MAXN + 1];
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    std::cin >> h >> w >> n;
    for (register int i = 0; i < n; i++) {
        std::cin >> m;
        for (register int j = 0; j < m; j++) d[r++] = i + 1;
    }
    for (register int i = 0; i < h; i++) {
        for (register int j = 0; j < w; j++)
            printf("%d%c", d[i * w + (i % 2 ? w - j - 1 : j)],
                   w - j - 1 ? ' ' : '\n');
    }
    return 0;
}