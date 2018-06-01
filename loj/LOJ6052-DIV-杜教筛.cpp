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
 * 「LOJ 6052」DIV 31-05-2018
 * 杜教筛
 * @author xehoth
 */
#include <bits/stdc++.h>

const int B = 100000;
const int MOD = 1004535809;
const int MAXB = 4641588 + 3;
const int INV2 = 502267905;

int buc[2][B + 1];
bool np[MAXB];
int p[MAXB], cnt;
int sigma[MAXB], fs[MAXB], d[2 * B + 1], f[2 * B + 1];
long long n, l;

template <typename T>
inline T add(T x, T v) {
    return x + v >= MOD ? x + v - MOD : x + v;
}

template <typename T>
inline T dec(T x, T v) {
    return x - v < 0 ? x - v + MOD : x - v;
}

inline int &get(long long x) { return x < B ? buc[0][x] : buc[1][n / x]; }

inline void init(int n) {
    sigma[1] = 1;
    for (int i = 2; i < n; i++) {
        if (!np[i]) {
            p[cnt++] = i;
            sigma[i] = i + 1;
        }
        for (int j = 0, k; j < cnt && (k = i * p[j]) < n; j++) {
            np[k] = 1;
            if (i % p[j] == 0) {
                sigma[k] = (p[j] + 1) * sigma[i] - p[j] * sigma[i / p[j]];
                break;
            } else {
                sigma[k] = sigma[i] * (1 + p[j]);
            }
        }
    }
    for (int i = 1, t, k; i * i < n; i++) {
        t = i * i;
        for (int j = 1; (k = j * j + t) < n; j++)
            if (std::__gcd(i, j) == 1) fs[k] += i;
    }
    for (int i = 1; i < n; i++) sigma[i] = add(sigma[i], sigma[i - 1]);
    for (int i = 1; i < n; i++) fs[i] = add(fs[i], fs[i - 1]);
}

inline void calc(long long n) {
    int c = 0;
    for (long long i = 1, pos; i <= n; i = pos + 1) {
        pos = n / (n / i);
        if (pos < l) {
            d[c] = sigma[pos];
            f[c] = fs[pos];
        } else {
            for (long long a = 1; a * a <= pos; a++)
                f[c] = (f[c] + a * (long long)floor(sqrt(pos - a * a))) % MOD;
            for (long long a = 2; a * a <= pos; a++)
                f[c] = dec<int>(f[c], (long long)f[get(pos / (a * a))] * a % MOD);
            for (long long j = 1, t; j <= pos; j = t + 1) {
                t = pos / (pos / j);
                d[c] = (d[c] + (j + t) % MOD * (t - j + 1) % MOD * INV2 % MOD * (pos / t)) % MOD;
            }
        }
        get(pos) = c++;
    }
    get(0) = c;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::cin >> n;
    l = pow(n, 2.0 / 3.0) + 1;
    init(l);
    calc(n);
    int ret = 0;
    for (long long i = 1, pos; i <= n; i = pos + 1) {
        pos = n / (n / i);
        ret = (ret + (long long)dec(f[get(pos)], f[get(i - 1)]) * d[get(n / pos)]) % MOD;
    }
    std::cout << (ret * 2ull + d[get(n)]) % MOD;
    return 0;
}