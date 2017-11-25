#include <bits/stdc++.h>

namespace Math {

inline int gcd(int a, int b) { return !b ? a : gcd(b, a % b); }

template <typename T>
inline void exgcd(T a, T b, T &g, T &x, T &y) {
    !b ? (g = a, x = 1, y = 0) : (exgcd(b, a % b, g, y, x), y -= a / b * x);
}

template <typename T>
inline T invExgcd(const T &a, const T &mod) {
    T g, x, y;
    exgcd(a, mod, g, x, y);
    return (x % mod + mod) % mod;
}

template <typename T>
inline T modPow(T a, T b, T c) {
    register T ret = 1;
    for (; b; b >>= 1, a = a * a % c) (b & 1) ? ret = ret * a % c : 0;
    return ret;
}

template <typename T>
inline T invModPow(T x, T mod) {
    return modPow(x, mod - 2, mod);
}

template <typename T>
inline void invPrime(T *inv, const int n, const T mod) {
    inv[1] = 1;
    for (register int i = 2; i <= n; i++)
        inv[i] = (mod - mod / i) * inv[mod % i] % mod;
}

template <typename T>
inline void facInv(T *fac, T *inv, const int n, const int mod) {
    fac[0] = 1;
    for (register int i = 1; i <= n; i++) fac[i] = fac[i - 1] * i % mod;
    inv[n] = modPow(fac[n], mod - 2, mod);
    for (register int i = n - 1; i >= 0; i--)
        inv[i] = inv[i + 1] * (i + 1) % mod;
}

template <typename T>
inline void nCr(int n, int r, T *fac, T *inv, const int mod) {
    return fac[n] * inv[r] % mod * inv[n - r] % mod;
}

template <typename T, size_t size>
inline void initC(T c[size][size], const int n) {
    for (register int i = 1; i <= n; i++) {
        c[i][0] = 1;
        for (register int j = 1; j <= i; j++)
            c[i][j] = c[i - 1][j] + c[i - 1][j - 1];
    }
}

inline void fastLinearSieveMethod(int *prime, int *phi, int *mu, int &cnt,
                                  bool *vis, const int n) {
    for (register int i = 2; i <= n; i++) {
        if (!vis[i]) prime[cnt++] = i, phi[i] = i - 1, mu[i] = -1;
        for (register int j = 0, k; j < cnt && (k = i * prime[j]) <= n; k++) {
            vis[k] = true;
            if (i % prime[j] == 0) {
                mu[k] = 0, phi[k] = phi[i] * prime[j];
                break;
            } else {
                phi[k] = phi[i] * (prime[j] - 1), mu[k] = -mu[i];
            }
        }
    }
}

inline int phi(const int x) {
    register int ret = x;
    for (register int i = 2; i * i <= x; i++) {
        if (x % i == 0) ret = ret / i * (i - 1);
        while (x % i == 0) x /= i;
    }
    if (x != 1) ret = ret / x * (x - 1);
    return ret;
}

template <typename T>
inline T crt(T *a, T *m, const int n) {
    T M = m[0], ans = a[0], g, x, y;
    for (register int i = 2; i <= n; i++) {
        exgcd(M, m[i], g, x, y);
        if ((a[i] - ans) % g) return -1;
        x = (a[i] - ans) / g * x % (m[i] / g);
        ans = (ans + x * M) % (M = M / g * m[i]);
    }
    return ans > 0 ? ans : ans + M;
}
}

int main() { return 0; }