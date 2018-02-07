/*
 * created by xehoth on 19-04-2017
 */
#include <bits/stdc++.h>

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

inline int read(char *buf) {
    register char c;
    register int s = 0;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = '\0';
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = '\0';
    return s;
}

inline int getPos(int x, int y) { return 9 * (x - 1) + y; }

const int MOD = 2009;

struct Matrix {
    int r, c, num[95][95];

    Matrix(int r = 0, int c = 0) : r(r), c(c), num() {}

    inline void init() {
        for (register int i = 1; i <= r; i++) num[i][i] = 1;
    }

    inline Matrix operator*(const Matrix &b) const {
        Matrix d(r, b.c);
        for (register int i = 1; i <= r; i++) {
            for (register int k = 1; k <= c; k++) {
                for (register int j = 1; j <= b.c; j++) {
                    d.num[i][j] += num[i][k] * b.num[k][j];
                }
            }
        }
        for (register int i = 1; i <= r; i++) {
            for (register int j = 1; j <= c; j++) {
                d.num[i][j] %= MOD;
            }
        }
        return d;
    }
} A;

int n, t;

char s[12];

inline Matrix operator^(Matrix a, int b) {
    Matrix d(a.r, a.c);
    for (d.init(); b; b >>= 1, a = a * a) (b & 1) ? d = d * a : 0;
    return d;
}

int main() {
    //    freopen("in.in", "r", stdin);
    read(n), read(t);
    A.r = A.c = 9 * n;
    for (register int i = 1; i <= n; i++)
        for (register int j = 1; j <= 8; j++)
            A.num[getPos(i, j)][getPos(i, j + 1)] = 1;
    for (register int i = 1; i <= n; i++) {
        read(s + 1);
        for (register int j = 1; j <= n; j++)
            if (s[j] - '0') A.num[getPos(i, s[j] - '0')][getPos(j, 1)] = 1;
    }
    printf("%d", (A ^ t).num[1][getPos(n, 1)]);
    return 0;
}
