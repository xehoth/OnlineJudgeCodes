/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3744」Gty的妹子序列 09-10-2017
 * 分块 + 树状数组
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

int n;

int blockSize, blockCount;

const int MAXN = 50000;
const int MAX_BLOCK_SIZE = (223 + 1) * 3 / 2 + 1;
const int MAX_BLOCK_COUNT = MAXN / MAX_BLOCK_SIZE + 2;

struct BinaryIndexedTree {
    int d[MAXN + 1];

    inline void update(register int k, register int x = 1) {
        for (; k <= n; k += k & -k) d[k] += x;
    }

    inline int query(register int k) {
        register int ret = 0;
        for (; k; k ^= k & -k) ret += d[k];
        return ret;
    }

    inline int query(int l, int r) {
        return l > r ? 0 : query(r) - query(l - 1);
    }

    inline void clear() { memset(d, 0, sizeof(int) * (n + 1)); }
} bits[MAX_BLOCK_COUNT + 1], bit;

int a[MAXN + 1], f[MAX_BLOCK_COUNT + 1][MAX_BLOCK_COUNT + 1];

inline int getId(const int i) { return (i - 1) / blockSize + 1; }

inline void getBlock(int i, int &l, int &r) {
    l = (i - 1) * blockSize + 1, r = std::min(n, l + blockSize - 1);
}

inline int getBlockL(int i) { return (i - 1) * blockSize + 1; }

inline int getBlockR(int i) {
    return std::min(n, getBlockL(i) + blockSize - 1);
}

inline void init() {
    blockSize = sqrt(n) * 3 / 2, blockCount = std::ceil((double)n / blockSize);
    static int tmp[MAXN + 1];
    memcpy(tmp + 1, a + 1, sizeof(int) * n);
    std::sort(tmp + 1, tmp + n + 1);
    register int *end = std::unique(tmp + 1, tmp + n + 1);
    for (register int i = 1; i <= n; i++)
        a[i] = std::lower_bound(tmp + 1, end, a[i]) - tmp;
    for (register int i = 1, l, r; i <= blockCount; i++) {
        bits[i] = bits[i - 1], getBlock(i, l, r);
        for (register int j = l; j <= r; j++) bits[i].update(a[j]);
    }
    for (register int i = 1, l, r; i <= blockCount; i++) {
        bit.clear();
        for (register int j = i; j <= blockCount; j++) {
            f[i][j] = f[i][j - 1], getBlock(j, l, r);
            for (register int k = l; k <= r; k++) {
                f[i][j] += bit.query(a[k] + 1, n);
                bit.update(a[k], 1);
            }
        }
    }
}

inline int force(int l, int r) {
    register int ans = 0;
    bit.clear();
    for (register int i = l; i <= r; i++)
        ans += bit.query(a[i] + 1, n), bit.update(a[i]);
    return ans;
}

inline int query(int l, int r) {
    register int lb = getId(l), rb = getId(r);
    if (rb - lb <= 1) return force(l, r);
    register int ans = f[lb + 1][rb - 1];
    register int lbr = getBlockR(lb), rbl = getBlockL(rb);
    for (register int i = lbr; i >= l; i--) {
        bits[rb - 1].update(a[i], 1);
        ans += bits[rb - 1].query(1, a[i] - 1) - bits[lb].query(1, a[i] - 1);
    }
    for (register int i = rbl; i <= r; i++) {
        bits[rb - 1].update(a[i], 1);
        ans += bits[rb - 1].query(a[i] + 1, n) - bits[lb].query(a[i] + 1, n);
    }
    for (register int i = rbl; i <= r; i++) bits[rb - 1].update(a[i], -1);
    for (register int i = lbr; i >= l; i--) bits[rb - 1].update(a[i], -1);
    return ans;
}

using IO::io;

inline void solve() {
    io >> n;
    for (register int i = 1; i <= n; i++) io >> a[i];
    init();
    register int m;
    io >> m;
    register int lastAns = 0;
    for (register int l, r; m--;) {
        io >> l >> r;
        l ^= lastAns, r ^= lastAns;
        io << (lastAns = query(l, r)) << '\n';
    }
}
}

int main() {
    solve();
    return 0;
}