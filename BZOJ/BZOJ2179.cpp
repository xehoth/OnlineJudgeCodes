#include <bits/stdc++.h>
const int MAXN = 300000;
const double pi2 = 2 * acos(-1.0);
struct Complex {
    double r, i;
    Complex(double r = 0, double i = 0) : r(r), i(i) {}
    inline Complex operator+(const Complex &x) {
        return Complex(r + x.r, i + x.i);
    }
    inline Complex operator-(const Complex &x) {
        return Complex(r - x.r, i - x.i);
    }
    inline Complex operator*(const Complex &x) {
        return Complex(r * x.r - i * x.i, r * x.i + i * x.r);
    }
    inline Complex conj() { return Complex(r, -i); }
} a[MAXN], b[MAXN];
struct FFT {
    int k, pos[MAXN];
    inline void init(int n, int m) {
        for (k = 1; k <= n || k <= m; k <<= 1)
            ;
        k <<= 1;
        for (register int j = j = __builtin_ctz(k) - 1, i = 0; i < k; i++)
            pos[i] = pos[i >> 1] >> 1 | ((i & 1) << j);
    }
    inline void fft(Complex *a, const int n, const int t) {
        for (register int i = 1; i < n; i++)
            if (i < pos[i]) std::swap(a[i], a[pos[i]]);
        for (register int d = 0, m1, m2; 1 << d < n; d++) {
            m1 = 1 << d, m2 = m1 << 1;
            double tmp = pi2 / m2 * t;
            Complex _w(cos(tmp), sin(tmp));
            for (register int i = 0; i < n; i += m2) {
                Complex w(1, 0);
                for (register int j = 0; j < m1; j++) {
                    Complex &A = a[i + j + m1], &B = a[i + j], t = w * A;
                    A = B - t, B = B + t, w = w * _w;
                }
            }
        }
        if (t == -1)
            for (register int i = 0; i < n; i++) a[i].r /= n;
    }
    inline void multiply(Complex *a, Complex *b) {
        fft(a, k, 1);
        for (register int i = 0, j; i < k; i++)
            j = (k - i) & (k - 1),
            b[i] = (a[i] * a[i] - (a[j] * a[j]).conj()) * Complex(0, -0.25);
        fft(b, k, -1);
    }
} fft;
const int IN_LEN = 5001000, OUT_LEN = 2000000;
char ibuf[IN_LEN], obuf[OUT_LEN], *ih = ibuf, *oh = obuf;
inline void read(int &x) {
    for (x = 0; !isdigit(*ih); ih++)
        ;
    while (isdigit(*ih)) x = (x << 1) + (x << 3) + ((*ih++) ^ '0');
}
inline void write(int x) {
    static int buf[30], cnt;
    if (!x)
        *oh++ = 48;
    else {
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) *oh++ = buf[cnt--];
    }
}
int n, m, c[MAXN];
int main() {
    // freopen("in.in", "r", stdin);
    fread(ibuf, 1, IN_LEN, stdin), read(n), n--, fft.init(n, n);
    ih++;
    for (register int i = n; i >= 0; i--) a[i].r = (*ih++) ^ '0';
    ih++;
    for (register int i = n; i >= 0; i--) a[i].i = (*ih++) ^ '0';
    fft.multiply(a, b), m = n << 1;
    for (register int i = 0; i <= m; i++) c[i] = (int)(b[i].r + 0.1);
    for (register int i = 0; i <= m; i++) {
        if (c[i] >= 10) {
            c[i + 1] += c[i] / 10, c[i] %= 10;
            if (i == m) m++;
        }
    }
    for (register int i = m; i >= 0; i--) write(c[i]);
    fwrite(obuf, 1, oh - obuf, stdout);
    return 0;
}