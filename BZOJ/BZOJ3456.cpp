#include <bits/stdc++.h>

namespace {

inline char read() {
    static const int IN_LEN = 100000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
}

const int OUT_LEN = 100000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('-');
    } else {
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
    ~InputOutputStream() { flush(); }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

typedef unsigned int uint;
typedef unsigned long long ulong;

const int MOD = 1004535809;
const int G = 3;
const int MAXN = 1 << 18 | 1;

int w[MAXN] = {1};

inline uint modPow(register uint a, register uint b) {
    register uint ret = 1;
    for (; b; b >>= 1, a = (ulong)a * a % MOD)
        if (b & 1) ret = (ulong)ret * a % MOD;
    return ret;
}

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
                t = (ulong)w[k] * y[k] % MOD;
                y[k] = (x[k] - t <= 0 ? x[k] - t + MOD : x[k] - t);
                x[k] = (x[k] + t >= MOD ? x[k] + t - MOD : x[k] + t);
            }
        }
    }
    if (f == -1) {
        std::reverse(a + 1, a + n);
        register const int inv = modPow(n, MOD - 2);
        for (register int i = 0; i < n; i++) a[i] = (ulong)a[i] * inv % MOD;
    }
}

inline int dec(register int a, const register int v) {
    return a - v <= 0 ? a - v + MOD : a - v;
}

struct Polynomial {
    int deg;
    int a[MAXN];

    void inverse(const int n, Polynomial &p) {
        if (n == 1) return (void)(p.deg = 1, p.a[0] = modPow(a[0], MOD - 2));
        static int tmp[MAXN];
        inverse(n + 1 >> 1, p);
        register int k = 1;
        for (; k < n << 1;) k <<= 1;
        memcpy(tmp, a, sizeof(int) * n);
        memset(tmp + n, 0, sizeof(int) * (k - n + 1));
        ntt(tmp, k, 1);
        memset(p.a + p.deg, 0, sizeof(int) * (k - p.deg + 1));
        ntt(p.a, k, 1);
        for (register int i = 0; i < k; i++)
            p.a[i] = (ulong)p.a[i] * dec(2, (ulong)tmp[i] * p.a[i] % MOD) % MOD;
        ntt(p.a, k, -1), p.deg = n;
    }
};

inline void init(const int k) {
    register int bn = k >> 1;
    w[bn + 0] = 1;
    w[bn + 1] = modPow(G, (MOD - 1) / k);
    for (register int i = 2; i < bn; i++)
        w[bn + i] = (unsigned long long)w[bn + i - 1] * w[bn + 1] % MOD;
    for (register int i = bn - 1; i > 0; i--) w[i] = w[i << 1];
}

Polynomial A, B, C;
int inv[MAXN], invFac[MAXN], choose[MAXN];

inline void solve() {
    register int n;
    io >> n;
    register int p = 1;
    for (; p < (n + 1) << 1;) p <<= 1;
    inv[1] = invFac[0] = invFac[1] = 1;
    for (register uint i = 2; i <= n; i++) {
        inv[i] = (MOD - (ulong)MOD / i) * inv[MOD % i] % MOD;
        invFac[i] = (ulong)invFac[i - 1] * inv[i] % MOD;
    }
    choose[0] = choose[1] = 1;
    for (register int i = 2; i <= n; i++)
        choose[i] = modPow(2u, (ulong)i * (i - 1ull) / 2ull % (MOD - 1u));

    A.deg = B.deg = n + 1;
    for (register int i = 0; i <= n; i++)
        A.a[i] = (ulong)choose[i] * invFac[i] % MOD;
    for (register int i = 1; i <= n; i++)
        B.a[i] = (ulong)choose[i] * invFac[i - 1] % MOD;
    register int k = 1;
    for (register int l = n + 1 << 1; k < l;) k <<= 1;
    init(k);
    A.inverse(n + 1, C);
    memset(C.a + C.deg, 0, sizeof(int) * (p - C.deg + 1));
    ntt(C.a, p, 1), ntt(B.a, p, 1);
    for (register int i = 0; i <= p; i++) C.a[i] = (ulong)C.a[i] * B.a[i] % MOD;
    ntt(C.a, p, -1);
    register int ans = (ulong)C.a[n] * modPow(invFac[n - 1], MOD - 2) % MOD;
    if (ans < 0) ans += MOD;
    io << ans;
}

}  // namespace

int main() {
    solve();
    return 0;
}