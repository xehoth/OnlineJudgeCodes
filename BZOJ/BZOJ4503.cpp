/*
 * craeted by xehoth on 25-02-2017
 */
#include <bits/stdc++.h>

const int OUT_LEN = 1000000;
char obuf[OUT_LEN], *oh = obuf;
char cntbuf[20], *cbh = cntbuf;

inline void print(char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}

template <class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        if (x < 0) print('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

template <class T>
inline void printCount(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        *cbh++ = 48;
    } else {
        if (x < 0) *cbh++ = '-', x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) *cbh++ = buf[cnt--];
    }
}

inline void flush() {
    *cbh++ = '\n';
    fwrite(cntbuf, 1, cbh - cntbuf, stdout);
    fwrite(obuf, 1, oh - obuf, stdout);
}

const int MAXN = 100000;

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
} a[MAXN << 2], b[MAXN << 2];

const double PI = acos(-1);

inline void fft(Complex *a, const int n, const int f) {
    register int i, j, k;
    for (i = j = 0; i < n; i++) {
        if (i > j) std::swap(a[i], a[j]);
        for (k = n >> 1; (j ^= k) < k; k >>= 1)
            ;
    }
    for (i = 1; i < n; i <<= 1) {
        Complex wn(cos(PI / i), f * sin(PI / i));
        for (j = 0; j < n; j += i << 1) {
            Complex w(1, 0);
            for (k = 0; k < i; k++, w = w * wn) {
                Complex x = a[j + k], y = w * a[i + j + k];
                a[j + k] = x + y, a[i + j + k] = x - y;
            }
        }
    }
    if (f == -1)
        for (i = 0; i < n; i++) a[i].r /= n;
}

char s[MAXN + 10], t[MAXN + 10];

Complex A[MAXN << 2], B[MAXN << 2], C[MAXN << 2];
int lent, lens;

inline void solve() {
    static int a[MAXN << 2], b[MAXN << 2];
    for (register int i = 0; i < lens; i++) a[i] = s[i] - 'a' + 1;
    for (register int i = 0; i < lent; i++)
        b[lent - i - 1] = t[i] == '?' ? 0 : t[i] - 'a' + 1;
    register int k, len;
    for (k = 1, len = 0; k < lens + lens; k <<= 1, len++)
        ;
    for (register int i = 0; i < k; i++)
        A[i].r = b[i] * b[i] * b[i], B[i].r = 1;
    fft(A, k, 1), fft(B, k, 1);
    for (register int i = 0; i < k; i++) C[i] = A[i] * B[i];
    for (register int i = 0; i < k; i++) A[i] = Complex(a[i] << 1, 0);
    for (register int i = 0; i < k; i++) B[i] = Complex(b[i] * b[i], 0);
    fft(A, k, 1), fft(B, k, 1);
    for (register int i = 0; i < k; i++) C[i] = C[i] - A[i] * B[i];
    for (register int i = 0; i < k; i++) A[i] = Complex(a[i] * a[i], 0);
    for (register int i = 0; i < k; i++) B[i] = Complex(b[i], 0);
    fft(A, k, 1), fft(B, k, 1);
    for (register int i = 0; i < k; i++) C[i] = C[i] + A[i] * B[i];
    fft(C, k, -1);
    register int num = 0;
    for (register int i = 0; i <= lens - lent; i++)
        if (C[i + lent - 1].r < 0.5) print(i), print('\n'), num++;
    printCount(num);
}

int main() {
    //  freopen("match.in", "r", stdin);
    //  freopen("match.out", "w", stdout);
    scanf("%s\n%s", s, t);
    lens = strlen(s), lent = strlen(t);
    solve();
    flush();
    return 0;
}