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
 * 「」--2018
 *
 * @author xehoth
 */
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unordered_map>

namespace xehoth {

template <typename T>
inline T gcd(T a, T b) {
    return !b ? a : gcd(b, a % b);
}

template <typename T>
inline void exgcd(T a, T b, T &g, T &x, T &y) {
    !b ? (x = 1, y = 0, g = a) : (exgcd(b, a % b, g, y, x), y -= a / b * x);
}

template <typename T>
inline void exgcd(T a, T b, T &x, T &y) {
    !b ? (x = 1, y = 0) : (exgcd(b, a % b, y, x), y -= a / b * x);
}

template <typename T, T mod>
inline T exgcdInv(const T v) {
    static T x, y;
    exgcd(v, mod, x, y);
    return (x % mod + mod) % mod;
}

template <typename T>
inline T exgcdInv(const T v, const T mod) {
    static T x, y;
    exgcd(v, mod, x, y);
    return (x % mod + mod) % mod;
}

template <typename T, T mod, typename U = unsigned long long>
inline T modPow(T a, int b) {
    T ret = 1;
    for (; b; b >>= 1, a = (U)a * a % mod) (b & 1) && (ret = (U)ret * a % mod);
    return ret;
}

template <typename T = int, typename U = unsigned long long>
inline T modPow(T a, int b, const T mod) {
    T ret = 1;
    for (; b; b >>= 1, a = (U)a * a % mod) (b & 1) && (ret = (U)ret * a % mod);
    return ret;
}

template <typename T, typename mul>
inline T modPow(T a, int b) {
    T ret(1);
    for (; b; b >>= 1, a = mul(a, a)) (b & 1) && (ret = mul(ret, a));
    return ret;
}

template <typename T, T mod>
inline T modPowInv(const T v) {
    return modPow<T, mod>(v, mod - 2);
}

template <typename T>
inline T modPowInv(const T v, const T mod) {
    return modPow(v, mod - 2, mod);
}

template <typename T, typename U = unsigned long long>
inline T crt(T *a, T *m, const int n) {
    T M = m[0], mi, ret = 0;
    for (int i = 1; i < n; ++i) M *= m[i];
    for (int i = 0; i < n; ++i) {
        mi = M / m[i];
        ret = (ret + (U)a[i] * mi * exgcdInv(mi, m[i])) % M;
    }
    return (ret % M + M) % M;
}

template <typename T, typename U = unsigned long long>
inline T excrt(T *a, T *m, const int n) {
    T M = m[0], ret = a[0], g, x, y;
    for (int i = 1; i < n; i++) {
        exgcd(M, m[i], g, x, y);
        if ((a[i] - ret) % g) return -1;
        x = (a[i] - ret) / g * x % (m[i] / g);
        ret = (ret + (U)x * M) % (M = M / g * m[i]);
    }
}

template <typename T, typename U = unsigned long long>
inline T bsgs(T a, T b, T c) {
    int cnt = 0, g, d = 1;
    while ((g = gcd(a, c)) != 1) {
        if (b % g) return -1;
        b /= g;
        c /= g;
        ++cnt;
        d = (long long)d * (a / g) % c;
    }
    b = (U)b * exgcdInv(d, c) % c;
    const int s = sqrt(c);
    std::unordered_map<T, int> h(s);
    T p = 1;
    for (int i = 0; i < s; ++i) {
        if (p == b) return i + cnt;
        h[(U)p * b % c] = i;
        p = (U)p * a % c;
    }
    T q = p;
    for (int i = s; i - s + 1 <= c - 1; i += s) {
        auto it = h.find(q);
        if (it != h.end()) return i - it->second + cnt;
        q = (U)q * p % c;
    }
    return -1;
}

template <typename T, T mod, typename U = unsigned long long>
struct MatrixMod {
    std::vector<std::vector<T> > a;

    MatrixMod() = default;
    MatrixMod(const int n, const int m) : a(n, std::vector<T>(m)) {}

    inline const MatrixMod &operator*(const MatrixMod &b) const {
        static MatrixMod c;
        const int n = a.size(), p = b.size(), m = b[0].size();
        c = MatrixMod(n, m);
        for (int i = 0; i < n; ++i)
            for (int k = 0; k < p; ++k)
                for (int j = 0; j < m; ++j)
                    c[i][j] = (c[i][j] + (U)a[i][k] * b[k][j]) % mod;
        return c;
    }

    inline std::vector<T> &operator[](int i) { return a[i]; }
    inline const std::vector<T> &operator[](int i) const { return a[i]; }
};

template <typename T, typename U = unsigned long long>
struct Matrix {
    std::vector<std::vector<T> > a;

    Matrix() = default;
    Matrix(const int n, const int m) : a(n, std::vector<T>(m)) {}

    inline const Matrix &operator*(const Matrix &b) const {
        static Matrix c;
        const int n = a.size(), p = b.size(), m = b[0].size();
        c = Matrix(n, m);
        for (int i = 0; i < n; ++i)
            for (int k = 0; k < p; ++k)
                for (int j = 0; j < m; ++j) c[i][j] += (U)a[i][k] * b[k][j];
        return c;
    }

    inline std::vector<T> &operator[](int i) { return a[i]; }
    inline const std::vector<T> &operator[](int i) const { return a[i]; }
};

template <bool lcpFlag = false>
struct SuffixArray;

template <>
struct SuffixArray<true> {};

template <>
struct SuffixArray<false> {
    std::vector<bool> t;
    std::vector<int> sa, rk, ht;

    inline bool islms(const bool *t, const int i) {
        return i > 0 && t[i] && !t[i - 1];
    }

    template <typename T>
    inline void sort(T s, int *sa, const int len, const int sz, const int sigma,
                     bool *t, int *b, int *cb, int *p) {
        memset(sa, -1, sizeof(int) * len);
        memset(b, 0, sizeof(int) * sigma);
        for (int i = 0; i < len; ++i) ++b[sa[i]];
        cb[0] = b[0];
        for (int i = 1; i < sigma; ++i) cb[i] = cb[i - 1] + b[i];
        for (int i = sz - 1; i >= 0; --i) sa[--cb[s[p[i]]]] = p[i];
        for (int i = 1; i < sigma; ++i) cb[i] = cb[i - 1] + b[i];
        for (int i = 0; i < len; ++i)
            if (sa[i] > 0 && !t[sa[i] - 1]) sa[cb[s[sa[i] - 1]]++] = sa[i] - 1;
    }

    SuffixArray() = default;
    SuffixArray(const int n) : t(2 * n), sa(n), rk(n), ht(n) {}

    inline void build(const int sigma) {}
};
}  // namespace xehoth

int main() {
    int ret = 1;
    for (int i = 1; i < ret; ++i) return ret;

    return 0;
}
