/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「COGS 2294」释迦 08-01-2018
 * 拆系数 FFT (朴素)
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
const int MAXN = 1 << 18 | 1;
#define double long double
const double PI = 3.1415926535897932384626;

typedef unsigned long long ulong;

struct Complex {
    double r, i;

    inline Complex operator*(const Complex &x) const {
        return (Complex){r * x.r - i * x.i, r * x.i + i * x.r};
    }

    inline Complex operator+(const Complex &x) const {
        return (Complex){r + x.r, i + x.i};
    }

    inline Complex operator-(const Complex &x) const {
        return (Complex){r - x.r, i - x.i};
    }

    inline void operator+=(const Complex &x) {
        r += x.r;
        i += x.i;
    }

    inline Complex conj() const { return (Complex){r, -i}; }
} w[MAXN], a[MAXN], b[MAXN], c[MAXN], d[MAXN];

inline void fft(Complex *a, const int n, const int f) {
    for (register int i = 0, j = 0; i < n; i++) {
        (i > j) && (std::swap(a[i], a[j]), 0);
        for (register int k = n >> 1; (j ^= k) < k; k >>= 1)
            ;
    }
    for (register int i = 1; i < n; i <<= 1) {
        const register Complex *w = ::w + i;
        for (register int j = 0; j < n; j += i << 1) {
            register Complex *x = a + j, *y = a + i + j, t;
            for (register int k = 0; k < i; k++) {
                t = w[k] * y[k];
                y[k] = x[k] - t;
                x[k] += t;
            }
        }
    }
    if (f == -1) {
        std::reverse(a + 1, a + n);
        for (register int i = 0; i < n; i++) a[i].r /= n;
    }
}

inline void init(const int k) {
    register int n = k >> 1;
    w[n] = (Complex){1, 0};
    for (register int i = 1; i < n; i++)
        w[n + i] = (Complex){cos(2 * PI * i / k), sin(2 * PI * i / k)};
    for (register int i = n - 1; i > 0; i--) w[i] = w[i << 1];
}

inline void solve() {
    register int n, k = 1;
    io >> n;
    for (register int i = 0, x; i < n; i++) {
        io >> x;
        a[i].r = x >> 15;
        b[i].r = x & 32767;
    }
    for (register int i = 0, x; i < n; i++) {
        io >> x;
        c[i].r = x >> 15;
        d[i].r = x & 32767;
    }
    for (; k < n + n;) k <<= 1;
    init(k);
    fft(a, k, 1);
    fft(b, k, 1);
    fft(c, k, 1);
    fft(d, k, 1);
    for (register int i = 0; i < k; i++) {
        Complex ta = a[i], tb = b[i];
        a[i] = a[i] * c[i];
        b[i] = ta * d[i] + b[i] * c[i];
        c[i] = tb * d[i];
    }
    fft(a, k, -1);
    fft(b, k, -1);
    fft(c, k, -1);
    for (register int i = 0; i < n; i++) {
        register ulong ta = (ulong)floor(a[i].r + 0.5) % MOD,
                       tb = (ulong)floor(b[i].r + 0.5) % MOD,
                       tc = (ulong)floor(c[i].r + 0.5) % MOD;
        io << ((ta << 30) % MOD + (tb << 15) % MOD + tc) % MOD << ' ';
    }
}
}  // namespace

int main() {
    freopen("annona_squamosa.in", "r", stdin);
    freopen("annona_squamosa.out", "w", stdout);
    solve();
    return 0;
}