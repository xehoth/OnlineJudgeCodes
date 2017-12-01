/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「hihoCoder 1236」Scores 01-12-2017
 * 分块 + bitset
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

inline char read() {
    static const int IN_LEN = 100000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

const int OUT_LEN = 100000;

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
    ~InputOutputStream() { flush(); }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            c == '-' ? iosig = true : 0;
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        iosig ? x = -x : 0;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

template <size_t MAXN>
struct BitSet {
    static int n;

    unsigned long long a[MAXN / 64 + ((MAXN & 63) != 0)];

    inline void operator|=(const BitSet &p) {
        for (register int i = 0; i < n; i++) a[i] |= p.a[i];
    }

    inline void operator&=(const BitSet &p) {
        for (register int i = 0; i < n; i++) a[i] &= p.a[i];
    }

    inline void operator^=(const BitSet &p) {
        for (register int i = 0; i < n; i++) a[i] ^= p.a[i];
    }

    inline unsigned int count() {
        register unsigned int cnt = 0;
        for (register int i = 0; i < n; i++) cnt += __builtin_popcountll(a[i]);
        return cnt;
    }

    inline void reset() { memset(a, 0, sizeof(unsigned long long) * n); }

    inline void reset(const int i) { a[i >> 6] &= ~(1ull << (i & 63)); }

    inline void set(const int i) { a[i >> 6] |= (1ull << (i & 63)); }

    inline void set() { memset(a, 0xff, sizeof(unsigned long long) * n); }

    inline void flip() {
        for (register int i = 0; i < n; i++) a[i] ^= ULLONG_MAX;
    }

    inline void flip(const int i) { a[i >> 6] ^= (1ull << (i & 63)); }

    inline bool test(const int i) { return (a[i >> 6] >> (i & 63)) & 1; }

    inline BitSet operator|(const BitSet &p) const {
        BitSet ret = *this;
        ret |= p;
        return ret;
    }

    inline BitSet operator&(const BitSet &p) const {
        BitSet ret = *this;
        ret &= p;
        return ret;
    }

    inline BitSet operator^(const BitSet &p) const {
        BitSet ret = *this;
        ret ^= p;
        return ret;
    }

    inline int operator[](const int i) const {
        return (a[i >> 6] >> (i & 63)) & 1;
    }
};

template <size_t size>
int BitSet<size>::n;

const int MAXN = 50000 + 5;
const int MAX_BLOCK = 300 + 5;

BitSet<MAXN> d[5][MAX_BLOCK], ans[5];
int n, m, l[MAX_BLOCK], r[MAX_BLOCK], id[MAXN];
std::pair<int, int> scores[5][MAXN];

inline void init() {
    register int blockSize = sqrt(n) * 1.3;
    register int blockCnt = n / blockSize + (n % blockSize ? 1 : 0);
    l[0] = r[0] = -1;
    for (register int i = 1; i <= blockCnt; i++)
        l[i] = (i - 1) * blockSize, r[i] = i * blockSize;
    r[blockCnt] = n;
    for (register int i = 0; i < 5; i++) d[i][0].reset();
    for (register int i = 0; i < 5; i++) {
        std::sort(scores[i], scores[i] + n);
        for (register int j = 1; j <= blockCnt; j++) {
            d[i][j] = d[i][j - 1];
            for (register int k = l[j]; k < r[j]; k++)
                d[i][j].set(scores[i][k].second);
        }
    }
    for (register int i = 1; i <= blockCnt; i++)
        for (register int j = l[i]; j < r[i]; j++) id[j] = i;
}

void solve(int u, int x) {
    register int t =
        std::lower_bound(scores[u], scores[u] + n, std::make_pair(x, n)) -
        scores[u];
    ans[u] = d[u][id[t] - 1];
    for (register int i = l[id[t]]; i < t; i++) ans[u].set(scores[u][i].second);
}

inline void solveCase() {
    io >> n >> m;
    BitSet<MAXN>::n = n / 64 + ((n & 63) != 0);
    for (register int i = 0; i < n; i++)
        for (register int j = 0; j < 5; j++)
            io >> scores[j][i].first, scores[j][i].second = i;
    init();
    register int q;
    io >> q;
    for (register int last = 0, x; q--;) {
        for (register int i = 0; i < 5; i++) {
            io >> x;
            solve(i, x ^ last);
        }
        for (register int i = 4; i; i--) ans[i - 1] &= ans[i];
        last = ans[0].count();
        io << last << '\n';
    }
}

inline void solve() {
    register int T;
    io >> T;
    while (T--) solveCase();
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}