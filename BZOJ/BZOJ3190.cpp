/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3190」赛车 01-10-2017
 * 单调栈
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
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
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

inline void read(double &x) {
    static char buf[30];
    read(buf);
    x = atof(buf);
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
}  // namespace IO

namespace {

using IO::io;

const double EPS = 1e-12;

const int MAXN = 10005;

inline int sign(double x) { return (x > EPS) - (x < -EPS); }

struct Line {
    int id, num;
    double k, b;
    inline bool operator<(const Line &a) const {
        return k < a.k || a.k == k && b > a.b;
    }
};

int n, ans[MAXN];
Line l[MAXN];
bool flag[MAXN];

inline double getX(const Line &p, const Line &q) {
    return (q.b - p.b) / (p.k - q.k);
}

inline double getY(const Line &l, double x) { return x * l.k + l.b; }

inline bool check(const Line &p, const Line &q, const Line &r) {
    register double x = getX(p, q);
    return sign(getY(p, x) - getY(r, x)) < 0;
}

inline void solve() {
    register int n;
    io >> n;
    for (register int i = 1; i <= n; i++) io >> l[i].b, l[i].id = i;
    for (register int i = 1; i <= n; i++) io >> l[i].k;
    std::sort(l + 1, l + n + 1);
    for (register int i = 1; i <= n; i++) l[i].num = i;
    static Line stack[MAXN], *top = stack;

    for (register int i = 1; i <= n; i++) {
        if (top > stack && sign(l[i].k - top->k) == 0) continue;
        while (top > stack + 1 && check(*top, *(top - 1), l[i])) top--;
        *++top = l[i];
    }
    for (register int i = 1; i <= top - stack; i++) {
        if (i != top - stack) {
            register double x = getX(stack[i], stack[i + 1]);
            if (sign(x) < 0) continue;
        }
        Line now = stack[i];
        for (register int i = now.num; i <= n; i++) {
            if (sign(now.k - l[i].k) == 0 && sign(now.b - l[i].b) == 0)
                flag[l[i].id] = true;
            else
                break;
        }
    }
    for (register int i = 1; i <= n; i++)
        if (flag[i]) ++ans[++ans[0]] = i;
    io << ans[0] << '\n';
    for (register int i = 1; i <= ans[0]; i++)
        io << ans[i] << " \n"[i == ans[0]];
}
}  // namespace

int main() {
    solve();
    return 0;
}