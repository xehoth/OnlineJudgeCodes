/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 4723」Flappy Bird 06-02-2018
 *
 * @author xehoth
 */
#include <algorithm>
#include <cstdio>
#include <iostream>

struct InputOutputStream {
    enum { SIZE = 1 << 18 | 1 };
    char ibuf[SIZE], *s, *t;

    InputOutputStream() : s(), t() {}

    inline char read() {
        return (s == t) && (t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin)),
               s == t ? -1 : *s++;
    }

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
} io;

const int MAXN = 500000 + 1;
int x[MAXN], a[MAXN], b[MAXN], A, B;

int main() {
    int n;
    io >> n >> x[0];
    x[0] = 0;
    for (int i = 1; i <= n; i++) io >> x[i] >> a[i] >> b[i];
    for (int i = 1, tmp; i <= n; i++) {
        tmp = x[i] - x[i - 1];
        A = std::max(A - tmp, a[i] + 1);
        B = std::min(B + tmp, b[i] - 1);
        if ((A & 1) != (x[i] & 1)) A++;
        if ((B & 1) != (x[i] & 1)) B--;
        if (A > B) {
            std::cout << "NIE";
            return 0;
        }
    }
    std::cout << (A + x[n]) / 2;
    return 0;
}