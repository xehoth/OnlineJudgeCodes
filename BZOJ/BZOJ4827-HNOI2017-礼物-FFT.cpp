/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4827」礼物 23-09-2017
 * FFT
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline bool read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return false;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
    return true;
}

inline void read(char &c) {
    while (c = read(), isspace(c) && c != -1)
        ;
}

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = 0;
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = 0;
    return s;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
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

    ~InputOutputStream() { flush(); }
} io;
}

namespace FastFourierTansform {

const int MAXN = 262144 / 2;

struct Complex {
    double r, i;

    Complex(double r = 0, double i = 0) : r(r), i(i) {}

    inline Complex operator+(const Complex &x) const {
        return Complex(r + x.r, i + x.i);
    }

    inline Complex operator-(const Complex &x) const {
        return Complex(r - x.r, i - x.i);
    }

    inline Complex operator*(const Complex &x) const {
        return Complex(r * x.r - i * x.i, r * x.i + i * x.r);
    }

    inline void operator*=(const Complex &x) {
        register double t = r;
        r = r * x.r - i * x.i, i = t * x.i + i * x.r;
    }

    inline void operator+=(const Complex &x) { r += x.r, i += x.i; }

    inline Complex conj() const { return Complex(r, -i); }
} w[MAXN + 1] = Complex(1, 0);

const double PI = acos(-1);

inline void fft(Complex *a, const int n, const int f) {
    for (register int i = 0, j = 0; i < n; i++) {
        i > j ? std::swap(a[i], a[j]) : (void)0;
        for (register int k = n >> 1; (j ^= k) < k; k >>= 1)
            ;
    }
    for (register int i = 1; i < n; i <<= 1) {
        w[1] = Complex(cos(PI / i), f * sin(PI / i));
        for (register int j = 2; j < i; j++) w[j] = w[j - 1] * w[1];
        for (register int j = 0; j < n; j += i << 1) {
            register Complex *x = a + j, *y = a + i + j;
            for (register int k = 0; k < i; k++) {
                Complex t = w[k] * y[k];
                y[k] = x[k] - t, x[k] += t;
            }
        }
    }
    if (f == -1)
        for (register int i = 0; i < n; i++) a[i].r /= n, a[i].i /= n;
}
}

namespace {

using IO::io;
using FastFourierTansform::Complex;
using FastFourierTansform::fft;

const int MAXN = 50000;
const int MAXM = 262144 / 2;
int x[MAXN + 1], y[MAXN + 1];

Complex xc[MAXM * 2 + 1], yc[MAXM + 1], c[MAXM + 1];

inline void solve() {
    register int n, m, sumDelta = 0, sumSquare = 0;
    io >> n >> m;
    for (register int i = 0; i < n; i++)
        io >> x[i], sumDelta += x[i], sumSquare += x[i] * x[i];
    for (register int i = 0; i < n; i++)
        io >> y[i], sumDelta -= y[i], sumSquare += y[i] * y[i];
    register int mid1 = floor((double)sumDelta / -n),
                 mid2 = ceil((double)sumDelta / -n);
    // x ^ 2 + y ^ 2 + 2v(x - y) + v ^ 2
    // sumSquare + 2 * v * sumDelta + n * v * v
    register int ans =
        std::min(sumSquare + 2 * mid1 * sumDelta + n * mid1 * mid1,
                 sumSquare + 2 * mid2 * sumDelta + n * mid2 * mid2);
    // exit(0);
    // - 2xy
    for (register int i = 0; i < n; i++)
        (i & 1) ? xc[i >> 1].i = x[n - i - 1] : xc[i >> 1].r = x[n - i - 1];
    for (register int i = 0; i < n; i++)
        (i & 1) ? yc[i >> 1].i = y[i] : yc[i >> 1].r = y[i];
    for (register int i = n, r = n * 2; i < r; i++)
        (i & 1) ? yc[i >> 1].i = y[i - n] : yc[i >> 1].r = y[i - n];
    register int k = 1;
    for (register int limit = 3 * n / 2; k < limit;) k <<= 1;
    fft(xc, k, 1), fft(yc, k, 1);
    const Complex t(1, 0);
    using FastFourierTansform::w;
    for (register int i = 0, j; i < k; i++) {
        j = k - i & k - 1;
        c[i] = (xc[i] * yc[i] * 4 -
                (xc[i] - (xc[j].conj())) * (yc[i] - (yc[j].conj())) *
                    (((i & k >> 1) ? t - w[i ^ k >> 1] : w[i] + t))) *
               0.25;
    }
    fft(c, k, -1);
    for (register int i = 0, r = k << 1; i < r; i++) {
        (i & 1) ? xc[i].r = c[i >> 1].i : xc[i].r = c[i >> 1].r;
    }
    register int max = INT_MIN;
    for (register int i = n - 1, r = 2 * n - 1; i < r; i++)
        max = std::max(max, (int)round(xc[i].r));
    io << ans - max * 2;
}
}

int main() {
    // freopen("gift.in", "r", stdin);
    // freopen("gift.out", "w", stdout);
    solve();
    return 0;
}