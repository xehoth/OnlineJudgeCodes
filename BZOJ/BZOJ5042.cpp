#pragma GCC optimize("O3")
#include <bits/stdc++.h>

namespace {

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

typedef unsigned long long ulong;

#define optMax(x, y) ((x) > (y) ? (x) : (y))
#define optMin(x, y) ((x) < (y) ? (x) : (y))

struct Node {
    ulong min, max;
} d[4194304 * 2 | 1];

inline void solve() {
    register int n, q, M = 1;
    register ulong x, ret;
    read(n), read(q);
    for (; M < n + 2;) M <<= 1;
    for (register int i = 1; i <= n; i++)
        read(x), d[i + M].min = d[i + M].max = x;
    for (register int i = M - 1; i; i--) {
        d[i].min = optMin(d[i << 1].min, d[i << 1 | 1].min);
        d[i].max = optMax(d[i << 1].max, d[i << 1 | 1].max);
    }
    for (register int cmd, l, r; q--;) {
        read(cmd), read(l), read(r);
        switch (cmd) {
            case 1: {
                ret = 0xffffffffffffffffull;
                for (l = l + M - 1, r = r + M + 1; l ^ r ^ 1;
                     l >>= 1, r >>= 1) {
                    (r & 1) ? ret = optMin(ret, d[r ^ 1].min) : 0;
                    (~l & 1) ? ret = optMin(ret, d[l ^ 1].min) : 0;
                }
                print(ret), print('\n');
                break;
            }
            case 2: {
                ret = 0;
                for (l = l + M - 1, r = r + M + 1; l ^ r ^ 1;
                     l >>= 1, r >>= 1) {
                    (r & 1) ? ret = optMax(ret, d[r ^ 1].max) : 0;
                    (~l & 1) ? ret = optMax(ret, d[l ^ 1].max) : 0;
                }
                print(ret), print('\n');
                break;
            }
        }
    }
    flush();
}
}  // namespace

int main() {
    solve();
    return 0;
}