/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3196」二逼平衡树 08-09-2017
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
}  // namespace IO

namespace {

using IO::io;

const int MAXN = 50005;
const int SIZE = 900;

struct Task {
    int n, m, curBlock, curPos, max;

    int block[MAXN / SIZE + 1][SIZE], a[MAXN];

    inline void init() {
        io >> n >> m;
        for (register int i = 0; i < n; i++)
            io >> a[i], max = std::max(max, a[i]);
        for (register int i = 0; i < n; i++) {
            block[curBlock][curPos] = a[i];
            if (++curPos == SIZE) curBlock++, curPos = 0;
        }
        for (register int i = 0; i < curBlock; i++)
            std::sort(block[i], block[i] + SIZE);
        if (curPos) std::sort(block[curBlock], block[curBlock] + curPos);
    }

    inline int getRank(int l, int r, int v) {
        register int lb = l / SIZE, rb = r / SIZE, k = 1;
        if (lb == rb) {
            for (register int i = l; i <= r; i++) a[i] < v ? k++ : 0;
            return k;
        }
        for (register int i = l, end = lb * SIZE + SIZE; i < end; i++)
            a[i] < v ? k++ : 0;
        for (register int i = rb * SIZE; i <= r; i++) a[i] < v ? k++ : 0;
        for (register int i = lb + 1; i < rb; i++)
            k += std::lower_bound(block[i], block[i] + SIZE, v) - block[i];
        return k;
    }

    inline int select(int l, int r, int k) {
        register int s = -1, t = max + 1;
        for (register int mid; t - s > 1;)
            getRank(l, r, mid = s + t >> 1) > k ? t = mid : s = mid;
        return t - 1;
    }

    inline void modify(int p, int v) {
        if (a[p] == v) return;
        max = std::max(max, v);
        register int *cur = block[p / SIZE];
        register int old = a[p], sz = (p / SIZE) == curBlock ? curPos : SIZE;
        a[p] = v, p = std::lower_bound(cur, cur + sz, old) - cur;
        cur[p] = v;
        if (old < v) {
            while (p + 1 < SIZE && cur[p] > cur[p + 1])
                std::swap(cur[p], cur[p + 1]), p++;
        } else {
            while (p > 0 && cur[p] < cur[p - 1])
                std::swap(cur[p], cur[p - 1]), p--;
        }
    }

    inline int precursor(int l, int r, int v) {
        register int lb = l / SIZE, rb = r / SIZE, val = INT_MIN;
        if (lb == rb) {
            for (register int i = l; i <= r; i++)
                a[i] < v ? val = std::max(a[i], val) : 0;
            return val;
        }
        for (register int i = l, end = lb * SIZE + SIZE; i < end; i++)
            a[i] < v ? val = std::max(val, a[i]) : 0;
        for (register int i = rb * SIZE; i <= r; i++)
            a[i] < v ? val = std::max(val, a[i]) : 0;
        for (register int i = lb + 1, idx; i < rb; i++) {
            idx = std::lower_bound(block[i], block[i] + SIZE, v) - block[i];
            idx > 0 ? val = std::max(val, block[i][idx - 1]) : 0;
        }
        return val;
    }

    inline int successor(int l, int r, int v) {
        register int lb = l / SIZE, rb = r / SIZE, val = INT_MAX;
        if (lb == rb) {
            for (register int i = l; i <= r; i++)
                a[i] > v ? val = std::min(a[i], val) : 0;
            return val;
        }
        for (register int i = l, end = lb * SIZE + SIZE; i < end; i++)
            a[i] > v ? val = std::min(val, a[i]) : 0;
        for (register int i = rb * SIZE; i <= r; i++)
            a[i] > v ? val = std::min(val, a[i]) : 0;
        for (register int i = lb + 1, idx; i < rb; i++) {
            idx = std::upper_bound(block[i], block[i] + SIZE, v) - block[i];
            idx < SIZE ? val = std::min(val, block[i][idx]) : 0;
        }
        return val;
    }

    inline void solve() {
        register int cmd, l, r, x;
        for (init(); m--;) {
            io >> cmd;
            switch (cmd) {
                case 1:
                    io >> l >> r >> x, l--, r--;
                    io << getRank(l, r, x) << '\n';
                    break;
                case 2:
                    io >> l >> r >> x, l--, r--;
                    io << select(l, r, x) << '\n';
                    break;
                case 3:
                    io >> l >> x, l--;
                    modify(l, x);
                    break;
                case 4:
                    io >> l >> r >> x, l--, r--;
                    io << precursor(l, r, x) << '\n';
                    break;
                case 5:
                    io >> l >> r >> x, l--, r--;
                    io << successor(l, r, x) << '\n';
                    break;
            }
        }
    }
} task;
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    task.solve();
    return 0;
}
