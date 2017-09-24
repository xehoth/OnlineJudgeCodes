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

inline int read(char *buf) {
    register int s = 0;
    register char c;
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

inline void read(double &x) {
    static char buf[30];
    read(buf), x = atof(buf);
}

template <typename T1, typename T2>
inline void read(T1 &a, T2 &b) {
    read(a), read(b);
}

template <typename T1, typename T2, typename T3>
inline void read(T1 &a, T2 &b, T3 &c) {
    read(a), read(b), read(c);
}

template <typename T1, typename T2, typename T3, typename T4>
inline void read(T1 &a, T2 &b, T3 &c, T4 &d) {
    read(a), read(b), read(c), read(d);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
inline void read(T1 &a, T2 &b, T3 &c, T4 &d, T5 &e) {
    read(a), read(b), read(c), read(d), read(e);
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

template <typename T>
inline void println(T x) {
    print(x), print('\n');
}

template <typename T1, typename T2>
inline void print(T1 a, T2 b) {
    print(a), print(b);
}

template <typename T1, typename T2, typename T3>
inline void print(T1 a, T2 b, T3 c) {
    print(a), print(b), print(c);
}

template <typename T1, typename T2, typename T3, typename T4>
inline void print(T1 a, T2 b, T3 c, T4 d) {
    print(a), print(b), print(c), print(d);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
inline void print(T1 a, T2 b, T3 c, T4 d, T5 e) {
    print(a), print(b), print(c), print(d), print(e);
}

template <typename T1, typename T2>
inline void println(T1 a, T2 b) {
    print(a), print(b), print('\n');
}

template <typename T1, typename T2, typename T3>
inline void println(T1 a, T2 b, T3 c) {
    print(a), print(b), print(c), print('\n');
}

template <typename T1, typename T2, typename T3, typename T4>
inline void println(T1 a, T2 b, T3 c, T4 d) {
    print(a), print(b), print(c), print(d), print('\n');
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
inline void println(T1 a, T2 b, T3 c, T4 d, T5 e) {
    print(a), print(b), print(c), print(d), print(e), print('\n');
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}

namespace Task {

const int MAXN = 100010;

#define long long long
long cost[MAXN], base[MAXN];
long n, S;

inline bool check(long m) {
    for (int i = 1; i <= n; i++) {
        cost[i] = base[i] + i * m;
    }
    std::sort(cost + 1, cost + n + 1);
    long s = 0;
    for (int i = 1; i <= m; ++i) {
        s += cost[i];
    }
    return s <= S;
}

inline void solve() {
    using namespace IO;
    read(n), read(S);
    for (register int i = 1; i <= n; i++) read(base[i]);
    static long sum[MAXN];
    register int l = 0, r = n + 1;
    while (r - l > 1) {
        register int mid = l + r >> 1;
        if (check(mid))
            l = mid;
        else
            r = mid;
    }
    print(l), print(' ');
    for (int i = 1; i <= n; i++) {
        cost[i] = base[i] + i * l;
    }
    std::sort(cost + 1, cost + n + 1);
    long s = 0;
    for (int i = 1; i <= l; ++i) {
        s += cost[i];
    }
    print(s);
}
}

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    Task::solve();
    IO::flush();
    return 0;
}