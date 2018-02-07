#include <bits/stdc++.h>

long long n, Mod;

struct Matrix {
    int x, y;
    long long a[5][5];
    void clear() { memset(a, 0, sizeof(a)); }
    void init(int a, int b) {
        x = a;
        y = b;
    }
} m0, mt;

long long modPow(long long a, long long b) {
    long long f = a, ret = 0;
    while (b) {
        if (b & 1) {
            ret += f;
            if (ret > Mod) ret %= Mod;
        }
        b >>= 1, f <<= 1;
        if (f > Mod) f %= Mod;
    }
    return ret;
}

inline Matrix nul(const Matrix &Ma, const Matrix &Mb) {
    Matrix ret;
    ret.init(Ma.x, Mb.y);
    ret.clear();
    for (register int i = 1; i <= ret.x; i++) {
        for (register int j = 1; j <= ret.y; j++) {
            for (register int k = 1; k <= Ma.y; k++) {
                ret.a[i][j] += modPow(Ma.a[i][k], Mb.a[k][j]);
                ret.a[i][j] %= Mod;
            }
        }
    }
    return ret;
}

Matrix powm(Matrix Ma, long long b) {
    Matrix f, ret;
    f = Ma;
    ret.init(Ma.x, Ma.y);
    ret.clear();
    for (int i = 1; i <= ret.x; i++) ret.a[i][i] = 1;
    while (b) {
        if (b & 1) ret = nul(ret, f);
        b >>= 1;
        f = nul(f, f);
    }
    return ret;
}

int main() {
    scanf("%lld%lld", &n, &Mod);
    long long Temp, Ans;
    m0.init(1, 3);
    m0.clear();
    m0.a[1][1] = 0;
    m0.a[1][2] = 1;
    m0.a[1][3] = 1;
    mt.init(3, 3);
    Temp = 1;
    for (int i = 1; i <= 18; i++) {
        Temp *= 10ll;
        if (Temp > n) break;
        mt.clear();
        mt.a[1][1] = Temp;
        mt.a[2][1] = mt.a[2][2] = mt.a[3][2] = mt.a[3][3] = 1;
        mt = powm(mt, Temp - Temp / 10);
        m0 = nul(m0, mt);
    }
    mt.clear();
    mt.a[1][1] = Temp;
    mt.a[2][1] = mt.a[2][2] = mt.a[3][2] = mt.a[3][3] = 1;
    mt = powm(mt, n - Temp / 10 + 1);
    m0 = nul(m0, mt);
    Ans = m0.a[1][1];
    printf("%lld\n", Ans);
    return 0;
}