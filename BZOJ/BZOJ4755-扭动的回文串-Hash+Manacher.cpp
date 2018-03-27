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
 * 「BZOJ 4755」扭动的回文串 25-03-2018
 * Hash + Manacher
 * @author xehoth
 */
#include <bits/stdc++.h>

typedef unsigned long long H;

const int HASH_BASE = 31;
const int MAXN = 100000 + 1;
int n, ans, d[MAXN * 2 + 3];
char a[MAXN], b[MAXN], s[MAXN * 2 + 3];
H powVal[MAXN], ha[MAXN], hb[MAXN];

inline void solve(char *str) {
    s[0] = '.';
    for (int i = 1; i < 2 * n; i++) s[i] = (i & 1) ? str[(i + 1) >> 1] : '#';
    int max = 0, L, R;
    for (int i = 1; i < 2 * n; i++) {
        d[i] =
            max + d[max] < i ? 0 : std::min(d[max * 2 - i], max + d[max] - i);
        while (s[i + d[i] + 1] == s[i - d[i] - 1]) d[i]++;
        if (i + d[i] > max + d[max]) max = i;
        L = (i - d[i] + 2) >> 1;
        R = (i + d[i] + 1) >> 1;
        if (str == a)
            R--;
        else
            L++;
        int l = 0, r = std::min(L - 1, n - R) + 1;
        for (int mid; r - l > 1;) {
            mid = (l + r) >> 1;
            if (ha[L - 1] - ha[L - mid - 1] * powVal[mid] ==
                hb[R + 1] - hb[R + mid + 1] * powVal[mid]) {
                l = mid;
            } else {
                r = mid;
            }
        }
        ans = std::max(ans, R - L + 2 + l * 2);
    }
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    scanf("%d\n%s\n%s", &n, a + 1, b + 1);
    powVal[0] = 1;
    for (int i = 1; i <= n; i++) powVal[i] = powVal[i - 1] * HASH_BASE;
    for (int i = 1; i <= n; i++) ha[i] = ha[i - 1] * HASH_BASE + a[i];
    for (int i = n; i; i--) hb[i] = hb[i + 1] * HASH_BASE + b[i];
    solve(a);
    solve(b);
    std::cout << ans;
    return 0;
}