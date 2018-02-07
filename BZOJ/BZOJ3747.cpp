/*
 * created by xehoth on 17-04-2017
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

namespace SegmentTree {

const int MAXN = 1000005;

#define long long long

struct Node {
    long max, add;
} d[1048576 * 2 + 1];

int M;

inline void maintain(int k) {
    d[k].max = std::max(d[k << 1].max, d[k << 1 | 1].max);
}

inline void build(const int n) {
    for (M = 1; M < n + 2; M <<= 1)
        ;
}

inline void cover(int k, long v) { d[k].max += v, d[k].add += v; }

inline void pushDown(int k) {
    if (d[k].add && k <= M) {
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

inline void modify(int s, int t, int v) {
    register int l = 0, r = 0;
    for (s = s + M - 1, t = t + M + 1; s ^ t ^ 1; s >>= 1, t >>= 1) {
        (~s & 1) ? (l ? 0 : (update(l = s ^ 1), 0), cover(s ^ 1, v), 0) : 0;
        (t & 1) ? (r ? 0 : (update(r = t ^ 1), 0), cover(t ^ 1, v), 0) : 0;
    }
    for (l >>= 1; l; l >>= 1) maintain(l);
    for (r >>= 1; r; r >>= 1) maintain(r);
}

inline void solve() {
    register int n, m;
    static int f[MAXN], w[MAXN], next[MAXN], last[MAXN];
    read(n), read(m);
    for (register int i = 1; i <= n; i++) read(f[i]);
    for (register int i = 1; i <= m; i++) read(w[i]);
    for (register int i = n; i; i--) next[i] = last[f[i]], last[f[i]] = i;
    build(n);
    for (register int i = 1; i <= m; i++) {
        if (last[i]) {
            if (!next[last[i]])
                modify(last[i], n, w[i]);
            else
                modify(last[i], next[last[i]] - 1, w[i]);
        }
    }
    register long ans = 0;
    for (register int i = 1; i <= n; i++) {
        ans = std::max(ans, d[1].max);
        register int t = next[i];
        if (t) {
            modify(i, t - 1, -w[f[i]]);
            if (next[t])
                modify(t, next[t] - 1, w[f[i]]);
            else
                modify(t, n, w[f[i]]);
        } else {
            modify(i, n, -w[f[i]]);
        }
    }
    printf("%lld\n", ans);
}
}  // namespace SegmentTree

int main() {
    // freopen("in.in", "r", stdin);
    SegmentTree::solve();
    return 0;
}
