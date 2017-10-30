/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 2022」颜色 30-10-2017
 * 分块
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
using IO::io;

const int MAXN = 100000;
const int MAXC = 100000;
const int MAX_BLOCK = 245;
const int BLOCK_SIZE = 412;
int blockSize;

int n, m, a[MAXN + 1], w[MAXC + 1], pre[MAXC + 1];

struct Block {
    std::pair<int, int> a[MAX_BLOCK + 1][BLOCK_SIZE + 1];
    std::pair<int, int> sorted[MAX_BLOCK + 1][BLOCK_SIZE + 1];
    int sum[MAX_BLOCK][BLOCK_SIZE + 1];
    int n, cnt, size[MAX_BLOCK + 1];

    inline void init(int n, int *color) {
        this->n = n, cnt = 0, blockSize = sqrt(n) * 1.3;
        register int r = 0, c = 0;
        for (register int i = 0; i < n; i++) {
            a[r][c].first = color[i], a[r][c].second = w[::a[i]];
            sorted[r][c] = a[r][c], size[r]++, c++;
            if (c == blockSize) c = 0, r++;
        }
        cnt = r + 1;
        for (register int i = 0; i < cnt; i++) {
            std::sort(sorted[i], sorted[i] + size[i]);
            sum[i][0] = sorted[i][0].second;
            for (register int j = 1; j < size[i]; j++)
                sum[i][j] += sum[i][j - 1] + sorted[i][j].second;
        }
    }

    inline void modify(int p, int x, int w) {
        register int r = p / blockSize, c = p % blockSize;

        if (a[r][c].first == x && a[r][c].second == w) return;
        register int old = a[r][c].first, oldW = a[r][c].second;
        a[r][c].first = x, a[r][c].second = w;
        register int pos = std::lower_bound(sorted[r], sorted[r] + size[r],
                                            std::make_pair(old, oldW)) -
                           sorted[r];
        sorted[r][pos].first = x, sorted[r][pos].second = w;
        if (std::make_pair(x, w) > std::make_pair(old, oldW)) {
            register int begin = pos;
            while (pos < size[r] - 1 && sorted[r][pos + 1] < sorted[r][pos])
                std::swap(sorted[r][pos], sorted[r][pos + 1]), pos++;
            if (begin == 0) sum[r][0] = sorted[r][begin].second, begin++;
            for (; begin < size[r]; begin++)
                sum[r][begin] = sum[r][begin - 1] + sorted[r][begin].second;
        } else if (std::make_pair(x, w) < std::make_pair(old, oldW)) {
            while (pos > 0 && sorted[r][pos - 1] > sorted[r][pos])
                std::swap(sorted[r][pos], sorted[r][pos - 1]), pos--;
            register int begin = pos;
            if (begin == 0) sum[r][0] = sorted[r][begin].second, begin++;
            for (; begin < size[r]; begin++)
                sum[r][begin] = sum[r][begin - 1] + sorted[r][begin].second;
        } else {
            register int begin = pos;
            if (begin == 0) sum[r][0] = sorted[r][begin].second, begin++;
            for (; begin < size[r]; begin++)
                sum[r][begin] = sum[r][begin - 1] + sorted[r][begin].second;
        }
    }

    inline int query(int l, int r, int x) {
        register int r1 = l / blockSize, r2 = r / blockSize;
        register int c1 = l % blockSize, c2 = r % blockSize;
        register int res = 0;
        if (r1 == r2) {
            while (c1 <= c2) {
                if (a[r1][c1].first < x) res += a[r1][c1].second;
                c1++;
            }
        } else {
            register int tmp = size[r1];
            while (c1 < tmp) {
                if (a[r1][c1].first < x) res += a[r1][c1].second;
                c1++;
            }
            r1++;
            while (c2 >= 0) {
                if (a[r2][c2].first < x) res += a[r2][c2].second;
                c2--;
            }
            for (register int pos; r1 < r2;) {
                pos = std::lower_bound(sorted[r1], sorted[r1] + size[r1],
                                       std::make_pair(x, 0)) -
                      sorted[r1];
                if (pos != 0) res += sum[r1][pos - 1];
                r1++;
            }
        }
        return res;
    }
} block;

typedef std::set<int> Set;
Set set[MAXC + 1];

inline void init() {
    for (register int i = 0; i < n; i++) {
        if (set[a[i]].empty())
            set[a[i]].insert(-1), set[a[i]].insert(set[a[i]].end(), n);
        pre[i] = *--set[a[i]].insert(--set[a[i]].end(), i);
    }
    block.init(n, pre);
}

inline void modify(int p, int x) {
    if (a[p] == x) return;
    register int old = a[p];
    a[p] = x, set[old].erase(p);
    register int tp = *set[old].upper_bound(p);
    if (tp != n)
        pre[tp] = *--set[old].lower_bound(p), block.modify(tp, pre[tp], w[old]);
    if (set[x].empty()) set[x].insert(-1), set[x].insert(set[x].end(), n);
    set[x].insert(p), tp = *set[x].upper_bound(p);
    if (tp != n) pre[tp] = p, block.modify(tp, p, w[x]);
    pre[p] = *--set[x].lower_bound(p), block.modify(p, pre[p], w[x]);
}

inline void solve() {
    io >> n >> m;
    for (register int i = 0; i < n; i++) io >> a[i];
    for (register int i = 1; i <= n; i++) io >> w[i];
    init();
    for (register int i = 1, l, r, cmd; i <= m; i++) {
        io >> cmd;
        switch (cmd) {
            case 1:
                io >> l >> r, modify(l - 1, r);
                break;
            case 2:
                io >> l >> r, io << block.query(l - 1, r - 1, l - 1) << '\n';
                break;
        }
    }
}
}

int main() {
    // freopen("color.in", "r", stdin);
    // freopen("color.out", "w", stdout);
    solve();
    return 0;
}