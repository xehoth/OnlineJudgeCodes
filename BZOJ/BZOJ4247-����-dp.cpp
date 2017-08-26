#include <bits/stdc++.h>
/**
 * 「BZOJ 4247」挂饰 16-06-2017
 * dp
 * @author xehoth
 */
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
}

namespace Task {

const int MAXN = 2000;

inline void solve() {
    using namespace IO;
    register int n;
    read(n);
    static struct Array {
        int a[MAXN * 2 + 1];
        int &operator[](const int i) { return a[i + MAXN]; }
    } f[MAXN + 1];
    for (register int i = -n; i < 0; i++) f[0][i] = INT_MIN;
    for (register int i = 1, x, v; i <= n; i++) {
        read(x), read(v);
        const int w = 1 - x;
        for (register int j = -n; j <= n; j++) {
            if (j - w >= -n && j - w <= n && f[i - 1][j - w] != INT_MIN)
                f[i][j] = std::max(f[i - 1][j], f[i - 1][j - w] + v);
            else if (j - w > n)
                f[i][j] = std::max(f[i - 1][j], f[i - 1][n] + v);
            else
                f[i][j] = f[i - 1][j];
        }
    }
    print(*std::max_element(&f[n][-n], &f[n][1] + 1));
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