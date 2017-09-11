/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「51NOD 1195」斐波那契数列的循环节 11-09-2017
 * 二次剩余
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

inline void print(const char *s) {
    for (; *s; s++) print(*s);
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
typedef unsigned long long ulong;
#define long long long
const int MAXN = 100000;
const int MAX_BUC = 1000000;

struct Fibonacci {
    int k, b[2], c[2];

    inline void mulMod(int *x, int *y, const int MOD) {
        register int tmp = (((ulong)x[0] * y[0]) + ((ulong)x[1] * y[1])) % MOD;
        x[1] = (((ulong)x[0] + x[1]) * y[1] + (ulong)x[1] * y[0]) % MOD;
        x[0] = tmp;
    }

    inline void pow(int *a, int b, int *ans, const int MOD) {
        for (; b; b >>= 1, mulMod(a, a, MOD))
            (b & 1) ? mulMod(ans, a, MOD) : (void)0;
    }

    inline int fix(int b, const int MOD) { return b >= MOD ? b - MOD : b; }

    inline int getLinearRecursion(const int n, const int MOD) {
        c[0] = 0, b[1] = 0, c[1] = 1, b[0] = 1, pow(c, n, b, MOD);
        return fix(b[0] + b[1], MOD);
    }

    inline bool check(const int n, const int MOD) {
        c[0] = 0, b[1] = 0, c[1] = 1, b[0] = 1, pow(c, n, b, MOD);
        return b[0] == 1 && b[1] == 0;
    }
};

template <typename T>
inline T gcd(T x, T y) {
    for (register T t = 0; y != 0;) t = x % y, x = y, y = t;
    return x;
}

struct Task {
    Fibonacci fib;

    int prime[MAXN + 1], pcnt;
    bool vis[MAXN + 1];

    inline void fastLinearSieveMethod() {
        prime[0] = 2, prime[1] = 3, prime[2] = 5, prime[3] = 7;
        pcnt = 4;
        for (register int i = 11; i <= MAXN; i += 2) {
            if (!vis[i]) prime[pcnt++] = i;
            for (register int j = 0, t; j < pcnt && (t = i * prime[j]) <= MAXN;
                 j++) {
                vis[t] = true;
                if (i % prime[j] == 0) break;
            }
        }
    }

    long buc[MAX_BUC + 1], fs[MAX_BUC + 1];

    inline long modPow(long a, long b, const int MOD) {
        register long ret = 1;
        for (; b; b >>= 1, a = a * a % MOD) (b & 1) ? ret = ret * a % MOD : 0;
        return ret;
    }

    inline bool isQuadraticResidue(const int n, const int p) {
        return modPow(n, p - 1 >> 1, p) == 1;
    }

    int l, x, fac[100][2];

    inline void getFactorT(long count, int step) {
        if (step == l) {
            fs[x++] = count;
            return;
        }
        register long sum = 1;
        for (register int i = 0; i < fac[step][1]; i++)
            sum *= fac[step][0], getFactorT(count * sum, step + 1);
        getFactorT(count, step + 1);
    }

    inline long solvePrime(const int p) {
        if (p <= MAX_BUC && buc[p]) return buc[p];
        register int t = (isQuadraticResidue(5, p) ? p - 1 : 2 * p + 2);
        l = 0;
        for (register int i = 0; i < pcnt; i++) {
            if (prime[i] > t / prime[i]) break;
            if (t % prime[i] == 0) {
                register int count = 0;
                fac[l][0] = prime[i];
                while (t % prime[i] == 0) count++, t /= prime[i];
                fac[l++][1] = count;
            }
        }
        if (t > 1) fac[l][0] = t, fac[l++][1] = 1;
        x = 0, getFactorT(1, 0);
        std::sort(fs, fs + x);
        for (register int i = 0; i < x; i++) {
            if (fib.check(fs[i], p)) {
                if (p <= MAX_BUC) buc[p] = fs[i];
                return fs[i];
            }
        }
    }

    inline long modPow(long a, long b) {
        register long ret = 1;
        for (; b; b >>= 1, a = a * a) (b & 1) ? ret = ret * a : 0;
        return ret;
    }

    inline long solve(int n) {
        register long ans = 1, cnt = 0;
        for (register int i = 0; i < pcnt; i++) {
            if (prime[i] > n / prime[i]) break;
            if (n % prime[i] == 0) {
                register int count = 0;
                while (n % prime[i] == 0) count++, n /= prime[i];
                cnt = modPow(prime[i], count - 1) * solvePrime(prime[i]);
                ans = (ans / gcd(ans, cnt)) * cnt;
            }
        }
        if (n > 1) cnt = solvePrime(n), ans = ans / gcd(ans, cnt) * cnt;
        return ans;
    }

    inline void solve() {
        fastLinearSieveMethod();
        register int t;
        buc[2] = 3, buc[3] = 8, buc[5] = 20;
        io >> t;
        for (register int n; t--;) {
            io >> n;
            io << solve(n) << '\n';
        }
    }
} task;
#undef long
}

int main() {
    task.solve();
    return 0;
}