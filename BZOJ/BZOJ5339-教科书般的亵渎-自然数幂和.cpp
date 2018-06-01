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
 * 「BZOJ 5339」教科书般的亵渎 23-05-2018
 * 自然数幂和
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXM = 52;
const int MOD = 1000000007;

int B[MAXM + 1], C[MAXM + 1][MAXM + 1], inv[MAXM + 1];
long long a[MAXM + 1];

typedef unsigned long long ull;

inline int add(int x, int v) { return x + v >= MOD ? x + v - MOD : x + v; }

inline int dec(int x, int v) { return x - v < 0 ? x - v + MOD : x - v; }

inline int get(int n, int k) {
    int ret = 0, t = n + 1;
    for (int i = 1; i <= k + 1; i++) {
        ret = (ret + (ull)C[k + 1][i] * B[k + 1 - i] % MOD * t) % MOD;
        t = t * (n + 1ull) % MOD;
    }
    return (ull)ret * inv[k + 1] % MOD;
}

inline int modPow(int a, int b) {
    int ret = 1;
    for (; b; b >>= 1, a = (ull)a * a % MOD)
        if (b & 1) ret = (ull)ret * a % MOD;
    return ret;
}

int main() {
    // freopen("defile.in", "r", stdin);
    // freopen("defile.out", "w", stdout);
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    inv[1] = 1;
    for (int i = 2; i <= MAXM; i++) inv[i] = (MOD - MOD / i) * (ull)inv[MOD % i] % MOD;
    for (int i = 0; i <= MAXM; i++) C[i][0] = 1;
    for (int i = 1; i <= MAXM; i++)
        for (int j = 1; j <= i; j++) C[i][j] = add(C[i - 1][j], C[i - 1][j - 1]);
    B[0] = 1;
    for (int i = 1, s; i <= MAXM; i++) {
        s = 0;
        for (int j = 0; j < i; j++) s = (s + (ull)C[i + 1][j] * B[j]) % MOD;
        B[i] = (ull)(MOD - s) * inv[i + 1] % MOD;
    }
    int T;
    std::cin >> T;
    for (int m, ret; T--;) {
        long long n;
        std::cin >> n >> m;
        ret = 0;
        for (int i = 1; i <= m; i++) std::cin >> a[i];
        std::sort(a + 1, a + m + 1);
        for (int i = 1; i <= m; i++) a[i] %= MOD;
        for (int i = 0; i <= m; i++) {
            ret = add(ret, get(n - a[i], m + 1));
            for (int j = i + 1; j <= m; j++) {
                ret = dec(ret, modPow(dec(a[j], a[i]), m + 1));
            }
        }
        std::cout << ret << '\n';
    }
    return 0;
}