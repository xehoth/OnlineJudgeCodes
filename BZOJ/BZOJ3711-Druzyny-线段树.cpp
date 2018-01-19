/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 3711」Druzyny 15-01-2018
 * 线段树
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

inline char read() {
    static const int IN_LEN = 1 << 18 | 1;
    static char buf[IN_LEN], *s, *t;
    return (s == t) && (t = (s = buf) + fread(buf, 1, IN_LEN, stdin)),
           s == t ? -1 : *s++;
}

struct InputStream {
    template <typename T>
    inline InputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        iosig && (x = -x);
        return *this;
    }
} io;

const int MAXN = 1000000 + 9;
const int MOD = 1e9 + 7;
const int INF = 0x3f3f3f3f;

int n, c[MAXN], d[MAXN], g[MAXN];

inline int add(const int x, const int v) {
    return x + v >= MOD ? x + v - MOD : x + v;
}

struct Info {
    int max, cnt;

    Info() {}
    Info(const int max, const int cnt) : max(max), cnt(cnt) {}

    inline Info operator+(const Info &p) const {
        return max < p.max ? p
                           : (max > p.max ? *this : Info(max, add(cnt, p.cnt)));
    }

    inline Info operator+(const int x) const { return Info(max + x, cnt); }

    inline void operator+=(const Info &p) { *this = *this + p; }
} f[MAXN];

struct Node {
    int maxPosC;
    int minValD;
    Info f, tag;
} pool[2097153];

inline void build(int p, int l, int r) {
    if (l == r) {
        p[pool].maxPosC = l;
        p[pool].minValD = d[l];
        return;
    }
    register int mid = (l + r) >> 1;
    build(p << 1, l, mid);
    build(p << 1 | 1, mid + 1, r);
    p[pool].maxPosC = c[pool[p << 1].maxPosC] > c[pool[p << 1 | 1].maxPosC]
                          ? pool[p << 1].maxPosC
                          : pool[p << 1 | 1].maxPosC;
    p[pool].minValD = std::min(pool[p << 1].minValD, pool[p << 1 | 1].minValD);
}

inline void buildUpdate(int p, int l, int r) {
    p[pool].tag.max = -INF;
    p[pool].tag.cnt = 0;
    if (l == r) {
        p[pool].minValD = d[l];
        p[pool].f = f[l];
        return;
    }
    register int mid = (l + r) >> 1;
    buildUpdate(p << 1, l, mid);
    buildUpdate(p << 1 | 1, mid + 1, r);
    p[pool].f = pool[p << 1].f + pool[p << 1 | 1].f;
    p[pool].minValD = std::min(pool[p << 1].minValD, pool[p << 1 | 1].minValD);
}

inline void modifyAdd(int p, int l, int r, int s, int t, const Info &v) {
    if (s <= l && t >= r) {
        p[pool].tag += v;
        return;
    }
    register int mid = (l + r) >> 1;
    if (s <= mid) modifyAdd(p << 1, l, mid, s, t, v);
    if (t > mid) modifyAdd(p << 1 | 1, mid + 1, r, s, t, v);
}

inline int queryC(int p, int l, int r, int s, int t) {
    if (s <= l && t >= r) return p[pool].maxPosC;
    register int mid = (l + r) >> 1;
    register int pos = 0, res;
    if (s <= mid) {
        res = queryC(p << 1, l, mid, s, t);
        (c[res] > c[pos]) && (pos = res);
    }
    if (t > mid) {
        res = queryC(p << 1 | 1, mid + 1, r, s, t);
        (c[res] > c[pos]) && (pos = res);
    }
    return pos;
}

inline int queryD(int p, int l, int r, int s, int t) {
    if (s <= l && t >= r) return p[pool].minValD;
    register int mid = (l + r) >> 1, min = INF;
    if (s <= mid) min = std::min(min, queryD(p << 1, l, mid, s, t));
    if (t > mid) min = std::min(min, queryD(p << 1 | 1, mid + 1, r, s, t));
    return min;
}

inline Info queryF(int p, int l, int r, int s, int t) {
    if (s > t) return Info(-INF, 0);
    if (s <= l && t >= r) return p[pool].f;
    register int mid = (l + r) >> 1;
    if (t <= mid) return queryF(p << 1, l, mid, s, t);
    if (s > mid) return queryF(p << 1 | 1, mid + 1, r, s, t);
    return queryF(p << 1, l, mid, s, mid) +
           queryF(p << 1 | 1, mid + 1, r, mid + 1, t);
}

inline void modifyCover(int p, int l, int r, int pos, const Info &v) {
    if (l == r) {
        p[pool].f = v;
        return;
    }
    register int mid = (l + r) >> 1;
    pos <= mid ? modifyCover(p << 1, l, mid, pos, v)
               : modifyCover(p << 1 | 1, mid + 1, r, pos, v);
    p[pool].f = pool[p << 1].f + pool[p << 1 | 1].f;
}

inline Info query(const int x, register int l, register int r) {
    register int p = 1;
    Info ret(-INF, 0);
    for (register int mid; l != r;) {
        ret += p[pool].tag;
        mid = (l + r) >> 1;
        if (x <= mid) {
            p = p << 1;
            r = mid;
        } else {
            p = p << 1 | 1;
            l = mid + 1;
        }
    }
    return ret + p[pool].tag;
}

inline void update(int l, int mid, int r) {
    register int i = std::max(c[mid] + l, mid);
    if (i > r || g[i] >= mid) return;
    register int newL = std::max(l, g[i]), newR = i - c[mid];
    Info tmp = queryF(1, 0, n, newL, newR) + 1;
    for (; i <= mid - 1 + c[mid] && i <= r; i++) {
        if (g[i] > newL) {
            if (g[i] >= mid) return;
            newL = g[i];
            tmp = queryF(1, 0, n, newL, newR) + 1;
        }
        f[i] += tmp;
        newR++;
        if (newR >= newL) tmp += f[newR] + 1;
    }
    while (i <= r) {
        if (g[i] > newL) {
            if (g[i] >= mid) return;
            newL = g[i];
        }
        tmp = queryF(1, 0, n, newL, mid - 1) + 1;
        int t = queryD(1, 0, n, newL + 1, n);
        if (t > r) {
            modifyAdd(1, 0, n, i, r, tmp);
            return;
        }
        modifyAdd(1, 0, n, i, t - 1, tmp);
        i = t;
    }
}

void solve(int l, int r) {
    if (l == r) {
        if (l) modifyCover(1, 0, n, l, f[l] = f[l] + query(l, 0, n));
        return;
    }
    register int mid = queryC(1, 0, n, l + 1, r);
    solve(l, mid - 1);
    update(l, mid, r);
    solve(mid, r);
}

inline void solve() {
    io >> n;
    for (register int i = 1; i <= n; i++) io >> c[i] >> d[i];
    build(1, 0, n);
    for (register int i = 0; i <= n; i++) {
        d[i] = n + 1;
        f[i] = Info(-INF, 0);
    }
    f[0] = Info(0, 1);
    for (register int i = 0, j = 0; i <= n; i++) {
        while (j < i && i - j > queryD(1, 0, n, j + 1, i)) j++;
        g[i] = j;
        if (d[g[i]] > n) d[g[i]] = i;
    }
    buildUpdate(1, 0, n);
    solve(0, n);
    printf(f[n].max > 0 ? "%d %d\n" : "NIE\n", f[n].max, f[n].cnt);
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}