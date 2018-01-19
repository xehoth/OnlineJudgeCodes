/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 2957」楼房重建 11-01-2018
 * 分块
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

const int OUT_LEN = 1 << 18 | 1;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    (oh == obuf + OUT_LEN) && (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf);
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[21], cnt;
    if (x != 0) {
        (x < 0) && (print('-'), x = -x);
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    } else {
        print('0');
    }
}

struct InputOutputStream {
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
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

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 100000 + 9;
const int MAX_BLOCK = 2000 + 9;

const double EPS = 1e-10;

double h[MAXN];

struct Block {
    std::vector<double> st;
    int l, r;

    inline int query(double k) {
        return st.size() -
               (std::upper_bound(st.begin(), st.end(), k + EPS) - st.begin());
    }

    inline void maintain() {
        st.clear();
        register double now = 0;
        for (register int i = l; i <= r; i++)
            (h[i] > now) && (st.push_back(h[i]), now = h[i]);
    }
} d[MAX_BLOCK];

int blockCnt, blockSize, id[MAXN];

inline void query() {
    register int ans = 0;
    register double now = 0;
    for (register int i = 1; i <= blockCnt; i++) {
        ans += d[i].query(now);
        (!d[i].st.empty()) && (now = std::max(now, d[i].st.back()));
    }
    io << ans << '\n';
}

inline void solve() {
    register int n, m;
    io >> n >> m;
    blockSize = ceil(sqrt(n * log2(n) * 0.6));
    for (register int i = 1; i <= n; i++) {
        id[i] = (i - 1) / blockSize + 1;
        if (!d[id[i]].l) d[id[i]].l = i;
        d[id[i]].r = i;
    }
    blockCnt = id[n];
    for (register int a, b; m--;) {
        io >> a >> b;
        h[a] = (double)b / a;
        d[id[a]].maintain();
        query();
    }
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}