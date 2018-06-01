/**
 * Copyright (c) 2018, xehoth
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * 「BZOJ 5336」party 21-05-2018
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 1000 + 1;
const int MAXK = 15;
const int MOD = 1e9 + 7;

inline void add(int &x, int v) { x = x + v >= MOD ? x + v - MOD : x + v; }

int n, k;
int to[1 << MAXK | 1][3], pos[MAXK + 1], tr[3][3];
int f[2][1 << MAXK | 1][3], ans[MAXK + 1];
char s[MAXK + 1];

int main() {
    // freopen("sample/1.in", "r", stdin);
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    std::cin >> n >> k >> (s + 1);
    const int max = 1 << k;
    for (int i = 1; i <= k; i++) s[i] = (s[i] == 'N') ? 0 : (s[i] == 'O' ? 1 : 2);
    for (int i = 0, cnt, x; i < max; i++) {
        for (int j = 0; j < 3; j++) {
            cnt = 0;
            memset(pos + 1, 0, sizeof(int) * k);
            for (int l = 1; l <= k; l++) {
                x = std::max(cnt + ((i >> (l - 1)) & 1), cnt + (j == s[l]));
                if (!pos[x]) pos[x] = l;
                cnt += ((i >> (l - 1)) & 1);
            }
            for (int l = 1; l <= k; l++)
                if (pos[l] != 0) to[i][j] |= (1 << (pos[l] - 1));
        }
    }
    tr[0][0] = tr[1][0] = tr[2][0] = 1;
    tr[1][1] = 2;
    tr[2][2] = -1;
    f[0][0][0] = 1;
    for (int i = 0, cur = 0; i < n; i++) {
        for (int j = 0; j < max; j++) {
            if (f[cur][j][0]) {
                if (tr[0][0] >= 0) add(f[cur ^ 1][to[j][0]][tr[0][0]], f[cur][j][0]);
                if (tr[0][1] >= 0) add(f[cur ^ 1][to[j][1]][tr[0][1]], f[cur][j][0]);
                if (tr[0][2] >= 0) add(f[cur ^ 1][to[j][2]][tr[0][2]], f[cur][j][0]);
            }
            if (f[cur][j][1]) {
                if (tr[1][0] >= 0) add(f[cur ^ 1][to[j][0]][tr[1][0]], f[cur][j][1]);
                if (tr[1][1] >= 0) add(f[cur ^ 1][to[j][1]][tr[1][1]], f[cur][j][1]);
                if (tr[1][2] >= 0) add(f[cur ^ 1][to[j][2]][tr[1][2]], f[cur][j][1]);
            }
            if (f[cur][j][2]) {
                if (tr[2][0] >= 0) add(f[cur ^ 1][to[j][0]][tr[2][0]], f[cur][j][2]);
                if (tr[2][1] >= 0) add(f[cur ^ 1][to[j][1]][tr[2][1]], f[cur][j][2]);
                if (tr[2][2] >= 0) add(f[cur ^ 1][to[j][2]][tr[2][2]], f[cur][j][2]);
            }
        }
        for (int j = 0; j < max; j++) f[cur][j][0] = f[cur][j][1] = f[cur][j][2] = 0;
        cur ^= 1;
    }
    for (int i = 0, t; i < max; i++) {
        t = __builtin_popcount(i);
        add(ans[t], f[n & 1][i][0]);
        add(ans[t], f[n & 1][i][1]);
        add(ans[t], f[n & 1][i][2]);
    }
    for (int i = 0; i <= k; i++) std::cout << ans[i] << '\n';
    return 0;
}
