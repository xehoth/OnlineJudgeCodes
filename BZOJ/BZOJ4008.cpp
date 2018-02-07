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

inline void read(double &t) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    register int x = 0;
    for (; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    if (c == '.') {
        register int y = 0, cnt = 1;
        for (c = read(); isdigit(c); c = read())
            y = y * 10 + (c ^ '0'), cnt *= 10;
        t = x + (double)y / cnt;
    } else {
        t = x;
    }
    iosig ? t = -t : 0;
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

const int MAXN = 1000;

using IO::io;

const int MAX_N = 220 + 3, MAX_M = 132 + 3;
const double eps = 1e-8;
int n, m, d[MAX_N];
double p[MAX_N], P[MAX_N], dp[MAX_N][MAX_M];

void solveCase() {
    memset(dp, 0, sizeof(dp));
    memset(P, 0, sizeof(P));
    dp[0][m] = 1.0;
    for (register int i = 0; i < n; i++) {
        double t = 1.0;
        for (register int j = 0; j <= m; j++) {
            dp[i + 1][j] += dp[i][j] * t;
            if (j) dp[i + 1][j - 1] += dp[i][j] * (1 - t);
            P[i] += dp[i][j] * (1 - t), t *= (1 - p[i]);
        }
    }
    for (register int j = 0; j < m; j++) {
        double t = 0;
        for (register int i = 0; i <= n; i++) t += dp[j][i];
    }
    register double ans = 0;
    for (register int i = 0; i < n; i++) ans += P[i] * d[i];
    printf("%.10f\n", ans);
}

inline void solve() {
    register int T;
    io >> T;
    while (T--) {
        io >> n >> m;
        for (register int i = 0; i < n; i++) io >> p[i] >> d[i];
        solveCase();
    }
}
}  // namespace

int main() {
    solve();
    return 0;
}