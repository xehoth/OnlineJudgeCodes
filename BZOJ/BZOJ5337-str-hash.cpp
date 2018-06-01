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
 * 「BZOJ 5337」str 23-05-2018
 * Hash
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MOD = 1e9 + 7;
typedef unsigned int H;
const int HASH_BASE = 31;
const int MAXN = 10000;

inline int add(int x, int v) { return x + v >= MOD ? x + v - MOD : x + v; }

H powVal[MAXN + 1], h[MAXN + 1];
char s[MAXN + 1];
int n, K;
int f[2][MAXN + 1];

inline H get(int l, int r) { return h[r] - h[l - 1] * powVal[r - l + 1]; }

int main() {
    powVal[0] = 1;
    scanf("%d\n%s", &K, s + 1);
    n = strlen(s + 1);
    for (int i = 1; i <= n; i++) powVal[i] = (H)powVal[i - 1] * HASH_BASE;
    for (int i = 1; i <= n; i++) h[i] = h[i - 1] * HASH_BASE + s[i];
    for (int i = 0; i <= n; i++) f[0][i] = 1;
    int end = K & 1;
    for (int k, p = 0; K--; p ^= 1) {
        std::cin >> k;
        H hv;
        for (int len; k--;) {
            scanf("%s", s + 1);
            len = strlen(s + 1);
            hv = 0;
            for (int i = 1; i <= len; i++) hv = hv * HASH_BASE + s[i];
            for (int i = 1; i + len - 1 <= n; i++) {
                if (hv == get(i, i + len - 1)) {
                    f[p ^ 1][i + len - 1] = add(f[p ^ 1][i + len - 1], f[p][i - 1]);
                }
            }
        }
        memset(f[p], 0, sizeof(int) * (n + 1));
    }
    int ans = 0;
    for (int i = 1; i <= n; i++) ans = add(ans, f[end][i]);
    printf("%d", ans);
    return 0;
}