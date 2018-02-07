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

const int MAXN = 100005;

struct Node {
    int min, sub;
} d[131072 << 1 | 1];

int M;

inline void build(const int n) {
    for (M = 1; M < n + 2; M <<= 1)
        ;
    for (register int i = 1; i <= n; i++) read(d[i + M].min);
    for (register int i = M - 1; i; i--)
        d[i].min = std::min(d[i << 1].min, d[i << 1 | 1].min);
}

inline void cover(int k, int v) { d[k].min -= v, d[k].sub += v; }

inline void pushDown(int k) {
    if (d[k].sub && k < M) {
        (k << 1) ? (cover(k << 1, d[k].sub), 0) : 0;
        (k << 1 | 1) ? (cover(k << 1 | 1, d[k].sub), 0) : 0;
        d[k].sub = 0;
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
        d[l].min = std::min(d[l << 1].min, d[l << 1 | 1].min);
    for (r >>= 1; r; r >>= 1)
        d[r].min = std::min(d[r << 1].min, d[r << 1 | 1].min);
}

inline int query(int s, int t) {
    register int l = 0, r = 0, ret = INT_MAX;
    for (s = s + M - 1, t = t + M + 1; s ^ t ^ 1; s >>= 1, t >>= 1) {
        (~s & 1) ? (l ? 0 : (update(l = s ^ 1), 0),
                    ret = std::min(ret, d[s ^ 1].min), 0)
                 : 0;
        (t & 1) ? (r ? 0 : (update(r = t ^ 1), 0),
                   ret = std::min(ret, d[t ^ 1].min), 0)
                : 0;
    }
    return ret;
}

struct Data {
    int l, r;

    inline bool operator<(const Data &x) const { return r < x.r; }
} a[MAXN];

inline void solve() {
    register int n, m, ans = 0;
    read(n), read(m);
    build(n);
    for (register int i = 1; i <= m; i++) read(a[i].l), read(a[i].r);
    std::sort(a + 1, a + m + 1);
    for (register int i = 1; i <= m; i++) {
        if (query(a[i].l, a[i].r)) {
            modify(a[i].l, a[i].r);
            ans++;
        }
    }
    print(ans);
}
}  // namespace SegmentTree

int main() {
    // freopen("in.in", "r", stdin);
    SegmentTree::solve();
    flush();
    return 0;
}