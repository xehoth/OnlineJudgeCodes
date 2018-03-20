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
 * 「BZOJ 4522」密钥破解 20-03-2018
 * Pollard-Rho
 * @author xehoth
 */
#include <bits/stdc++.h>

inline unsigned long long gen() {
    static unsigned long long x = 495;
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    return x * 0x2545F4914F6CDD1Dull;
}

const int PRIME[] = {3, 5, 7, 11, 13, 17, 19, 23, 29};

inline long long mul(long long a, long long b, long long c) {
    return (a * b - (long long)((long double)a / c * b) * c + c) % c;
}

inline long long modPow(long long a, long long b, long long mod) {
    long long ret = 1;
    for (; b; b >>= 1, a = mul(a, a, mod))
        if (b & 1) ret = mul(ret, a, mod);
    return ret;
}

inline bool witness(long long v, long long p) {
    long long r = p - 1, x;
    int s = 0;
    for (; ~r & 1; r >>= 1) s++;
    x = modPow(v, r, p);
    if (x == 1 || x == p - 1) return false;
    while (s--)
        if ((x = mul(x, x, p)) == p - 1) return false;
    return true;
}

inline bool millerRabin(long long p) {
    if (p == 2) return true;
    if (!(p & 1) || p < 2) return false;
    for (int i = 0; i < 9; i++) {
        if (p == PRIME[i]) return true;
        if (witness(PRIME[i], p)) return false;
    }
    return true;
}

long long gcd(long long a, long long b) { return b ? gcd(b, a % b) : a; }

inline long long add(long long x, long long v, long long mod) {
    return x + v >= mod ? x + v - mod : x + v;
}

inline long long pollardRho(long long n, long long c) {
    long long k = 2, x = gen() % n, y = x, p = 1;
    for (long long i = 1; p == 1; i++) {
        x = add(mul(x, x, n), c, n);
        p = gcd(n, std::abs(y - x));
        if (i == k) {
            y = x;
            k <<= 1;
        }
    }
    return p;
}

inline void getFac(long long n, std::vector<long long> &fac) {
    if (n == 1) return;
    if (millerRabin(n)) {
        fac.push_back(n);
        return;
    }
    long long t = n;
    while (t == n) t = pollardRho(n, gen() % (n - 1));
    getFac(t, fac);
    getFac(n / t, fac);
}

void exgcd(long long a, long long b, long long &x, long long &y) {
    !b ? (x = 1, y = 0) : (exgcd(b, a % b, y, x), y -= a / b * x);
}

inline long long inv(long long v, long long mod) {
    long long x, y;
    exgcd(v, mod, x, y);
    return (x % mod + mod) % mod;
}

int main() {
    long long e, N, c;
    std::cin >> e >> N >> c;
    std::vector<long long> fac;
    getFac(N, fac);
    std::sort(fac.begin(), fac.end());
    long long r = (fac[0] - 1) * (fac[1] - 1);
    long long d = inv(e, r);
    long long n = modPow(c, d, N);
    std::cout << d << ' ' << n;
    return 0;
}