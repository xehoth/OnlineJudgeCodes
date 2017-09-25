/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2216」Lightning Conductor 25-09-2017
 * dp + 决策单调性优化
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
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (iosig = false, c = read(); !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
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

const int OUT_LEN = 10000000;

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

const int MAXN = 500005;

struct Node {
    int l, r, pos;

    Node(int l = 0, int r = 0, int pos = 0) : l(l), r(r), pos(pos) {}
} st[MAXN + 1];

int a[MAXN + 1], n;

double f[MAXN + 1], g[MAXN + 1];

inline double calc(int i, int j) { return a[j] - a[i] + sqrt(i - j); }

inline void dp(double *f) {
    register Node *top = st, *pos;
    *++top = Node(2, n, 1), pos = top;
    for (register int i = 2; i <= n; i++) {
        while (i > pos->r) pos++;
        pos->l = i + 1, f[i] = fmax(0, calc(i, pos->pos));
        while (top >= pos && calc(top->l, i) > calc(top->l, top->pos)) top--;
        if (top >= pos) {
            register int l = top->l, r = top->r, mid;
            for (r++; r - l > 1;) {
                mid = l + r >> 1;
                if (calc(mid, i) < calc(mid, top->pos))
                    l = mid;
                else
                    r = mid;
            }
            if (r < n) top->r = r, *++top = Node(r + 1, n, i);
        } else {
            *++top = Node(i + 1, n, i);
        }
    }
}

inline void solve() {
    io >> n;
    for (register int i = 1; i <= n; i++) io >> a[i];

    dp(f), std::reverse(a + 1, a + n + 1);
    dp(g), std::reverse(g + 1, g + n + 1);
    for (register int i = 1; i <= n; i++)
        io << (int)ceil(fmax(f[i], g[i])) << '\n';
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}