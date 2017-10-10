/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3236」作业 09-10-2017
 * 分块 + 莫队
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

const int MAXN = 100004;
const int MAXQ = 1000004;

using IO::io;

int id[MAXN + 1];

int blockSize;

struct Query {
    int l, r, a, b, id;

    inline bool operator<(const Query &x) const {
        return ::id[l] == ::id[x.l] ? ((::id[l] & 1) ? r > x.r : r < x.r)
                                    : l < x.l;
    }
} q[MAXQ + 1];

int a[MAXN + 1], ans1[MAXQ + 1], ans2[MAXQ + 1];

const int MAX_BLOCK_COUNT = 1000;

int cnt[MAXN + 1], block1[MAX_BLOCK_COUNT + 1], block2[MAXN + 1];

inline void add(register int x) {
    block1[id[x]]++, ++cnt[x] == 1 ? block2[id[x]]++ : 0;
}

inline void erase(register int x) {
    block1[id[x]]--, --cnt[x] == 0 ? block2[id[x]]-- : 0;
}

inline int query(register int l, register int r, register int id) {
    register int *t1 = ans1 + id, *t2 = ans2 + id;
    if (::id[l] == ::id[r]) {
        for (register int i = l; i <= r; i++)
            cnt[i] ? (*t1 += cnt[i], (*t2)++) : 0;
    } else {
        for (register int i = ::id[l] * blockSize - 1; i >= l; i--)
            cnt[i] ? (*t1 += cnt[i], (*t2)++) : 0;
        for (register int i = ::id[r] * blockSize - blockSize; i <= r; i++)
            cnt[i] ? (*t1 += cnt[i], (*t2)++) : 0;
        for (register int i = ::id[l] + 1; i < ::id[r]; i++)
            *t1 += block1[i], *t2 += block2[i];
    }
}

inline void solve() {
    register int n, m;
    io >> n >> m;
    blockSize = sqrt(n / 2);
    for (register int i = 1; i <= n; i++) io >> a[i], id[i] = i / blockSize + 1;
    for (register int i = 1; i <= m; i++)
        io >> q[i].l >> q[i].r >> q[i].a >> q[i].b, q[i].id = i;
    std::sort(q + 1, q + m + 1);
    for (register int i = 1, l = 1, r = 0; i <= m; i++) {
        while (l < q[i].l) erase(a[l++]);
        while (l > q[i].l) add(a[--l]);
        while (r < q[i].r) add(a[++r]);
        while (r > q[i].r) erase(a[r--]);
        query(q[i].a, q[i].b, q[i].id);
    }
    for (register int i = 1; i <= m; i++)
        io << ans1[i] << ' ' << ans2[i] << '\n';
}
}

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    solve();
    return 0;
}