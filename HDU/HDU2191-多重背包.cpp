/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「HDU 2191」 29-09-2017
 * 多重背包
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

const int MAXN = 100;

using IO::io;

int v, f[MAXN + 1];

template <typename T>
struct MonotoneQueue {
    typedef std::pair<T, int> Pair;

    Pair q[MAXN + 1], *l, *r;
    int cur, pos;

    inline void push(const T &x) {
        while (l < r && (r - 1)->first < x) r--;
        *r++ = Pair(x, cur++);
    }

    inline const T &top() { return l->first; }

    inline void pop() {
        if (l->second == pos++) l++;
    }

    MonotoneQueue() : l(q), r(q) {}

    inline int size() { return cur - pos; }

    inline void clear() { l = r = q, cur = pos = 0; }
};

inline void pack(int c, int w, int n) {
    n = std::min(n, v / c);
    for (register int r = 0; r < c; r++) {
        static MonotoneQueue<int> q;
        q.clear();
        register int m = (v - r) / c;
        for (register int k = 0; k <= m; k++) {
            if (q.size() == n + 1) q.pop();
            q.push(f[k * c + r] - k * w);
            f[k * c + r] = q.top() + k * w;
        }
    }
}

inline void solve() {
    register int T, n;
    io >> T;
    while (T--) {
        io >> v >> n;
        memset(f, 0, sizeof(int) * (v + 1));
        for (register int i = 0, c, w, num; i < n; i++)
            io >> c >> w >> num, pack(c, w, num);
        io << f[v] << '\n';
    }
}
}

int main() {
    solve();
    return 0;
}
