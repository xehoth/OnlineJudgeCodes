/*
 * created by xehoth 04-05-2017
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <class T>
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
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
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

}  // namespace IO

namespace BinaryIndexedTree {

#define long long long

const int MAXN = (1 << 21) + 10;

int d[MAXN], n, a[MAXN], sum[MAXN], ans;

inline void modify(int k, int v) {
    for (; k <= MAXN; k += k & -k) d[k] += v;
}

inline int query(int k) {
    register int ret = 0;
    for (; k; k ^= k & -k) ret += d[k];
    return ret;
}

inline void solve() {
    using namespace IO;
    read(n);
    register int max = 0;
    for (register int i = 1; i <= n; i++)
        read(a[i]), max = std::max(max, a[i]), sum[i] = sum[i - 1] + a[i];
    register int ans = 0;
    for (register int k = 0; k < 20; k++) {
        memset(d, 0, sizeof(int) * (MAXN));
        register int res = 0;
        modify(1, 1);
        for (register int i = 1; i <= n; i++) {
            register int tmp = sum[i] & ((1 << k + 1) - 1);
            if (tmp >= (1 << k)) {
                res += query(1 << (k + 1)) -
                       (query(tmp + 1) - query(tmp - (1 << k) + 1));
            } else {
                res += query(tmp + (1 << k) + 1) - query(tmp + 1);
            }
            tmp = sum[i] & ((1 << k + 1) - 1), modify(tmp + 1, 1);
        }
        ans += ((res & 1) ? (1 << k) : 0);
    }
    print(ans);
}
}

int main() {
    // freopen("sum.in", "r", stdin);
    // freopen("sum.out", "w", stdout);
    BinaryIndexedTree::solve();
    IO::flush();
    return 0;
}