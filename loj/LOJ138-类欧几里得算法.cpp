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
 * 「LOJ 138」类欧几里得算法 18-05-2018
 *
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXK = 10;
const int MOD = 1e9 + 7;

using uint = unsigned int;
using ull = unsigned long long;

inline int add(int x, int v) { return x + v >= MOD ? x + v - MOD : x + v; }

inline int dec(int x, int v) { return x - v < 0 ? x - v + MOD : x - v; }

int C[MAXK + 3][MAXK + 3], B[MAXK + 1], inv[MAXK + 3];
// P_{i,j} = (-1) ^ j\binom{i + 1} {j} B_j
int P[MAXK + 1][MAXK + 3];

inline int get(int k, int n) {
    int ret = 0;
    for (int i = 0; i < k + 2; i++) ret = ((ull)ret * n + P[k][i]) % MOD;
    return (ull)ret * inv[k + 1] % MOD;
}

inline void init() {
    inv[0] = inv[1] = 1;
    for (int i = 2; i <= MAXK + 1; i++)
        inv[i] = (MOD - MOD / i) * (ull)inv[MOD % i] % MOD;
    for (int i = 0; i <= MAXK + 1; i++) {
        C[i][0] = 1;
        for (int j = 1; j <= i; j++)
            C[i][j] = add(C[i - 1][j], C[i - 1][j - 1]);
    }
    B[0] = 1;
    for (int i = 1, s; i <= MAXK; i++) {
        s = 0;
        for (int j = 0; j < i; j++) s = (s + (ull)C[i + 1][j] * B[j]) % MOD;
        B[i] = (ull)(MOD - s) * inv[i + 1] % MOD;
    }
    for (int i = 0; i <= MAXK; i++) {
        for (int j = 0; j <= i; j++) {
            P[i][j] = (j & 1) ? ((ull)(MOD - C[i + 1][j]) * B[j] % MOD)
                              : ((ull)C[i + 1][j] * B[j] % MOD);
        }
        P[i][i + 1] = 0;
    }
    P[0][1] = 1;
}

/**
 * \sum_{x = 0} ^ {n} x ^ {k_1} {\left \lfloor
 * \frac{ax + b}{c} \right \rfloor} ^ {k_2}
 */
inline int classEuclid(int n, int a, int b, int c, int k1, int k2) {
    using Info = std::tuple<int, int, int, int>;
    static std::vector<Info> st;
    st.clear();
    st.reserve(std::__lg(std::min(a, c)) + 3);
    for (;;) {
        st.emplace_back(n, a, b, c);
        if (n < 0 || a == 0) break;
        if (a >= c) {
            a %= c;
        } else if (b >= c) {
            b %= c;
        } else {
            n = ((long long)a * n + b) / c - 1;
            b = c - b - 1;
            std::swap(a, c);
        }
    }

    const int S = k1 + k2;
    static int buc[2][MAXK + 1][MAXK + 1];
    using Ptr = int(*)[MAXK + 1];
    Ptr f = buc[0], g = buc[1];
    while (!st.empty()) {
        std::tie(n, a, b, c) = st.back();
        st.pop_back();
        if (n < 0) {
            continue;
        } else if (a == 0) {
            int q = b / c;
            for (int i = 0, s; i <= S; i++) {
                s = get(i, n);
                for (int j = 0; j <= S - i; j++) {
                    g[i][j] = s;
                    s = (ull)s * q % MOD;
                }
            }
        } else if (a >= c || b >= c) {
            int q = (a >= c) ? a / c : b / c, d = (a >= c) ? 1 : 0;
            // \sum_{i = 0} ^ {k_2} \binom {k_2} {i}q ^ i \sum_{x = 0} ^ n x ^
            // {k_1 + i}\lfloor \frac{rx + b} {c} \rfloor ^ {k_2 - i}
            for (int k1 = 0; k1 <= S; k1++) {
                for (int k2 = 0, s; k2 <= S - k1; k2++) {
                    s = 0;
                    for (int i = 0, p = 1; i <= k2; i++) {
                        s = (s +
                             (ull)p * C[k2][i] % MOD * f[k1 + i * d][k2 - i]) %
                            MOD;
                        p = (ull)p * q % MOD;
                    }
                    g[k1][k2] = s % MOD;
                }
            }
        } else {
            // S_{k_1}(n)m ^ {k_2} + \sum_{i = 0} ^ {k_2 - 1}\sum_{h = 0} ^ {k_1
            // + 1}\big(\binom{k_2} {i}(-1) ^ {k_2 - i}P_{k_1, h}\sum_{y = 1} ^
            // m y ^ i\lfloor \frac{cy - b - 1} {c} \rfloor ^ h \big)
            static int h[MAXK + 1][MAXK + 1];
            for (int k2 = 0; k2 <= S - 1; k2++) {
                for (int k1 = 0, s; k1 <= S - k2 - 1; k1++) {
                    s = 0;
                    for (int j = 0; j <= k1 + 1; ++j) {
                        s = (s + (ull)P[k1][k1 + 1 - j] * f[k2][j]) % MOD;
                    }
                    h[k1][k2] = (ull)s * inv[k1 + 1] % MOD;
                }
            }
            const int m = ((ull)a * n + b) / c;
            for (int k1 = 0; k1 <= S; k1++) {
                int p = get(k1, n);
                for (int k2 = 0, t; k2 <= S - k1; k2++) {
                    t = 0;
                    for (int i = 0; i < k2; i++)
                        t = (t + (ull)h[k1][i] * C[k2][i]) % MOD;
                    g[k1][k2] = dec(p, t);
                    p = (ull)p * m % MOD;
                }
            }
        }
        std::swap(f, g);
    }
    return f[k1][k2];
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    int T, n, a, b, c, k1, k2;
    init();
    for (std::cin >> T; T--;) {
        std::cin >> n >> a >> b >> c >> k1 >> k2;
        std::cout << classEuclid(n, a, b, c, k1, k2) << '\n';
    }
    return 0;
}