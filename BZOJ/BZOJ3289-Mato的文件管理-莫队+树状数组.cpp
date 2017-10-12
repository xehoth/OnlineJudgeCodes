/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3289」Mato的文件管理 12-10-2017
 * 莫队 + 树状数组
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

const int MAXN = 50004;
const int MAXQ = 50004;

using IO::io;

int id[MAXN + 1];

int blockSize;
typedef unsigned int uint;

struct Query {
    int l, r;
    uint *ans;

    inline bool operator<(const Query &x) const {
        return id[l] == id[x.l] ? ((id[l] & 1) ? r > x.r : r < x.r) : l < x.l;
    }
} q[MAXQ + 1];

int a[MAXN + 1], n, m;

uint ans[MAXQ + 1], now;

inline void init() {
    static int tmp[MAXN + 1];
    memcpy(tmp + 1, a + 1, sizeof(int) * n);
    std::sort(tmp + 1, tmp + n + 1);
    register int cnt = std::unique(tmp + 1, tmp + n + 1) - tmp - 1;
    for (register int i = 1; i <= n; i++)
        a[i] = std::lower_bound(tmp + 1, tmp + n + 1, a[i]) - tmp;
}

int d[MAXN + 1];

inline void add(register int k, register int v) {
    for (; k <= n; k += k & -k) d[k] += v;
}

inline int query(register int k) {
    register int ret = 0;
    for (; k; k ^= k & -k) ret += d[k];
    return ret;
}

inline void solve() {
    io >> n;
    blockSize = sqrt(n) * 1.2;
    for (register int i = 1; i <= n; i++) io >> a[i], id[i] = i / blockSize + 1;
    init();
    io >> m;
    for (register int i = 1; i <= m; i++)
        io >> q[i].l >> q[i].r, q[i].ans = ans + i;
    init();
    std::sort(q + 1, q + m + 1);
    for (register int i = 1, l = 1, r = 0; i <= m; i++) {
        while (l < q[i].l) add(a[l], -1), now -= query(a[l] - 1), l++;
        while (r > q[i].r) add(a[r], -1), now -= r - l - query(a[r]), r--;
        while (l > q[i].l) l--, add(a[l], 1), now += query(a[l] - 1);
        while (r < q[i].r) r++, add(a[r], 1), now += r - l + 1 - query(a[r]);
        *q[i].ans = now;
    }
    for (register int i = 1; i <= m; i++) io << ans[i] << '\n';
}
}

int main() {
    solve();
    return 0;
}