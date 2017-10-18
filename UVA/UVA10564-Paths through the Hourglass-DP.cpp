/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「UVA 10564」Paths through the Hourglass 18-10-2017
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

#define long long long

const int MAXN = 20 + 3, MAX_SUM = 500 + 3;
int n, s, a[MAXN * 2][MAXN];
bool flag, opers[MAXN * 2], ansOpers[MAXN * 2];
long f[MAXN * 2][MAXN][MAX_SUM];

inline int getNum(int line) { return line < n - 1 ? n - line : 2 + line - n; }

inline bool check(int i, int j) { return j >= 0 && j < getNum(i); }

long dp(int i, int j, int nowSum) {
    if (i == 2 * n - 2) {
        if (nowSum == s && !flag) {
            flag = true;
            memcpy(ansOpers, opers, sizeof(opers));
        }
        return nowSum == s;
    }
    if (f[i][j][nowSum] != -1) return f[i][j][nowSum];
    register long &cnt = f[i][j][nowSum] = 0;
    if (i < n - 1) {
        for (register int k = j - 1; k <= j; k++) {
            if (check(i + 1, k)) {
                opers[i] = k == j - 1 ? 0 : 1;
                cnt += dp(i + 1, k, nowSum + a[i + 1][k]);
            }
        }
    } else {
        for (register int k = j; k <= j + 1; k++) {
            if (check(i + 1, k)) {
                opers[i] = k == j ? 0 : 1;
                cnt += dp(i + 1, k, nowSum + a[i + 1][k]);
            }
        }
    }
    return cnt;
}

inline void solve() {
    while (std::cin >> n >> s && (n + s)) {
        for (register int i = 0; i < n * 2 - 1; i++)
            for (register int j = 0, l = getNum(i); j < l; j++)
                std::cin >> a[i][j];
        memset(f, -1, sizeof(f));
        flag = false;
        register long cnt = 0;
        register int rec = -1;
        for (register int i = 0; i < n; i++) {
            cnt += dp(0, i, a[0][i]);
            if (rec == -1 && flag) rec = i;
        }
        std::cout << cnt << '\n';
        if (cnt) {
            std::cout << rec << ' ';
            for (register int i = 0; i < 2 * n - 2; i++)
                std::cout.rdbuf()->sputc(ansOpers[i] ? 'R' : 'L');
            std::cout.rdbuf()->sputc('\n');
        } else {
            std::cout.rdbuf()->sputc('\n');
        }
    }
}
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    solve();
}