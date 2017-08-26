/**
 * 「SCOI 2009」粉刷匠 14-06-2017
 * dp
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
}

namespace Task {

const int MAXN = 50;
const int MAXM = 50;
const int MAXT = 2500;

inline void solve() {
    using namespace IO;
    register int n, m, t;
    read(n), read(m), read(t);
    static int w[MAXN][MAXM + 1], f[MAXM + 1][MAXM + 1];
    for (register int i = 0; i < n; i++) {
        memset(f, 0, sizeof(f));
        static char s[MAXM];
        read(s);
        for (register int j = 1; j <= m; j++) {
            for (register int k = 1; k <= j; k++) {
                register int cnt[2] = {0, 0};
                for (register int l = j - 1; l >= k - 1; l--) {
                    cnt[s[l] - '0']++;
                    f[j][k] = std::max(f[j][k],
                                       f[l][k - 1] + std::max(cnt[0], cnt[1]));
                    w[i][k] = std::max(w[i][k], f[j][k]);
                }
            }
        }
    }

    static int g[MAXT + 1];
    for (int i = 0; i < n; i++) {
        for (int j = t; j >= 0; j--) {
            for (int k = 1; k <= m; k++) {
                if (k <= j) g[j] = std::max(g[j], g[j - k] + w[i][k]);
            }
        }
    }

    print(g[t]);
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