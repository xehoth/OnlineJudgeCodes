/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4826」影魔 24-09-2017
 * 扫描线
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

const int MAXN = 200010;
using IO::io;

#define long long long

int n, m, p1, p2;

struct BinaryIndexedTree {
    long d[MAXN + 1], mul[MAXN + 1];

    inline void add(register int x, int v) {
        for (register int k = x; k <= n; k += k & -k)
            d[k] += v, mul[k] += (long)x * v;
    }

    inline long query(register int x) {
        register long ret = 0;
        for (register int k = x; k; k ^= k & -k)
            ret += (x + 1ll) * d[k] - mul[k];
        return ret;
    }
} bit;

int a[MAXN + 1], lBound[MAXN + 1], rBound[MAXN + 1];

long ans[MAXN + 1];

struct Node {
    int l, r, id, x;
    long *ans;
    Node(int l = 0, int r = 0, int id = 0, long *ans = NULL, int x = 0)
        : l(l), r(r), id(id), ans(ans), x(x) {}

    inline bool operator<(const Node &p) const { return id < p.id; }
} d1[MAXN * 2 + 2], d2[MAXN * 3 + 3];

inline void initBound() {
    a[0] = a[n + 1] = n + 1;
    static std::vector<int> st;
    st.reserve(n + 2), st.push_back(0);
    for (register int i = 1; i <= n + 1; i++) {
        while (a[st.back()] < a[i]) rBound[st.back()] = i, st.pop_back();
        lBound[i] = st.back(), st.push_back(i);
    }
}

inline void solve() {
    io >> n >> m >> p1 >> p2;
    for (register int i = 1; i <= n; i++) io >> a[i];
    initBound();

    for (register int i = 1, l, r; i <= m; i++) {
        io >> l >> r;
        ans[i] += (r - l) * p1;
        d1[i] = Node(l, r, l - 1, ans + i, -1);
        d1[i + m] = Node(l, r, r, ans + i, 1);
    }
    std::sort(d1 + 1, d1 + m * 2 + 1);

    register int cnt = 0;
    for (register int i = 1; i <= n; i++) {
        if (lBound[i] && rBound[i] <= n)
            d2[++cnt] = Node(lBound[i], lBound[i], rBound[i], 0, p1);
        if (lBound[i] && rBound[i] > i + 1)
            d2[++cnt] = Node(i + 1, rBound[i] - 1, lBound[i], 0, p2);
        if (rBound[i] <= n && i > lBound[i] + 1)
            d2[++cnt] = Node(lBound[i] + 1, i - 1, rBound[i], 0, p2);
    }
    std::sort(d2 + 1, d2 + cnt + 1);
    register int nowD1 = 1, nowD2 = 1;
    while (!d1[nowD1].id) nowD1++;
    for (register int i = 1; nowD1 <= m * 2 && i <= n; i++) {
        while (nowD2 <= cnt && d2[nowD2].id == i) {
            bit.add(d2[nowD2].r + 1, -d2[nowD2].x);
            bit.add(d2[nowD2].l, d2[nowD2].x), nowD2++;
        }
        while (nowD1 <= m * 2 && d1[nowD1].id == i) {
            *d1[nowD1].ans += d1[nowD1].x * (bit.query(d1[nowD1].r) -
                                             bit.query(d1[nowD1].l - 1));
            nowD1++;
        }
    }
    for (register int i = 1; i <= m; i++) io << ans[i] << '\n';
}

#undef long
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}