/*
 * created by xehoth on 20-04-2017
 */
#include <bits/stdc++.h>

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

namespace UnionFindSet {

const int MAXN = 1000003;

int fa[MAXN];

inline int get(int x) {
    register int p = x, i;
    while (p != fa[p]) p = fa[p];
    while (p != x) i = fa[x], fa[x] = p, x = i;
    return p;
}

#define long long long

inline void solve() {
    register int n, m, p, q;
    read(n), read(m), read(p), read(q);
    static int ans[MAXN];
    for (register int i = 1; i <= n; i++) fa[i] = i;
    fa[n + 1] = n + 1;
    register int l = (n < m ? m - n : 0), r = m;
    for (register int a = r; a > l; a--) {
        register int left = (a * p + q) % n + 1;
        register int right = (a * q + p) % n + 1;
        left > right ? (left ^= right ^= left ^= right) : 0;
        for (register int b = get(left); b <= right; b = fa[b]) {
            ans[b] = a, fa[b] = get(b + 1);
        }
    }
    for (register int i = 1; i <= n; i++) print(ans[i]), print('\n');
}
}  // namespace UnionFindSet

int main() {
    // freopen("in.in", "r", stdin);
    UnionFindSet::solve();
    flush();
    return 0;
}
