/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「COGS 2294」释迦 08-01-2018
 * 三模数 NTT
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

inline char read() {
    static const int IN_LEN = 1 << 18 | 1;
    static char buf[IN_LEN], *s, *t;
    return (s == t) && (t = (s = buf) + fread(buf, 1, IN_LEN, stdin)),
           s == t ? -1 : *s++;
}

const int OUT_LEN = 1 << 18 | 1;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    (oh == obuf + OUT_LEN) && (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf);
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[21], cnt;
    if (x != 0) {
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    } else {
        print('0');
    }
}

struct InputOutputStream {
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        iosig && (x = -x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MOD = 23333333;
const int MOD1 = 998244353;
const int MOD2 = 1004535809;
const int MOD3 = 469762049;
const int G = 3;
const int MAXN = 1 << 18 | 1;

typedef unsigned long long ulong;

int w[MAXN], a[MAXN], b[MAXN], ans1[MAXN], ans2[MAXN], *ans3 = a, tmp[MAXN];

template <int mod>
inline int modPow(register int a, register int b) {
    register int ret = 1;
    for (; b; b >>= 1, a = (ulong)a * a % mod)
        if (b & 1) ret = (ulong)ret * a % mod;
    return ret;
}

const ulong MOD1_MOD2 = 1002772198720536577ull;
const int MOD1_INV_MOD2 = 669690699 /*modPow<MOD2>(MOD1, MOD2 - 2)*/;
const int MOD2_INV_MOD1 = 332747959 /*modPow<MOD1>(MOD2, MOD1 - 2)*/;
const int MOD1_MOD2_INV_MOD3 = 354521948;
/*modPow<MOD3>(MOD1_MOD2 % MOD3, MOD3 - 2);*/
const int MOD1_MOD2_MOD = 15853906 /*MOD1_MOD2 % MOD*/;

template <int mod>
inline void ntt(int *a, int n, int f) {
    for (register int i = 0, j = 0; i < n; i++) {
        if (i > j) std::swap(a[i], a[j]);
        for (register int k = n >> 1; (j ^= k) < k; k >>= 1)
            ;
    }
    for (register int i = 1; i < n; i <<= 1) {
        for (register int j = 0; j < n; j += i << 1) {
            register int *x = a + j, *y = a + i + j, *w = ::w + i;
            for (register int k = 0, t; k < i; k++) {
                t = (ulong)w[k] * y[k] % mod;
                y[k] = (x[k] - t <= 0 ? x[k] - t + mod : x[k] - t);
                x[k] = (x[k] + t >= mod ? x[k] + t - mod : x[k] + t);
            }
        }
    }

    if (f == -1) {
        std::reverse(a + 1, a + n);
        register const int inv = modPow<mod>(n, mod - 2);
        for (register int i = 0; i < n; i++) a[i] = (ulong)a[i] * inv % mod;
    }
}

template <int mod>
inline void init(const int k, int *w) {
    register int n = k >> 1;
    w[n] = 1;
    w[n + 1] = modPow<mod>(G, (mod - 1) / k);
    for (register int i = 2; i < n; i++)
        w[n + i] = (ulong)w[n + i - 1] * w[n + 1] % mod;
    for (register int i = n - 1; i > 0; i--) w[i] = w[i << 1];
}

template <int mod>
inline void mul(int *a, int *b, const int k) {
    init<mod>(k, w);
    ntt<mod>(a, k, 1);
    ntt<mod>(b, k, 1);
    for (register int i = 0; i < k; i++) a[i] = (ulong)a[i] * b[i] % mod;
    ntt<mod>(a, k, -1);
}

inline int crt(register int a1, register int a2, register int a3) {
    register ulong a = ((ulong)MOD2 * ((ulong)a1 * MOD2_INV_MOD1 % MOD1) +
                        (ulong)MOD1 * ((ulong)a2 * MOD1_INV_MOD2 % MOD2)) %
                       MOD1_MOD2;
    return (a + (MOD3 + a3 - a % MOD3) % MOD3 * MOD1_MOD2_INV_MOD3 % MOD3 *
                    MOD1_MOD2_MOD % MOD) %
           MOD;
}

inline void solve() {
    register int n, k = 1;
    io >> n;
    for (register int i = 0; i < n; i++) io >> a[i];
    for (register int i = 0; i < n; i++) io >> b[i];
    for (; k < n + n;) k <<= 1;
    memcpy(ans1, a, sizeof(int) * n);
    memcpy(tmp, b, sizeof(int) * k);
    mul<MOD1>(ans1, tmp, k);
    memcpy(ans2, a, sizeof(int) * n);
    memcpy(tmp, b, sizeof(int) * k);
    mul<MOD2>(ans2, tmp, k);
    mul<MOD3>(a, b, k);
    for (register int i = 0; i < n; i++)
        io << crt(ans1[i], ans2[i], ans3[i]) << ' ';
}
}  // namespace

int main() {
    freopen("annona_squamosa.in", "r", stdin);
    freopen("annona_squamosa.out", "w", stdout);
    solve();
    return 0;
}