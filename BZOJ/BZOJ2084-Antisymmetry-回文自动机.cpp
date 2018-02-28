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
 * 「BZOJ 2084」Antisymmetry 27-02-2018
 * 回文自动机
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 500000 + 2;

int c[MAXN][2], fail[MAXN], cnt[MAXN], len[MAXN], cur = 2, n, last, p, np, tot;

char s[MAXN];

inline int getFail(int p) {
    while (p != 1 && (s[n - len[p] - 1] == s[n] || s[n - len[p] - 1] == -1))
        p = fail[p];
    return p;
}

int main() {
    for (fread(s, 1, 1, stdin); isdigit(s[0]); fread(s, 1, 1, stdin))
        tot = tot * 10 + (s[0] ^ '0');
    s[0] = -1;
    fread(s + 1, 1, 500001, stdin);
    fail[0] = fail[1] = 1;
    len[1] = -1;
    for (n = 1; n <= tot; n++) {
        s[n] -= '0';
        if ((p = getFail(last)) == 1) {
            last = 0;
            continue;
        }
        if (!c[p][s[n]]) {
            len[np = cur++] = len[p] + 2;
            fail[np] = c[getFail(fail[p])][s[n]];
            c[p][s[n]] = np;
        }
        cnt[last = c[p][s[n]]]++;
    }
    long long ans = 0;
    for (int i = cur - 1; i > 0; i--) cnt[fail[i]] += cnt[i];
    for (int i = 2; i < cur; i++)
        if (len[i] % 2 == 0) ans += cnt[i];
    std::cout << ans;
}
