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
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
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
}  // namespace IO

namespace Task {

typedef long double ld;
#define long long long

inline long mul(long a, long b, long c) {
    return (a * b - (long)((ld)a / c * b) * c + c) % c;
}

inline long modPow(long a, long b, long c) {
    register long ret = 1;
    for (; b; b >>= 1, a = mul(a, a, c)) (b & 1) ? ret = mul(ret, a, c) : 0;
    return ret;
}

inline long check(long a, long n, long r, long s) {
    register long x = modPow(a, r, n), pre = x;
    for (register int i = 1; i <= s; i++) {
        x = mul(x, x, n);
        if (x == 1 && pre != 1 && pre != n - 1) return 0;
        pre = x;
    }
    return x == 1;
}

const long PRIMES[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

inline bool millerRabin(long n) {
    if (n <= 1) return 0;
    register long r = n - 1, s = 0;
    while (~r & 1) r >>= 1, s++;
#define opt(i)                       \
    if (PRIMES[i] == n) return true; \
    if (!check(PRIMES[i], n, r, s)) return false
    opt(0);
    opt(1);
    opt(2);
    opt(3);
    opt(4);
    opt(5);
    opt(6);
    opt(7);
    opt(8);
#undef opt
    return true;
}

inline long pollardRho(long n, long c) {
    register long k = 2, x = rand() % n, y = x, p = 1;
    for (register long i = 1; p == 1; i++) {
        x = (mul(x, x, n) + c) % n;
        p = std::__gcd(n, y > x ? y - x : x - y);
        (i == k) ? (y = x, k <<= 1) : 0;
    }
    return p;
}

long max;

inline void solve(long n) {
    if (n == 1) return;
    if (millerRabin(n)) {
        max = std::max(max, n);
        return;
    }
    register long t = n;
    while (t == n) t = pollardRho(n, rand() % (n - 1));
    solve(t), solve(n / t);
}

inline void solve() {
    srand(495);
    using namespace IO;
    register int T;
    register long x;
    for (read(T); T--;) {
        max = 0;
        read(x), solve(x);
        max == x ? print("Prime\n") : (print(max), print('\n'));
    }
}
}  // namespace Task

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    Task::solve();
    IO::flush();
    return 0;
}