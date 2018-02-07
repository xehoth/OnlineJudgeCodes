/*
 * created xehoth on 17-04-2017
 */
#include <bits/stdc++.h>

inline char read() {
    static const int IN_LEN = 60000;
    static char buf[IN_LEN], *s, *t;
    if (s == t) {
        t = (s = buf) + fread(buf, 1, IN_LEN, stdin);
        if (s == t) return -1;
    }
    return *s++;
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

const int OUT_LEN = 50000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}

template <class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        if (x < 0) print('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

const int MAXN = 100010;

inline void solve() {
    register int n, m, x, y, p;
    static char a[MAXN], s[MAXN];
    static int f[MAXN], g[MAXN];
    while (~(n = read(a + 1))) {
        register int r = 0, ans = 0;
        for (register int i = 1; i <= n; i++)
            s[i << 1] = a[i], s[i << 1 | 1] = '#', g[i] = 0;
        s[0] = '$', s[1] = '#', s[m = (n + 1) << 1] = '@';
        for (register int i = 1; i < m; i++) {
            for (f[i] = r > i ? std::min(r - i, f[(p << 1) - i]) : 1;
                 s[i - f[i]] == s[i + f[i]]; f[i]++)
                ;
            x = (i - f[i]) / 2 + 1, y = (i + f[i]) / 2 - 1;
            x <= y ? g[x] = std::max(g[x], y) : 0;
            i + f[i] > r ? (r = i + f[i], p = i) : 0;
        }
        for (register int i = 2; i <= n; i++) g[i] = std::max(g[i], g[i - 1]);
        for (register int j = g[1]; j < n; j = g[j + 1]) ans++;
        print(ans), print('\n');
    }
}

int main() {
    // freopen("in.in", "r", stdin);
    solve();
    flush();
    return 0;
}