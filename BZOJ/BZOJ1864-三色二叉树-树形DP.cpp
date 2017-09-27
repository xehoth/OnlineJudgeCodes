/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1864」三色二叉树 27-09-2017
 * 树形 DP
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

inline void read(double &x) {
    static char buf[30];
    read(buf), x = atof(buf);
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

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

inline void print(double x) {
    static char buf[30];
    sprintf(buf, "%.1f", x);
    print((const char *)buf);
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

#define long long long

using IO::io;

const int MAXN = 500000;

struct Node {
    Node *lc, *rc;

    Node();

    int min[2], max[2];

    inline void *operator new(size_t);
} pool[MAXN + 1], *null = pool, *cur = pool + 1;

Node::Node() : lc(null), rc(null) {}

inline void *Node::operator new(size_t) { return cur++; }

char buf[MAXN + 1];
int now = -1;

inline void build(Node *&p) {
    register char c = buf[++now];
    if (c == '0') return;
    p->lc = new Node();
    build(p->lc);
    if (c == '2') p->rc = new Node(), build(p->rc);
}

inline void dfs(Node *p) {
    if (p == null) return;
    dfs(p->lc), dfs(p->rc);
    p->max[1] = p->lc->max[0] + p->rc->max[0] + 1;
    p->min[1] = p->lc->min[0] + p->rc->min[0] + 1;
    p->max[0] =
        std::max(p->lc->max[0] + p->rc->max[1], p->lc->max[1] + p->rc->max[0]);
    p->min[0] =
        std::min(p->lc->min[0] + p->rc->min[1], p->lc->min[1] + p->rc->min[0]);
}

inline void solve() {
    io >> buf;
    Node *root = new Node();
    build(root);
    dfs(root);
    io << std::max(root->max[0], root->max[1]) << ' '
       << std::min(root->min[0], root->min[1]);
}
#undef long
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}
