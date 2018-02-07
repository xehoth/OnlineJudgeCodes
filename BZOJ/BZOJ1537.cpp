#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 100000;
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

const int OUT_LEN = 100000;

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
    ~InputOutputStream() { flush(); }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        return read(x), *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        return print(x), *this;
    }
} io;
}  // namespace IO

namespace {

using IO::io;

const int MAXN = 100000;

struct Node {
    int x, y, v;

    inline bool operator<(const Node &p) const {
        return y < p.y || (y == p.y && x < p.x);
    }
} a[MAXN + 1];

int d[MAXN + 1], n;

inline void modify(register int k, const int v) {
    for (; k <= n; k += k & -k) d[k] = std::max(d[k], v);
}

inline int query(register int k) {
    register int ret = 0;
    for (; k; k ^= k & -k) ret = std::max(d[k], ret);
    return ret;
}

inline void solve() {
    io >> n >> n >> n;
    static int tmp[MAXN + 1];
    for (register int i = 1; i <= n; i++)
        io >> a[i].x >> a[i].y >> a[i].v, tmp[i] = a[i].x;
    std::sort(tmp + 1, tmp + n + 1);
    register int *end = std::unique(tmp + 1, tmp + n + 1);
    for (register int i = 1; i <= n; i++)
        a[i].x = std::lower_bound(tmp + 1, end, a[i].x) - tmp;
    std::sort(a + 1, a + n + 1);
    register int ans = 0;
    for (register int i = 1, x; i <= n; i++) {
        x = query(a[i].x) + a[i].v, modify(a[i].x, x);
        ans = std::max(ans, x);
    }
    io << ans;
}
}  // namespace

int main() {
    solve();
    return 0;
}