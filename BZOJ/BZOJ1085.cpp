/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1085」07-01-2017
 *
 * @author xehoth
 */
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
const int MAXN = 5;
const char des[5][5] = {{'1', '1', '1', '1', '1'},
                        {'0', '1', '1', '1', '1'},
                        {'0', '0', '*', '1', '1'},
                        {'0', '0', '0', '0', '1'},
                        {'0', '0', '0', '0', '0'}};
const int move[8][2] = {{1, -2}, {2, -1}, {2, 1},   {1, 2},
                        {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}};
int t, dep, stx, sty;
char cur[MAXN][MAXN];
inline bool check(const int &x, const int &y) {
    return x >= 0 && x <= 4 && y >= 0 && y <= 4;
}
inline int bound() {
    register int res = -1;
    for (register int i = 0; i <= 4; i++)
        for (register int j = 0; j <= 4; j++)
            if (cur[i][j] != des[i][j]) res++;
    return res;
}
inline bool dfs(const int &step, const int &x, const int &y) {
    if (step > dep) {
        if (bound() == -1) return true;
        return false;
    }
    if (bound() + step - 1 > dep) return false;
    for (register int i = 0; i <= 7; i++) {
        register int dx = x + move[i][0], dy = y + move[i][1];
        if (check(dx, dy)) {
            std::swap(cur[x][y], cur[dx][dy]);
            if (dfs(step + 1, dx, dy)) return true;
            std::swap(cur[x][y], cur[dx][dy]);
        }
    }
    return false;
}
int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cin >> t;
    while (t--) {
        for (register int i = 0; i <= 4; i++) std::cin >> cur[i];
        for (register int i = 0; i <= 4; i++) {
            for (register int j = 0; j <= 4; j++) {
                if (cur[i][j] == '*') {
                    stx = i, sty = j;
                    break;
                }
            }
        }
        for (dep = 0; dep <= 15; dep++)
            if (dfs(1, stx, sty)) break;
        if (dep > 15)
            std::cout << "-1\n";
        else
            std::cout << dep << "\n";
    }
    return 0;
}
