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
 * 「BZOJ 5330」反回文串 25-05-2018
 * PollardRho
 * @author xehoth
 */
#include <bits/stdc++.h>

typedef unsigned long long ull;

inline ull gen() {
    static ull x = 495;
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    return x * 0x2545F4914F6CDD1Dull;
}

const int PRIME[] = {3, 5, 7, 11, 13, 17, 19};

inline long long mul(long long a, long long b, long long c) {
    return (a * b - (long long)((long double)a / c * b) * c + c) % c;
}

inline long long modPow(long long a, long long b, long long mod) {
    long long ret = 1;
    for (; b; b >>= 1, a = mul(a, a, mod))
        if (b & 1) ret = mul(ret, a, mod);
    return ret;
}

inline bool witness(long long v, long long p, long long r, int s) {
    long long x = modPow(v, r, p);
    if (x == 1 || x == p - 1) return false;
    while (s--)
        if ((x = mul(x, x, p)) == p - 1) return false;
    return true;
}

inline bool millerRabin(long long p) {
    if (p == 2) return true;
    if (!(p & 1) || p < 2) return false;
    long long r = p - 1;
    int s = 0;
    for (; ~r & 1; r >>= 1) s++;
    for (int i = 0; i < 7; i++) {
        if (p == PRIME[i]) return true;
        if (witness(PRIME[i], p, r, s)) return false;
    }
    return true;
}

inline long long add(long long x, long long v, long long mod) {
    return x + v >= mod ? x + v - mod : x + v;
}

inline long long pollardRho(long long n, long long c) {
    long long k = 2, x = gen() % n, y = x, p = 1;
    for (long long i = 1; p == 1; i++) {
        x = add(mul(x, x, n), c, n);
        p = std::__gcd(n, std::abs(y - x));
        if (i == k) {
            y = x;
            k <<= 1;
        }
    }
    return p;
}

const int MAXN = 65;
long long pr[MAXN], tp[MAXN], n, k;
int cnt, tc[MAXN], mod, m;

inline void getFac(long long n) {
    if (n == 1) return;
    if (millerRabin(n)) {
        pr[cnt++] = n;
        return;
    }
    long long t = n;
    while (t == n) t = pollardRho(n, gen() % (n - 1));
    getFac(t);
    getFac(n / t);
}

inline int modPow(int a, long long b) {
    int ret = 1;
    for (; b; b >>= 1, a = (ull)a * a % mod)
        if (b & 1) ret = (ull)ret * a % mod;
    return ret;
}

inline int dfs(int x, long long cur, int val) {
    if (x == m) {
        if (cur & 1) return cur % mod * modPow(k, (cur + 1) >> 1) % mod * val % mod;
        return (cur >> 1) % mod * modPow(k, cur >> 1) % mod * val % mod;
    }
    long long ret = 0, v = (long long)val * ((1 - tp[x]) % mod + mod) % mod;
    if (tp[x] & 1) ret += dfs(x + 1, cur, v);
    for (int i = 1; i < tc[x]; i++) ret += dfs(x + 1, cur *= tp[x], v);
    ret += dfs(x + 1, cur * tp[x], val);
    return ret % mod;
}

inline void solve() {
    std::cin >> n >> k >> mod;
    k %= mod;
    m = cnt = 0;
    getFac(n);
    std::sort(pr, pr + cnt);
    if (cnt) {
        tp[m++] = pr[0];
        tc[0] = 1;
    }
    for (int i = 1; i < cnt; i++) {
        if (pr[i] != pr[i - 1]) {
            tp[m++] = pr[i];
            tc[m - 1] = 0;
        }
        tc[m - 1]++;
    }
    std::cout << dfs(0, 1, 1) << '\n';
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);
    int T;
    std::cin >> T;
    while (T--) solve();
    return 0;
}