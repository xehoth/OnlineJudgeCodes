/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1563」诗人小G 25-09-2017
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

#define long long long
const int MAXN = 100010;
const long INF = 0x3f3f3f3f3f3f3f3fLL;
const long LIMIT = (long)1e18;

struct Node {
    int l, r, pos;
    Node() {}
    Node(int l, int r, int pos) : l(l), r(r), pos(pos) {}
} sta[MAXN];
double F[MAXN];
char poem[40];
int g[MAXN], T, n, top, p;
long sum[MAXN], f[MAXN], len;

inline long pow(long a, int n) {
    register long ans = 1, tmp = a;
    for (; n; n >>= 1, tmp *= tmp) {
        if (n & 1) {
            if (double(ans) * double(tmp) > LIMIT) return INF;
            ans *= tmp;
        }
    }
    return ans;
}

inline long getf(int i, int j) {
    register long tmp = pow(llabs(i - j - 1 + sum[i] - sum[j] - len), p);
    return (double)tmp + f[j] > LIMIT ? INF : tmp + f[j];
}

inline double getF(int i, int j) {
    return ::pow(fabs(i - j - 1.0 + sum[i] - sum[j] - len), (double)p) + F[j];
}

inline void update(int i) {
    while (i < sta[top].l &&
           getF(sta[top].l, i) < getF(sta[top].l, sta[top].pos))
        sta[top - 1].r = sta[top].r, --top;
    register int l = sta[top].l, r = sta[top].r;
    if (i >= l) l = i + 1;
    l--;
    while (r - l > 1) {
        register int mid = (l + r) >> 1;
        if (getF(mid, i) < getF(mid, sta[top].pos))
            r = mid;
        else
            l = mid;
    }
    if (r < sta[top].r) sta[top + 1] = Node(r, sta[top].r, i), sta[top++].r = r;
    return;
}

inline void solve() {
    io >> T;
    while (T--) {
        io >> n >> len >> p;
        for (register int i = 1; i < n + 1; ++i)
            sum[i] = sum[i - 1] + IO::read(poem);
        sta[top = 1] = Node(1, n + 1, 0);
        for (register int i = 1, j = 1; i < n + 1; ++i) {
            if (i >= sta[j].r) j++;
            F[i] = getF(i, g[i] = sta[j].pos);
            update(i);
        }
        if (F[n] > LIMIT) {
            io << "Too hard to arrange\n";
        } else {
            for (register int i = 1; i < n + 1; ++i) f[i] = getf(i, g[i]);
            io << f[n] << '\n';
        }
        io << "--------------------\n";
    }
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}