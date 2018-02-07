/*
 * created by xehoth on 18-04-2017
 */
#include <bits/stdc++.h>

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    if (s == t) {
        t = (s = buf) + fread(buf, 1, IN_LEN, stdin);
        if (s == t) return -1;
    }
    return *s++;
}

template <class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        if (c == '-') iosig = true;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    if (iosig) x = -x;
}

const int OUT_LEN = 100;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout);
    *oh++ = c;
}

template <class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

namespace SegmentTree {

const int MAXN = 1000000;

struct Node {
    int max, add;
} d[1048576 << 1 | 1];

int M;

inline void build(int n) {
    for (M = 1; M < n + 2; M <<= 1)
        ;
}

inline void cover(int k, int v) { d[k].max += v, d[k].add += v; }

inline void pushDown(int k) {
    if (d[k].add && k < M) {
        (k << 1) ? (cover(k << 1, d[k].add), 0) : 0;
        (k << 1 | 1) ? (cover(k << 1 | 1, d[k].add), 0) : 0;
        d[k].add = 0;
    }
}

inline void update(int k) {
    static int st[25], top;
    for (top = 0; k; k >>= 1) st[++top] = k;
    while (top--) pushDown(st[top]);
}

inline void modify(int s, int t) {
    register int l = 0, r = 0;
    for (s = s + M - 1, t = t + M + 1; s ^ t ^ 1; s >>= 1, t >>= 1) {
        (~s & 1) ? (l ? 0 : (update(l = s ^ 1), 0), cover(s ^ 1, 1), 0) : 0;
        (t & 1) ? (r ? 0 : (update(r = t ^ 1), 0), cover(t ^ 1, 1), 0) : 0;
    }
    for (l >>= 1; l; l >>= 1)
        d[l].max = std::max(d[l << 1].max, d[l << 1 | 1].max);
    for (r >>= 1; r; r >>= 1)
        d[r].max = std::max(d[r << 1].max, d[r << 1 | 1].max);
}

inline void solve() {
    register int n;
    read(n);
    build(MAXN);
    for (register int i = 1, x, y; i <= n; i++) {
        read(x), read(y), modify(x, y);
    }
    print(d[1].max);
}
}  // namespace SegmentTree

int main() {
    // freopen("in.in", "r", stdin);
    SegmentTree::solve();
    flush();
    return 0;
}
