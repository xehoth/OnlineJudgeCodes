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
        read(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }

} io;
}  // namespace IO

namespace {

using IO::io;

const int MOD = 100007;
const int MAXN = 100010;
const int MAXK = 11;

std::pair<int, int> d[MAXN / 2];
int g[MAXK][2][MAXN], s[MAXK][2];

inline void add(int *x, int i, int v) {
    while (i < MAXN) (x[i] += v) %= MOD, i += i & -i;
}

inline void add(int j, int k, int i, int v) {
    (s[j][k] += v) %= MOD, add(g[j][k], i, v);
}

inline int query(int *x, int i) {
    register int v = 0;
    while (i) (v += x[i]) %= MOD, i -= i & -i;
    return v;
}

inline void solve() {
    register int n, k;
    io >> n >> k;
    for (register int i = 0; i < n; i++) io >> d[i].first >> d[i].second;
    std::sort(d, d + n);
    for (register int i = 0; i < n; i++) {
        add(0, 0, d[i].second, 1);
        add(0, 1, d[i].second, 1);
        for (register int j = 1; j <= k; j++) {
            register int t = query(g[j][1], d[i].second - 1) +
                             query(g[j - 1][0], d[i].second - 1);
            add(j, 1, d[i].second, t);
            t = s[j][0] - query(g[j][0], d[i].second) + s[j - 1][1] -
                query(g[j - 1][1], d[i].second);
            add(j, 0, d[i].second, t);
        }
    }
    register int ans = ((s[k][0] + s[k][1]) % MOD + MOD) % MOD;
    io << ans;
}
}  // namespace

int main() {
    solve();
    return 0;
}