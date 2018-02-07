#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline bool read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return false;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
    return true;
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

namespace {

using IO::io;

struct Node {
    int lmax, rmax, max, sum;
    Node(int x = INT_MIN) : lmax(x), rmax(x), max(x), sum(x) {}
};

inline int max(int a, int b) {
    register int m = (a - b) >> 31;
    return b & m | a & ~m;
}

inline Node operator+(const Node &l, const Node &r) {
    register Node f;
    if (l.max == INT_MIN) return r;
    if (r.max == INT_MIN) return l;
    f.lmax = max(l.lmax, l.sum + r.lmax);
    f.rmax = max(r.rmax, r.sum + l.rmax);
    f.sum = l.sum + r.sum;
    f.max = max(l.max, r.max);
    f.max = max(f.max, l.rmax + r.lmax);
    return f;
}
Node d[1 << 20 | 1];

inline void solve() {
    register int n, m, q;
    io >> n >> q;
    for (m = 1; m <= n + 3;) m <<= 1;
    for (register int i = 1, t; i <= n; i++) io >> t, d[i + m] = Node(t);
    for (register int i = m - 1; i; i--) d[i] = d[i << 1] + d[i << 1 | 1];
    for (register int cmd, l, r; q--;) {
        io >> cmd >> l >> r;
        switch (cmd) {
            case 1: {
                register Node L, R;
                (l > r) ? std::swap(l, r) : (void)0;
                for (l += m - 1, r += m + 1; l ^ r ^ 1; l >>= 1, r >>= 1) {
                    if (~l & 1) L = L + d[l ^ 1];
                    if (r & 1) R = d[r ^ 1] + R;
                }
                io << (L + R).max << '\n';
                break;
            }
            case 2: {
                for (d[l += m] = Node(r), l >>= 1; l; l >>= 1)
                    d[l] = d[l << 1] + d[l << 1 | 1];
                break;
            }
        }
    }
}
}  // namespace

int main() {
    solve();
    return 0;
}
