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

const int MAXN = 500005;

int fa[MAXN], f[MAXN], rank[MAXN], dep[MAXN];
int cnt = 0;

inline int get(int x) {
    static int st[100], top;
    while (x != fa[x]) st[++top] = x, x = fa[x];
    while (top) dep[st[top]] = dep[fa[st[top--]]] + 1;
    return x;
}

inline void put(int x, int y) {
    if ((x = get(x)) != (y = get(y))) {
        rank[x] > rank[y] ? (std::swap(x, y), 0) : 0;
        fa[x] = y, f[x] = ++cnt, rank[y] += rank[x];
    } else {
        cnt++;
    }
}

inline int ask(int x, int y) {
    register int fx = get(x), fy = get(y);
    if (fx ^ fy) return 0;
    register int rtn = 0;
    while (x ^ y) {
        dep[x] > dep[y] ? (rtn = std::max(rtn, f[x]), x = fa[x])
                        : (rtn = std::max(rtn, f[y]), y = fa[y]);
    }
    return rtn;
}

int opt, lastans, tmp;

inline void solve() {
    register int n, m;
    read(n), read(m);

    for (register int i = 1; i <= n; i++) fa[i] = i, rank[i] = 1;

    for (register int i = 1, x, y; i <= m; i++) {
        read(opt), read(x), read(y);
        if (opt)
            print(tmp = ask(x ^ lastans, y ^ lastans)), print('\n'),
                lastans = tmp;
        else
            put(x ^ lastans, y ^ lastans);
    }
}

}  // namespace UnionFindSet

int main() {
    // freopen("in.in", "r", stdin);
    UnionFindSet::solve();
    flush();
    return 0;
}
