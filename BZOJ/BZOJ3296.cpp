/*
 * created by xehoth on 19-04-2017
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

namespace UnionFindSet {

const int MAXN = 10000 + 30000 + 50;

int fa[MAXN];

inline int get(int x) { return x == fa[x] ? x : fa[x] = get(fa[x]); }

inline void put(int x, int y) { fa[get(x)] = get(y); }

inline void solve() {
    register int n, m;
    read(n), read(m);
    for (register int i = 1; i <= n + m; i++) fa[i] = i;
    for (register int i = 1, k, l; i <= n; i++) {
        read(k);
        for (register int j = 1; j <= k; j++) read(l), put(i, l + n);
    }
    register int ans = 0;
    static bool vis[MAXN];
    for (register int i = 1, t; i <= n; i++) {
        !vis[t = get(i)] ? (ans++, vis[t] = true) : 0;
    }
    print(ans - 1);
}
}  // namespace UnionFindSet

int main() {
    // freopen("in.in", "r", stdin);
    UnionFindSet::solve();
    flush();
    return 0;
}
