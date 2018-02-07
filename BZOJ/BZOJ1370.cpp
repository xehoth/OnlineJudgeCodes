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

const int OUT_LEN = 100;

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

const int MAXN = 2005;

int fa[MAXN];

inline int get(int x) {
    while (x != fa[x]) x = fa[x] = fa[fa[x]];
    return x;
}

inline void put(int x, int y) { fa[get(x)] = get(y); }

inline void solve() {
    register int n, m;
    read(n), read(m);
    for (register int i = 1; i <= n << 1; i++) fa[i] = i;
    for (register int i = 1, x, y; i <= m; i++) {
        switch (read()) {
            case 'F':
                read(x), read(y), put(x, y);
                break;
            case 'E':
                read(x), read(y), put(x, y + n), put(y, x + n);
                break;
        }
    }
    static int a[MAXN];
    for (register int i = 1; i <= n; i++) a[i] = get(i);
    std::sort(a + 1, a + n + 1);
    print(std::unique(a + 1, a + n + 1) - (a + 1));
}
}  // namespace UnionFindSet

int main() {
    // freopen("in.in", "r", stdin);
    UnionFindSet::solve();
    flush();
    return 0;
}
