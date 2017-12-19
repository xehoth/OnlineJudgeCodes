/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4327」玄武密码 19-12-2017
 * 后缀自动机
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

inline char read() {
    static const int IN_LEN = 1 << 18 | 1;
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

const int OUT_LEN = 1 << 18 | 1;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[21], cnt;
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
    ~InputOutputStream() { flush(); }

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
} io;

const int MAXN = 1e7 + 9;
const int SIGMA = 4;

char *cur;

struct Node {
    static const int NODE_SIZE;
    Node *c[SIGMA], *fa;
    int max;

    Node(int max) : max(max) {}

    inline void *operator new(size_t) { return cur += NODE_SIZE; }
};

const int Node::NODE_SIZE = sizeof(Node);

char pool[MAXN * 2 * Node::NODE_SIZE];

Node *root, *last;

inline Node *extend(int c, register Node *p = last) {
    register Node *np = new Node(p->max + 1);
    while (p && !p->c[c]) p->c[c] = np, p = p->fa;
    if (!p) {
        np->fa = root;
    } else {
        register Node *q = p->c[c];
        if (q->max == p->max + 1) {
            np->fa = q;
        } else {
            register Node *nq = new Node(*q);
            nq->max = p->max + 1;
            q->fa = np->fa = nq;
            while (p && p->c[c] == q) p->c[c] = nq, p = p->fa;
        }
    }
    return np;
}

inline int convert(char ch) {
    switch (ch) {
        case 'N':
            return 0;
        case 'S':
            return 1;
        case 'W':
            return 2;
        case 'E':
            return 3;
#ifdef DBG
        default:
            assert(false);
#endif
    }
}

char s[MAXN];

inline int solve(const char *s, const int len) {
    register Node *p = root, *next;
    for (register int i = 0; i < len; i++) {
        next = p->c[convert(s[i])];
        if (next) {
            p = next;
        } else {
            return i;
        }
    }
    return len;
}

inline void solve() {
    cur = pool;
    root = last = (Node *)pool;
    register int n, m;
    io >> n >> m;
    read(s);
    for (register int i = 0; i < n; i++) {
        last = extend(convert(s[i]));
    }
    for (register int len; m--;) {
        len = read(s);
        io << solve(s, len) << '\n';
    }
}
}  // namespace

int main() {
    solve();
    return 0;
}