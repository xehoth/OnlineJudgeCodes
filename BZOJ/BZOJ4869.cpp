/*
 * created by xehoth on 01-05-2017
 */
#include <bits/stdc++.h>

namespace IO {

const int IN_LEN = 4000000;
char ibuf[IN_LEN], *s = ibuf;

inline char read() { return *s++; }

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

const int OUT_LEN = 500000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) { *oh++ = c; }

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

namespace SegmentTree {

const int MAXN = 50000 + 10;
const int MAXX = 10000;

int n, m, p, c;
int cnt;
int a[MAXN], phi[MAXN], minPow[30][MAXN], maxPow[30][MAXN];

struct Node {
    int sum, min;
} d[MAXN << 2];

inline int mul(int x, int y, int c) {
    register int ret;
    __asm__ __volatile__("\tmull %%ebx\n\tdivl %%ecx\n"
                         : "=d"(ret)
                         : "a"(x), "b"(y), "c"(c));
    return ret;
}

inline int modPow(int a, int b, int mod, int x) {
    return mul(maxPow[x][b / MAXX], minPow[x][b % MAXX], phi[x]);
}

inline int getPhi(int p) {
    register int ret = p;
    for (register int i = 2; i * i <= p; i++) {
        (p % i == 0) ? ret = ret / i * (i - 1) : 0;
        while (p % i == 0) p /= i;
    }
    return (p != 1) ? ret / p * (p - 1) : ret;
}

inline int calc(int x, int p) {
    register int ret = (x >= phi[p] ? x % phi[p] + phi[p] : x);
    while (p--) ret = modPow(c, ret, phi[p], p), (ret == 0 ? ret += phi[p] : 0);
    return ret % phi[0];
}

inline void init() {
    phi[0] = p;
    while (p != 1) p = phi[++cnt] = getPhi(p);
    phi[++cnt] = 1;
    for (register int i = 0; i <= cnt; i++) minPow[i][0] = maxPow[i][0] = 1;
    for (register int j = 0; j <= cnt; j++)
        for (register int i = 1; i <= MAXX; i++)
            minPow[j][i] = mul(minPow[j][i - 1], c, phi[j]);
    for (register int j = 0; j <= cnt; j++)
        for (register int i = 1; i <= MAXX; i++)
            maxPow[j][i] = mul(maxPow[j][i - 1], minPow[j][MAXX], phi[j]);
}

inline void maintain(int k) {
    (d[k].sum = d[k << 1].sum + d[k << 1 | 1].sum) >= phi[0]
        ? d[k].sum -= phi[0]
        : 0;
    d[k].min = std::min(d[k << 1].min, d[k << 1 | 1].min);
}

inline void build(int k, int l, int r) {
    if (l == r) return (void)(d[k].sum = a[l]);
    register int mid = l + r >> 1;
    build(k << 1, l, mid), build(k << 1 | 1, mid + 1, r);
    maintain(k);
}

inline void modify(int k, int s, int t, int l, int r) {
    if (d[k].min >= cnt) return;
    if (s == t) {
        d[k].min++, d[k].sum = calc(a[s], d[k].min);
        return;
    }
    register int mid = s + t >> 1;
    if (l <= mid) modify(k << 1, s, mid, l, r);
    if (r > mid) modify(k << 1 | 1, mid + 1, t, l, r);
    maintain(k);
}

inline int query(int k, int s, int t, int l, int r) {
    if (l <= s && t <= r) return d[k].sum;
    register int mid = s + t >> 1, ret = 0;
    if (l <= mid) ret += query(k << 1, s, mid, l, r);
    if (r > mid) ret += query(k << 1 | 1, mid + 1, t, l, r);
    return maintain(k), ret >= phi[0] ? (ret -= phi[0], ret) : ret;
}

inline void solve() {
    using namespace IO;
    read(n), read(m), read(p), read(c);
    for (register int i = 1; i <= n; i++) read(a[i]);
    init();
    build(1, 1, n);
    int cmd, l, r;
    while (m--) {
        read(cmd), read(l), read(r);
        cmd == 1 ? (print((query(1, 1, n, l, r) % phi[0] + phi[0]) % phi[0]),
                    print('\n'))
                 : modify(1, 1, n, l, r);
    }
}

}  // namespace SegmentTree

int main() {
    // freopen("in.in", "r", stdin);
    // freopen("out.out", "w", stdout);
    fread(IO::ibuf, 1, IO::IN_LEN, stdin);
    SegmentTree::solve();
    IO::flush();
    return 0;
}
