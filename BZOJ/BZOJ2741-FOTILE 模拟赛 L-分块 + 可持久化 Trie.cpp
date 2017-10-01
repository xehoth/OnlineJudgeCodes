/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2741」FOTILE 模拟赛 L 01-10-2017
 * 分块 + 可持久化 Trie
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
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
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
}  // end IO

namespace {

const int MAXN = 12000;
const int MAX_LOG = 30;
const int MAXM = MAXN * MAX_LOG * 2;

struct Node {
    Node *c[2];
    int v;

    Node();

    inline void *operator new(size_t);
} pool[MAXM + 1], *null = pool, *cur = pool + 1, *buc[MAXN + 2],
                  **root = buc + 1;

Node::Node() : v(0) { c[0] = c[1] = null; }

inline void *Node::operator new(size_t) { return cur++; }

inline void insert(Node *&p, Node *pre, int v, int dep = MAX_LOG) {
    p = new Node(*pre), p->v++;
    if (dep < 0) return;
    insert(p->c[v >> dep & 1], pre->c[v >> dep & 1], v, dep - 1);
}

inline int query(Node *l, Node *r, int v, int dep = MAX_LOG) {
    if (dep < 0) return 0;
    register int bit = v >> dep & 1;
    return r->c[!bit] - l->c[!bit] > 0
               ? (1 << dep) + query(l->c[!bit], r->c[!bit], v, dep - 1)
               : query(l->c[bit], r->c[bit], v, dep - 1);
}

using IO::io;

int block[121][MAXN + 1], lastans, a[MAXN + 1];

inline void solve() {
    register int n, m, blockSize, blockCount;
    io >> n >> m;
    blockSize = (int)(sqrt(n) * 4 / 3);
    blockCount = n / blockSize + (bool)(n % blockSize);
    for (register int i = 1; i <= n; i++) io >> a[i], a[i] ^= a[i - 1];
    root[0] = null, buc[0] = null;
    for (register int i = 1; i <= n; i++) insert(root[i], root[i - 1], a[i]);
    for (register int i = 1; i <= blockCount; i++) {
        for (register int j = (i - 1) * blockSize + 1; j <= n; j++) {
            block[i][j] =
                std::max(block[i][j - 1],
                         query(root[(i - 1) * blockSize], root[j], a[j]));
            if (i == 1) block[i][j] = std::max(block[i][j], a[j]);
        }
    }
    for (register int l, r; m--;) {
        lastans %= n;
        io >> l >> r;
        l = (l + lastans) % n + 1;
        r = (r + lastans) % n + 1;
        if (l > r) std::swap(l, r);
        l--, lastans = 0;
        register int t = 0;
        for (register int i = 1; i <= blockCount; i++) {
            if (l <= (i - 1) * blockSize + 1 && r >= (i - 1) * blockSize + 1) {
                lastans = block[i][r], t = i;
                break;
            }
        }
        if (t) {
            for (register int j = l; j <= (t - 1) * blockSize + 1; j++)
                lastans = std::max(lastans, query(root[l - 1], root[r], a[j]));
        } else {
            for (register int j = l; j <= r; j++)
                lastans = std::max(lastans, query(root[l - 1], root[r], a[j]));
        }
        io << lastans << '\n';
    }
}
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}
