/**
 * Copyright (c) 2016-2018, xehoth
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
 * 「BZOJ 5299」解锁屏幕 04-05-2018
 * DP
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 19;
const int MOD = 100000007;

int n, map[MAXN][MAXN];
unsigned long long f[1 << MAXN][MAXN];

struct Point {
    int x, y;

    Point() {}

    Point(int x, int y) : x(x), y(y) {}

    int operator*(const Point &p) const { return x * p.y - y * p.x; }

    Point operator-(const Point &p) const { return Point(x - p.x, y - p.y); }
} pts[MAXN];

inline bool check(int l, int mid, int r) {
    if (pts[mid].x < std::min(pts[l].x, pts[r].x) ||
        pts[mid].x > std::max(pts[l].x, pts[r].x) ||
        pts[mid].y < std::min(pts[l].y, pts[r].y) ||
        pts[mid].y > std::max(pts[l].y, pts[r].y))
        return 0;
    return (pts[mid] - pts[l]) * (pts[r] - pts[l]) == 0;
}

int main() {
    std::cin >> n;
    for (int i = 0; i < n; i++) std::cin >> pts[i].x >> pts[i].y;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                for (int k = 0; k < n; k++) {
                    if (k != i && k != j && check(i, k, j)) {
                        map[i][j] |= (1 << k);
                    }
                }
            }
        }
    }
    for (register int i = 0; i < n; i++) f[1 << i][i] = 1;
    register int max = 1 << n;
    register unsigned long long ret = 0;
    for (register int i = 1; i < max; i++) {
        register bool flag = __builtin_popcount(i) >= 4;
        for (register int j = 0; j < n; j++) {
            if (f[i][j]) {
                if (flag) ret += f[i][j];
                if (!(i & 1) && (i & map[j][0]) == map[j][0])
                    f[i | 1][0] += f[i][j];
                if (!(i & 2) && (i & map[j][1]) == map[j][1])
                    f[i | 2][1] += f[i][j];
                if (!(i & 4) && (i & map[j][2]) == map[j][2])
                    f[i | 4][2] += f[i][j];
                if (!(i & 8) && (i & map[j][3]) == map[j][3])
                    f[i | 8][3] += f[i][j];
                if (!(i & 16) && (i & map[j][4]) == map[j][4])
                    f[i | 16][4] += f[i][j];
                if (!(i & 32) && (i & map[j][5]) == map[j][5])
                    f[i | 32][5] += f[i][j];
                if (!(i & 64) && (i & map[j][6]) == map[j][6])
                    f[i | 64][6] += f[i][j];
                if (!(i & 128) && (i & map[j][7]) == map[j][7])
                    f[i | 128][7] += f[i][j];
                if (!(i & 256) && (i & map[j][8]) == map[j][8])
                    f[i | 256][8] += f[i][j];
                if (!(i & 512) && (i & map[j][9]) == map[j][9])
                    f[i | 512][9] += f[i][j];
                if (!(i & 1024) && (i & map[j][10]) == map[j][10])
                    f[i | 1024][10] += f[i][j];
                if (!(i & 2048) && (i & map[j][11]) == map[j][11])
                    f[i | 2048][11] += f[i][j];
                if (!(i & 4096) && (i & map[j][12]) == map[j][12])
                    f[i | 4096][12] += f[i][j];
                if (!(i & 8192) && (i & map[j][13]) == map[j][13])
                    f[i | 8192][13] += f[i][j];
                if (!(i & 16384) && (i & map[j][14]) == map[j][14])
                    f[i | 16384][14] += f[i][j];
                if (!(i & 32768) && (i & map[j][15]) == map[j][15])
                    f[i | 32768][15] += f[i][j];
                if (!(i & 65536) && (i & map[j][16]) == map[j][16])
                    f[i | 65536][16] += f[i][j];
                if (!(i & 131072) && (i & map[j][17]) == map[j][17])
                    f[i | 131072][17] += f[i][j];
                if (!(i & 262144) && (i & map[j][18]) == map[j][18])
                    f[i | 262144][18] += f[i][j];
            }
        }
    }
    std::cout << ret % MOD;
    return 0;
}