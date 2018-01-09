/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「COGS 2294」释迦 08-01-2018
 * 拆系数 FFT (合并)
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

int ans[MAXN];

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
        for (register int i = 0; i < n; i++) {
            a[i].r /= n;
            a[i].i /= n;
        }
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
        a[i].r = x >> 13;
        a[i].i = x & 8191;
    }
    for (register int i = 0, x; i < n; i++) {
        io >> x;
        b[i].r = x >> 13;
        b[i].i = x & 8191;
    }
    for (; k < n + n;) k <<= 1;
    init(k);
    fft(a, k, 1);
    fft(b, k, 1);
    Complex t1, t2, t3, t4;
    const Complex o1 = (Complex){0.5, 0}, o2 = (Complex){0, -0.5};
    const Complex o = (Complex){0, 1};
    for (register int i = 0, j; i < k; i++) {
        j = (k - i) & (k - 1);
        t1 = (a[i] + a[j].conj()) * o1;
        t2 = (a[i] - a[j].conj()) * o2;
        t3 = (b[i] + b[j].conj()) * o1;
        t4 = (b[i] - b[j].conj()) * o2;
        c[i] = (t1 * t3) + o * (t2 * t4);
        d[i] = (t1 * t4) + o * (t2 * t3);
    }
    fft(c, k, -1);
    fft(d, k, -1);

    for (register int i = 0, v1, v2, v3, v4; i < k; i++) {
        v1 = (ulong)round(c[i].r) % MOD;
        v2 = (ulong)round(c[i].i) % MOD;
        v3 = (ulong)round(d[i].r) % MOD;
        v4 = (ulong)round(d[i].i) % MOD;
        ans[i] = (v2 + ((ulong)v1 << 26) + (((ulong)v3 + v4) << 13)) % MOD;
    }
    for (register int i = 0; i < n; i++) io << ans[i] << '\n';
}
}  // namespace

int main() {
    freopen("annona_squamosa.in", "r", stdin);
    freopen("annona_squamosa.out", "w", stdout);
    solve();
    return 0;
}