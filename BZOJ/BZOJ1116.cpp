/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1116」19-04-2017
 *
 * @author xehoth
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

const int OUT_LEN = 1000;

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

const int MAXN = 100005;

int fa[MAXN];
bool vis[MAXN];

inline int get(int x) { return x == fa[x] ? x : fa[x] = get(fa[x]); }

inline void put(int x, int y) {
    (x = get(x)) != (y = get(y)) ? (fa[x] = y, vis[y] |= vis[x]) : vis[y] = 1;
}

inline void solve() {
    register int n, m;
    read(n), read(m);
    for (register int i = 1; i <= n; i++) fa[i] = i;
    for (register int i = 1, x, y; i <= m; i++) read(x), read(y), put(x, y);
    for (register int i = 1; i <= n; i++)
        !vis[get(i)] ? (print('N'), print('I'), print('E'), flush(), exit(0), 0)
                     : 0;
    print('T'), print('A'), print('K');
}
}  // namespace UnionFindSet

int main() {
    // freopen("in.in", "r", stdin);
    UnionFindSet::solve();
    flush();
    return 0;
}
