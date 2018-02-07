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

const int MAXN = 100010;

namespace UnionFindSet {

int fa[MAXN];

inline int get(int x) {
    return !fa[x] || x == fa[x] ? fa[x] = x : fa[x] = get(fa[x]);
}

}  // namespace UnionFindSet

namespace BinaryIndexedTree {

using namespace UnionFindSet;

#define long long long

long d[MAXN], a[MAXN];

int n;

inline void modify(int k, long v) {
    for (; k <= n; k += k & -k) d[k] += v;
}

inline void update(int k, int m) {
    for (k = get(k); k <= m; k = get(k + 1)) {
        register long tmp = sqrt(a[k]);
        modify(k, tmp - a[k]), a[k] = tmp;
        a[k] <= 1 ? fa[k] = get(k + 1) : 0;
    }
}

inline long query(int k) {
    register long ret = 0;
    for (; k; k ^= k & -k) ret += d[k];
    return ret;
}

inline void solve() {
    read(n);
    for (register int i = 1; i <= n; i++)
        read(a[i]), a[i] <= 1 ? fa[i] = i + 1 : 0, modify(i, a[i]);
    register int m;
    read(m);
    for (register int i = 1, p, x, y; i <= m; i++) {
        read(p), read(x), read(y);
        x > y ? (std::swap(x, y), 0) : 0;
        p == 0 ? (update(x, y), 0)
               : (print(query(y) - query(x - 1)), print('\n'), 0);
    }
}
}  // namespace BinaryIndexedTree

int main() {
    // freopen("in.in", "r", stdin);
    BinaryIndexedTree::solve();
    flush();
    return 0;
}
