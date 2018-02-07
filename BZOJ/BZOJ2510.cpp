#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 100000;
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

struct InputOutputStream {
    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }
} io;
}  // namespace IO

namespace {

int n, m, k;
double s;

struct Matrix {
    double a[1010];
} a, b, c;

inline const Matrix &operator*(const Matrix &a, const Matrix &b) {
    memset(c.a, 0, sizeof(double) * (n + 1));
    for (register int k = 1, j = 0, i; k <= n; k++)
        for (j++, i = 1; i <= n; i++)
            c.a[k] += a.a[i] * b.a[j], (--j == 0) ? (j = n) : 0;
    return c;
}

inline const Matrix &power(Matrix a, int k) {
    static Matrix ret;
    for (ret = a, k--; k; k >>= 1, a = a * a)
        (k & 1) ? (void)(ret = ret * a) : (void)0;
    return ret;
}

using IO::io;

inline void solve() {
    io >> n >> m >> k;
    for (register int i = 1, t; i <= n; i++) io >> t, b.a[i] = t;
    a.a[n] = 1.0 / m, a.a[1] = 1.0 - a.a[n], a = power(a, k);
    for (register int i = 1, j = 1, k; i <= n; i++) {
        for (s = 0, k = 1; k <= n; k++)
            s += a.a[j] * b.a[k], (++j == n + 1) ? j = 1 : 0;
        (--j == 0) ? j = n : 0, printf("%.3f\n", s);
    }
}

}  // namespace

int main() {
    solve();
    return 0;
}