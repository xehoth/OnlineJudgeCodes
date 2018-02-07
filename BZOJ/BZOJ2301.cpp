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
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, oh - obuf, stdout), oh = obuf) : 0;
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
}  // namespace IO

const int MAXM = 50000 + 9;
const int MAXN = 50000;

bool vis[MAXM];
int prime[MAXM], tot, mu[MAXM], sum[MAXM];

inline void fastLinearSieveMethod() {
    mu[1] = 1;
    for (register int i = 2; i <= MAXN; i++) {
        if (!vis[i]) prime[++tot] = i, mu[i] = -1;
        for (register int j = 1, tmp; j <= tot && (tmp = prime[j] * i) <= MAXN;
             j++) {
            vis[tmp] = true;
            if (i % prime[j]) {
                mu[tmp] = -mu[i];
            } else {
                mu[tmp] = 0;
                break;
            }
        }
    }
    for (register int i = 1; i <= MAXN; i++) sum[i] = sum[i - 1] + mu[i];
}

int main() {
    using namespace IO;
    fastLinearSieveMethod();
    register int T;
    read(T);
    while (T--) {
        register int a, b, c, d, k, ans = 0;
        read(a), read(b), read(c), read(d), read(k);
        a = (a - 1) / k, b /= k, c = (c - 1) / k, d /= k;
        for (register int i = 1, limit = std::min(b, d), tmp; i <= limit;
             i = tmp + 1) {
            tmp = std::min(b / (b / i), d / (d / i));
            if (a / i) tmp = std::min(tmp, a / (a / i));
            if (c / i) tmp = std::min(tmp, c / (c / i));
            ans += (sum[tmp] - sum[i - 1]) * (b / i - a / i) * (d / i - c / i);
        }
        print(ans), print('\n');
    }
    IO::flush();
    return 0;
}