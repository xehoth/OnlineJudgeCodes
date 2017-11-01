/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 2025」纸带 1-11-2017
 * 线段树
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

struct InputOutputStream {
    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        for (c = read(); !isdigit(c); c = read())
            if (c == -1) return *this;
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        return *this;
    }
} io;

const int MAXN = 1000000;

int d[MAXN * 8 + 1], M, n, l[MAXN + 1], r[MAXN + 1];

#define pushDown(k) (d[k] ? d[k << 1] = d[k << 1 | 1] = d[k], d[k] = 0 : 0)
#define update(k)                                              \
    for (register int o = (k) >> 1; o; o >>= 1) st[++top] = o; \
    while (top--) pushDown(st[top]);

int st[25];

inline void modify(register int s, register int t, register int x) {
    register int top = 0;
    update(s = s + M - 1);
    top = 0;
    update(t = t + M + 1);
    for (; s ^ t ^ 1; s >>= 1, t >>= 1)
        (~s & 1) ? d[s ^ 1] = x : 0, (t & 1) ? d[t ^ 1] = x : 0;
}

inline void query(const int n) {
    for (register int i = 1; i < M; i++) pushDown(i);
    static bool vis[MAXN * 2 + 1];
    register int ans = 0;
    for (register int i = 1; i <= n; i++)
        (vis[d[i + M]] || !d[i + M]) ? 0 : (ans++, vis[d[i + M]] = true);
    std::cout << ans;
}

struct Data {
    int num, type, id;
    inline bool operator<(const Data &b) const { return num < b.num; }
} a[MAXN << 1 | 1];

inline void solve() {
    register int n, cnt = 0;
    io >> n;
    for (register int i = 1; i <= n; i++) {
        io >> a[++cnt].num, a[cnt].id = i, a[cnt].type = 0, a[cnt].num++;
        io >> a[++cnt].num, a[cnt].id = i, a[cnt].type = 1;
    }
    std::sort(a + 1, a + cnt + 1);
    register int i = 1, top = 0;
    for (register int j; i <= cnt; i++) {
        for (j = i; a[i].num == a[i + 1].num && i != cnt;) i++;
        top += (j != 1 && a[j - 1].num + 1 < a[i].num) ? 2 : 1;
        for (register int k = j; k <= i; k++)
            a[k].type ? (r[a[k].id] = top) : (l[a[k].id] = top);
    }
    for (M = 1; M < top + 2;) M <<= 1;
    for (register int i = 1; i <= n; i++) modify(l[i], r[i], i);
    query(top);
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}