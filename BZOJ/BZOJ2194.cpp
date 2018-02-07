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
inline void fwriteCharArray(const char *str, int len) {
    for (register int i = 0; i < len; i++) *oh++ = str[i];
}
#define MIN_VALUE 0x80000000
const int sizeTable[] = {9,      99,      999,      9999,      99999,
                         999999, 9999999, 99999999, 999999999, 0x7fffffff};
inline int stringSize(int x) {
    for (int i = 0;; i++)
        if (x <= sizeTable[i]) return i + 1;
}
const char DigitOnes[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4',
    '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4',
    '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '1', '2', '3', '4',
    '5', '6', '7', '8', '9', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
};
const char DigitTens[] = {
    '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '1', '1', '1', '1',
    '1', '1', '1', '1', '1', '2', '2', '2', '2', '2', '2', '2', '2', '2', '2',
    '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '4', '4', '4', '4', '4',
    '4', '4', '4', '4', '4', '5', '5', '5', '5', '5', '5', '5', '5', '5', '5',
    '6', '6', '6', '6', '6', '6', '6', '6', '6', '6', '7', '7', '7', '7', '7',
    '7', '7', '7', '7', '7', '8', '8', '8', '8', '8', '8', '8', '8', '8', '8',
    '9', '9', '9', '9', '9', '9', '9', '9', '9', '9',
};
const char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
                       '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                       'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
                       'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
inline void getChars(int i, int index, char buf[]) {
    int q, r;
    int charPos = index;
    char sign = 0;
    if (i < 0) {
        sign = '-';
        i = -i;
    }
    while (i >= 65536) {
        q = i / 100;
        r = i - ((q << 6) + (q << 5) + (q << 2));
        i = q;
        buf[--charPos] = DigitOnes[r];
        buf[--charPos] = DigitTens[r];
    }
    for (;;) {
        q = ((unsigned int)(i * 52429)) >> (16 + 3);
        r = i - ((q << 3) + (q << 1));
        buf[--charPos] = digits[r];
        i = q;
        if (i == 0) break;
    }
    if (sign != 0) {
        buf[--charPos] = sign;
    }
}
inline void write(int i) {
    if (i == MIN_VALUE) fwriteCharArray("-2147483648", 11);
    register int size = (i < 0) ? stringSize(-i) + 1 : stringSize(i);
    char buf[size];
    getChars(i, size, buf);
    fwriteCharArray(buf, size);
}
int n, m, c[MAXN];
int main() {
    // freopen("in.in", "r", stdin);
    fread(ibuf, 1, IN_LEN, stdin), read(n), n--, fft.init(n, n), m = n << 1;
    for (register int i = 0, j; i <= n; i++)
        read(j), a[i].r = j, read(j), a[n - i].i = j;
    fft.multiply(a, b);
    for (register int i = m >> 1; i <= m; i++)
        write(static_cast<int>(b[i].r + 0.1)), *oh++ = '\n';
    fwrite(obuf, 1, oh - obuf, stdout);
    return 0;
}