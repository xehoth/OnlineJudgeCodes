/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1483」梦幻布丁 09-10-2017
 * 链表+启发式合并
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline bool read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return false;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
    return true;
}

inline void read(char &c) {
    while (c = read(), isspace(c) && c != -1)
        ;
}

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = 0;
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = 0;
    return s;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }

    ~InputOutputStream() { flush(); }
} io;
}

namespace {

using IO::io;
const int MAXN = 1000000;

int a[MAXN + 1], cnt, n, fa[MAXN + 1];
std::list<int> pos[MAXN + 1];

inline void merge(int a, int b) {
    if (a == b) return;
    if (pos[fa[a]].size() > pos[fa[b]].size()) std::swap(fa[a], fa[b]);
    a = fa[a], b = fa[b];
    for (register std::list<int>::iterator it = pos[a].begin();
         it != pos[a].end(); it++) {
        if (::a[*it - 1] == b) cnt--;
        if (::a[*it + 1] == b) cnt--;
    }
    for (register std::list<int>::iterator it = pos[a].begin();
         it != pos[a].end(); it++)
        ::a[*it] = b;
    pos[b].merge(pos[a]), pos[b].unique();
}

inline void solve() {
    register int m;
    io >> n >> m;
    for (register int i = 1; i <= n; i++) {
        io >> a[i], a[i] != a[i - 1] ? cnt++ : 0;
        fa[a[i]] = a[i], pos[a[i]].push_back(i);
    }
    for (register int i = 1; i <= n; i++) pos[i].unique();
    for (register int x, y, cmd; m--;) {
        io >> cmd;
        switch (cmd) {
            case 1: {
                io >> x >> y, merge(x, y);
                break;
            }
            case 2: {
                io << cnt << '\n';
                break;
            }
        }
    }
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}