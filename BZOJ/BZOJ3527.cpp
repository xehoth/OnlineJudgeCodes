#include <bits/stdc++.h>

using namespace std;
#define final __attribute__((always_inline)) __attribute__((optimize("O3")))
const int MAXN = 100000;
const int MAXN_EXTENDED = 262144;
const long double PI = acos(-1);

struct FastFourierTransform {
    complex<long double> omega[MAXN_EXTENDED], omegaInverse[MAXN_EXTENDED];

    final void init(const int n) {
        for (register int i = 0; i < n; i++) {
            omega[i] = complex<long double>(cosl(2 * PI / n * i),
                                            sinl(2 * PI / n * i));
            omegaInverse[i] = conj(omega[i]);
        }
    }

    final void transform(complex<long double> *a, const int n,
                         const complex<long double> *omega) {
        int k = 0;
        while ((1 << k) != n) k++;
        for (register int i = 0; i < n; i++) {
            int t = 0;
            for (int j = 0; j < k; j++)
                if (i & (1 << j)) t |= (1 << (k - j - 1));
            if (t > i) swap(a[t], a[i]);
        }

        for (register int l = 2; l <= n; l *= 2) {
            const int m = l / 2;
            for (complex<long double> *p = a; p != a + n; p += l) {
                for (register int i = 0; i < m; i++) {
                    const complex<long double> t = omega[n / l * i] * p[i + m];
                    p[i + m] = p[i] - t;
                    p[i] += t;
                }
            }
        }
    }

    final void dft(complex<long double> *a, const int n) {
        transform(a, n, omega);
    }

    final void idft(complex<long double> *a, const int n) {
        transform(a, n, omegaInverse);
        for (register int i = 0; i < n; i++) a[i] /= n;
    }

    final void multiply(complex<long double> *a, complex<long double> *b,
                        const int n) {
        int size = 1;
        while (size < n * 2) size *= 2;
        init(size);
        dft(a, size);
        dft(b, size);
        for (register int i = 0; i < size; i++) a[i] *= b[i];
        idft(a, size);
    }
} fft;

int n;
long double q[MAXN + 1];

final void calc(const long double *q, long double *E) {
    static complex<long double> f[MAXN_EXTENDED], g[MAXN_EXTENDED];
    fill(f, f + MAXN_EXTENDED, 0);
    fill(g, g + MAXN_EXTENDED, 0);
    copy(q, q + n + 1, f);
    f[0] = g[0] = complex<long double>(0, 0);
    for (register int i = 1; i <= n; i++) {
        g[i] = complex<long double>(1.0 / pow(i, 2), 0);
    }

    fft.multiply(f, g, n + 1);
    for (register int i = 1; i <= n; i++) E[i] = f[i].real();
}

int main() {
    scanf("%d", &n);
    for (register int i = 1; i <= n; i++) {
        double x;
        scanf("%lf", &x);
        q[i] = x;
    }

    static long double E1[MAXN + 1], E2[MAXN + 1];
    calc(q, E1);
    reverse(q + 1, q + n + 1);
    calc(q, E2);

    for (register int i = 1; i <= n; i++)
        printf("%.3lf\n", static_cast<double>(E1[i] - E2[n - i + 1]));

    return 0;
}