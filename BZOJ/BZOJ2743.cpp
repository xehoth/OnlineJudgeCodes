/*
 * created by xehoth on 14-04-2017
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

const int OUT_LEN = 1000000;

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

namespace BinaryIndexedTree {

const int MAXN = 1000010;

struct Query {
    int l, r, *ans;

    inline bool operator<(const Query &q) const { return l < q.l; }
} q[MAXN];

int n, c, m, a[MAXN], next[MAXN], p[MAXN], ans[MAXN];

int d[MAXN];

inline void modify(int k, int v) {
    for (; k <= n; k += k & -k) d[k] += v;
}

inline int query(int k) {
    register int ret = 0;
    for (; k; k ^= k & -k) ret += d[k];
    return ret;
}

inline void solve() {
    read(n), read(c), read(m);
    for (register int i = 1; i <= n; i++) read(a[i]);
    for (register int i = n; i; i--) next[i] = p[a[i]], p[a[i]] = i;
    for (register int i = 1; i <= c; i++)
        next[p[i]] ? (modify(next[p[i]], 1), 0) : 0;
    for (register int i = 1; i <= m; i++)
        read(q[i].l), read(q[i].r), q[i].ans = ans + i;
    std::sort(q + 1, q + m + 1);
    register int l = 1;
    for (register int i = 1; i <= m; i++) {
        while (l < q[i].l) {
            next[l] ? (modify(next[l], -1), 0) : 0;
            next[next[l]] ? (modify(next[next[l]], 1), 0) : 0;
            l++;
        }
        *q[i].ans = query(q[i].r) - query(q[i].l - 1);
    }
    for (register int i = 1; i <= m; i++) print(ans[i]), print('\n');
}
}  // namespace BinaryIndexedTree

int main() {
    BinaryIndexedTree::solve();
    flush();
    return 0;
}
