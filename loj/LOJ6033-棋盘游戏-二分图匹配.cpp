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
 * 「LOJ 6033」棋盘游戏 01-06-2018
 * 二分图匹配
 * @author xehoth
 */
#include <bits/stdc++.h>

const int DX[] = {1, -1, 0, 0};
const int DY[] = {0, 0, 1, -1};

const int MAXN = 100;

int a[MAXN + 1][MAXN + 1], n, m, vis[MAXN + 1][MAXN + 1], idx;
std::pair<int, int> match[MAXN + 1][MAXN + 1];
bool ans[MAXN + 1][MAXN + 1];
char s[MAXN + 1];

bool dfs(int x, int y) {
    vis[x][y] = idx;
    for (int i = 0, nx, ny; i < 4; i++) {
        nx = x + DX[i];
        ny = y + DY[i];
        if (!nx || nx > n || !ny || ny > m || !a[nx][ny]) continue;
        if (vis[nx][ny] != idx) {
            vis[nx][ny] = idx;
            if (!match[nx][ny].first || dfs(match[nx][ny].first, match[nx][ny].second)) {
                match[x][y] = {nx, ny};
                match[nx][ny] = {x, y};
                return true;
            }
        }
    }
    return false;
}

void calc(int x, int y) {
    if (vis[x][y] == idx) return;
    vis[x][y] = idx;
    ans[x][y] = true;
    for (int i = 0, nx, ny; i < 4; i++) {
        nx = x + DX[i];
        ny = y + DY[i];
        if (!nx || nx > n || !ny || ny > m || !a[nx][ny]) continue;
        if (vis[nx][ny] != idx) {
            vis[nx][ny] = idx;
            calc(match[nx][ny].first, match[nx][ny].second);
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        std::cin >> (s + 1);
        for (int j = 1; j <= m; j++) a[i][j] = (s[j] == '.');
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (a[i][j] && !match[i][j].first) {
                idx++;
                dfs(i, j);
            }
        }
    }
    idx++;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            if (a[i][j] && !match[i][j].first) calc(i, j);
    int cnt = 0;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            if (ans[i][j]) cnt++;
    std::cout << cnt << '\n';
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            if (ans[i][j]) std::cout << i << ' ' << j << '\n';
    return 0;
}
