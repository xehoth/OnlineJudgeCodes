#include <bits/stdc++.h>
inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    if (s == t) {
        t = (s = buf) + fread(buf, 1, IN_LEN, stdin);
        if (s == t) return -1;
    }
    return *s++;
}

template <class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == '-') iosig = true;
        if (c == -1) return;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    if (iosig) x = -x;
}

const int OUT_LEN = 1000000;
char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}

template <class T>
inline void print(T x) {
    static char buf[30];
    if (x == 0) {
        print('0');
    } else {
        if (x < 0) print('-'), x = -x;
        register int cnt = 0;
        for (; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print(buf[cnt--]);
    }
}

template <class T>
inline void println(T x) {
    print(x), print('\n');
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

#define LL long long
using namespace std;

const int MAXN = 100000;
const double EPS = 1e-5;

namespace Hash {
const int MOD = 99971;
int head[MAXN], nxt[MAXN], val[MAXN], data[MAXN], T;

inline void init() {
    T = 0;
    memset(head, 0, sizeof(head));
}

inline void insert(int w, int d) {
    nxt[++T] = head[w % MOD];
    val[T] = w;
    data[T] = d;
    head[w % MOD] = T;
}

inline int find(int w) {
    for (int i = head[w % MOD]; i; i = nxt[i])
        if (val[i] == w) return data[i];
    return -1;
}
};  // namespace Hash

inline int quick_pow(int a, int b, int c) {
    int ret = 1;
    while (b) {
        if (b & 1) ret = ((LL)ret * a) % c;
        a = (LL)a * a % c;
        b >>= 1;
    }
    return ret;
}

inline int GCD(int a, int b) { return b ? GCD(b, a % b) : a; }

inline void EX_GCD(int a, int b, int &x, int &y) {
    if (!b)
        x = 1, y = 0;
    else
        EX_GCD(b, a % b, y, x), y -= a / b * x;
}

inline int EX_GCD(int a, int b, int c) {
    int ret, tmp, gcd = GCD(a, b);
    if (c % gcd)
        return -1;
    else {
        EX_GCD(a / gcd, b / gcd, ret, tmp);
        ret = (((LL)c / gcd * ret) % b + b) % b;
        return ret;
    }
}

inline void solve(int a, int b, int c) {
    int ret = EX_GCD(a, c, b);
    if (~ret)
        printf("%d\n", ret);
    else
        printf("Orz, I cannot find x!\n");
}

inline void BSGS(int a, int b, int c) {
    Hash::init();
    a %= c;
    b %= c;
    if (!a && b > 1) {
        printf("Orz, I cannot find x!\n");
        return;
    }

    int lim = int(ceil(sqrt(c)) + EPS);
    for (int i = 0, w = 1; i <= lim; i++) {
        if (w == b) {
            printf("%d\n", i);
            return;
        }
        Hash::insert(w, i);
        w = ((LL)w * a) % c;
    }

    int w_ori = quick_pow(a, lim, c), rev_ori = quick_pow(w_ori, c - 2, c);
    for (int i = 1, w = w_ori, rev = rev_ori; i <= lim; i++) {
        int tmp = Hash::find(((LL)rev * b) % c);
        if (tmp > -1) {
            printf("%d\n", i * lim + tmp);
            return;
        }
        w = ((LL)w * w_ori) % c;
        rev = ((LL)rev * rev_ori) % c;
    }
    printf("Orz, I cannot find x!\n");
}

int main() {
    int T, ty;
    read(T), read(ty);
    while (T--) {
        register int a, b, c;
        read(a), read(b), read(c);
        if (ty == 1)
            printf("%d\n", quick_pow(a, b, c));
        else if (ty == 2)
            solve(a, b, c);
        else
            BSGS(a, b, c);
    }
    return 0;
}