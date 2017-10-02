/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3995」道路修建 02-10-2017
 * 线段树
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 100000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
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

inline void read(char &x) {
    while (x = read(), isspace(x) && x != -1)
        ;
}

const int OUT_LEN = 100000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, oh - obuf, stdout), oh = obuf) : 0;
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
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    ~InputOutputStream() { flush(); }
} io;
}

namespace {

struct Data {
    int f[2][2];

    inline int *operator[](int i) { return f[i]; }

    inline const int *operator[](int i) const { return f[i]; }

    Data(int a = 0, int b = 0, int c = 0, int d = 0) {
        f[0][0] = a, f[0][1] = b, f[1][0] = c, f[1][1] = d;
    }

    inline static Data merge(const Data &l, const Data &r, int lenU, int lenD) {
        register Data d;
        register int lenMin = std::min(lenU, lenD), lenSum = lenU + lenD;
#define update(a, b)                         \
    d[a][b] =                                \
        std::min(lenMin + l[a][1] + r[1][b], \
                 lenSum + std::min(l[a][0] + r[1][b], l[a][1] + r[0][b]))
        update(0, 0), update(0, 1), update(1, 0), update(1, 1);
#undef update
        return d;
    }
};

const int MAXN = 60000;

int lenU[MAXN], lenD[MAXN], lenV[MAXN + 1];

inline Data calc(int x) { return Data(0, 0, 0, lenV[x]); }

struct Node {
    int l, r;
    Node *lc, *rc;
    Data d;

    Node(int, int);

    Node();

    inline void *operator new(size_t);

    inline void maintain() {
        d = Data::merge(lc->d, rc->d, lenU[l + r >> 1], lenD[l + r >> 1]);
    }

    inline void build() {
        if (r - l == 1) {
            d = calc(l);
        } else {
            register int mid = l + r >> 1;
            (lc = new Node(l, mid))->build(), (rc = new Node(mid, r))->build();
            maintain();
        }
    }

    inline void modify(int x) {
        if (r - l == 1) {
            d = calc(l);
        } else {
            (x < (l + r >> 1)) ? lc->modify(x) : rc->modify(x);
            maintain();
        }
    }

    inline Data query(int l, int r) {
        if (l == this->l && this->r == r) return d;
        register int mid = this->l + this->r >> 1;
        if (r <= mid)
            return lc->query(l, r);
        else if (l >= mid)
            return rc->query(l, r);
        else
            return Data::merge(lc->query(l, mid), rc->query(mid, r), lenU[mid],
                               lenD[mid]);
    }
} pool[MAXN * 4 + 1], *cur = pool + 1, *null = pool, *root;

Node::Node() : l(0), r(0), lc(null), rc(null) {}

Node::Node(int l, int r) : l(l), r(r), lc(null), rc(null) {}

inline void *Node::operator new(size_t) { return cur++; }

int n, m;

using IO::io;

inline void solve() {
    io >> n >> m;
    for (register int i = 1; i < n; i++) io >> lenU[i];
    for (register int i = 1; i < n; i++) io >> lenD[i];
    for (register int i = 0; i < n; i++) io >> lenV[i];
    (root = new Node(0, n))->build();
    for (register int i = 1; i <= m; i++) {
        register char cmd;
        io >> cmd;
        if (cmd == 'Q') {
            register int l, r;
            io >> l >> r, l--, r--;
            io << root->query(l, r + 1)[1][1] << '\n';
        } else if (cmd == 'C') {
            register int x0, y0, x1, y1, val;
            io >> x0 >> y0 >> x1 >> y1 >> val, y0--, y1--;
            if (x0 > x1 || (x0 == x1 && y0 > y1))
                std::swap(x0, x1), std::swap(y0, y1);
            if (x1 - x0 == 1) {
                lenV[y0] = val;
            } else if (y1 - y0 == 1) {
                if (x0 == 1)
                    lenU[y0 + 1] = val;
                else if (x0 == 2)
                    lenD[y0 + 1] = val;
            }
            root->modify(y0);
        }
    }
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}