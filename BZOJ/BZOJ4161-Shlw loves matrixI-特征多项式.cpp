/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4161」Shlw loves matrixI 07-09-2017
 * 特征多项式 O(k ^ 2 \log n)
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
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
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

namespace {

using IO::io;

const int MAXN = 4005;
const int MOD = 1e9 + 7;

struct LinearRecursion {
#define long long long

    int k, *a, b[MAXN + 1], c[MAXN + 1];

    inline void mulMod(int *x, int *y) {
        static int buc[MAXN + 1];
        memset(buc, 0, sizeof(int) * (k << 1));
        for (register int i = 0; i < k; i++)
            for (register int j = 0; j < k; j++)
                buc[i + j] = (buc[i + j] + (long)x[i] * y[j]) % MOD;
        // 对 f(M) = x ^ k - a_1 x ^ {k - 1} - \cdots - a_k 取模，模拟手算
        for (register int i = 2 * k - 2; i >= k; i--)
            for (register int j = 1; j <= k; j++)
                buc[i - j] = (buc[i - j] + (long)buc[i] * a[j]) % MOD;
        memcpy(x, buc, sizeof(int) * k);
    }

    inline void pow(int *a, int b, int *ans) {
        for (; b; b >>= 1, mulMod(a, a)) (b & 1) ? mulMod(ans, a) : (void)0;
    }

    inline int getLinearRecursion(const int n, const int k, int *a, int *h) {
        this->k = k, this->a = a;
        memset(c, 0, sizeof(int) * k);
        memset(b, 0, sizeof(int) * k);
        c[1] = 1, b[0] = 1;
        pow(c, n, b);
        register int ans = 0;
        for (register int i = 0; i < k; i++)
            ans = (ans + (long)b[i] * h[i]) % MOD;
        return ans;
    }
#undef long
};

struct Task {
    int a[MAXN + 1], h[MAXN + 1];
    LinearRecursion d;

    inline void solve() {
        register int n, k;
        io >> n >> k;
        for (register int i = 1; i <= k; i++)
            io >> a[i], a[i] < 0 ? a[i] += MOD : 0;
        for (register int i = 0; i < k; i++)
            io >> h[i], h[i] < 0 ? h[i] += MOD : 0;
        io << d.getLinearRecursion(n, k, a, h);
    }
} task;
}

int main() {
    task.solve();
    return 0;
}
