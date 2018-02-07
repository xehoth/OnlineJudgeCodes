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
}  // namespace IO

#define long long long

using IO::io;

const int MXAN = 100023;
struct Point {
    int x, y;
    double a;
} a[MXAN];
int i, j, k, n, m, r;
long size, ans;
int ra, fh;
char rx;

bool cmp(Point a, Point b) { return a.a < b.a; }
inline bool check(int b, int c) {
    return (long)a[b].x * a[c].y >= (long)a[b].y * a[c].x;
}
int main() {
    io >> n;
    for (i = 1; i <= n; i++)
        io >> a[i].x >> a[i].y, a[i].a = atan2(a[i].y, a[i].x);
    std::sort(a + 1, a + 1 + n, cmp);
    ans = (long)n * (long)(n - 1) * (long)(n - 2) / (long)6;
    for (size = 0, r = 1, i = 1; i <= n; i++) {
        while (r + 1 != i && check(i, r + 1)) {
            size++, r++;
            if (r >= n) r -= n;
        }
        ans -= (size - 1) * size >> 1;
        size--;
    }
    io << ans;
    return 0;
}