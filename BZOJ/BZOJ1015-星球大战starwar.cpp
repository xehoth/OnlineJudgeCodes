/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1015」星球大战starwar 20-04-2017
 *
 * @author xehoth
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

const int MAXN = 400001;

int fa[MAXN], tot;
bool vis[MAXN], des[MAXN];

std::vector<int> edge[MAXN];

inline int get(int x) { return x == fa[x] ? x : fa[x] = get(fa[x]); }

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v);
    edge[v].push_back(u);
}

inline void put(int x, int y) { x != y ? (fa[x] = y, tot--) : 0; }

inline void add(int x) {
    for (register int i = 0, p = get(x), v, q; i < edge[x].size(); i++)
        vis[v = edge[x][i]] ? (put(q = get(v), p), 0) : 0;
}

inline void solve() {
    register int n, m, d;
    read(n), read(m);
    for (register int i = 1; i <= n; i++) fa[i] = i;
    for (register int i = 1, u, v; i <= m; i++) read(u), read(v), addEdge(u, v);
    read(d);
    static int q[MAXN];
    for (register int i = 1; i <= d; i++) read(q[i]), des[q[i]] = true;
    for (register int i = 0; i < n; i++)
        !des[i] ? (tot++, add(i), vis[i] = true) : 0;
    static int ans[MAXN];
    ans[d + 1] = tot;
    for (register int i = d; i; i--)
        tot++, add(q[i]), vis[q[i]] = true, ans[i] = tot;
    for (register int i = 1; i <= d + 1; i++) print(ans[i]), print('\n');
}
}  // namespace UnionFindSet

int main() {
    // freopen("in.in", "r", stdin);
    UnionFindSet::solve();
    flush();
    return 0;
}
