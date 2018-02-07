/*
 * created xehoth on 17-04-2017
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

const int OUT_LEN = 10000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

namespace SegmentTree {

const int MAXN = 300005;

struct Node {
    int min, sum;

    Node() {}

    Node(const int min, const int sum) : min(min), sum(sum) {}
} d[524288 << 1 | 1];

int M, cnt;

inline void maintain(int k) {
    d[k].min = std::min(d[k << 1].min, d[k << 1 | 1].min),
    d[k].sum = d[k << 1].sum + d[k << 1 | 1].sum;
}

inline void build(const int n) {
    for (M = 1; M < n + 2; M <<= 1)
        ;
    for (register int i = 1; i <= n; i++)
        read(d[i + M].min), d[i + M].sum = d[i + M].min * d[i + M].min * 6;
    for (register int i = M - 1; i; i--) maintain(i);
}

inline void modify(int k, int v) {
    d[k += M].min = v, d[k].sum = v * v * 6;
    while (k >>= 1) maintain(k);
}

inline Node query(int s, int t) {
    Node ans(INT_MAX, 0);
    for (s = s + M - 1, t = t + M + 1; s ^ t ^ 1; s >>= 1, t >>= 1) {
        (~s & 1) ? (ans.min = std::min(ans.min, d[s ^ 1].min),
                    ans.sum += d[s ^ 1].sum)
                 : 0;
        (t & 1) ? (ans.min = std::min(ans.min, d[t ^ 1].min),
                   ans.sum += d[t ^ 1].sum)
                : 0;
    }
    return ans;
}

inline void solve() {
    register int n, m;
    read(n), read(m);
    build(n);
    register int cmd, x, y, l, r, k, len;
    Node ans;
    while (m--) {
        read(cmd);
        if (cmd == 1) {
            read(x), read(y), x ^= cnt, y ^= cnt, modify(x, y);
        } else {
            read(l), read(r), read(k), l ^= cnt, r ^= cnt, k ^= cnt,
                ans = query(l, r);
            len = r - l;
            if (ans.sum == ans.min * ans.min * (len + 1) * 6 +
                               len * (len + 1) * ans.min * k * 6 +
                               len * (len + 1) * (len << 1 | 1) * k * k) {
                print('Y'), print('e'), print('s'), print('\n'), cnt++;
            } else {
                print('N'), print('o'), print('\n');
            }
        }
    }
}

}  // namespace SegmentTree

int main() {
    // freopen("in.in", "r", stdin);
    SegmentTree::solve();
    flush();
    return 0;
}