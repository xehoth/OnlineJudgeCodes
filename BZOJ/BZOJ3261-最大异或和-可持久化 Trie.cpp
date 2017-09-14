/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 3261」最大异或和 14-09-2017
 * 可持久化 Trie
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

const int MAXN = 600000;
const int MAX_LOG = 25;

struct Node {
    Node *c[2];
    int cnt;

    Node();

    inline void *operator new(size_t);
} pool[MAXN * MAX_LOG + 1], *cur = pool + 1, *null = pool;

Node::Node() : cnt(0) { c[0] = c[1] = null; }

inline void *Node::operator new(size_t) { return cur++; }

struct PersistentTrie {
    Node *root[MAXN];

    inline void insert(Node *&p, Node *pre, int v) {
        p = cur++;
        Node *q = p;
        for (register int i = MAX_LOG, bit; i >= 0; i--) {
            *q = Node(*pre), q->cnt++, bit = v >> i & 1;
            q->c[bit] = cur++, pre = pre->c[bit], q = q->c[bit];
        }
        q->cnt = pre->cnt + 1;
    }

    inline int query(Node *l, Node *r, const int v) {
        register int ans = 0;
        for (register int i = MAX_LOG, bit; i >= 0; i--) {
            bit = v >> i & 1;
            if (r->c[bit ^ 1]->cnt - l->c[bit ^ 1]->cnt > 0) {
                ans += 1 << i;
                l = l->c[bit ^ 1], r = r->c[bit ^ 1];
            } else {
                l = l->c[bit], r = r->c[bit];
            }
        }
        return ans;
    }

    inline void solve() {
        register int n, m;
        io >> n >> m;
        static int a[MAXN + 1];
        for (register int i = 2; i <= n + 1; i++) io >> a[i], a[i] ^= a[i - 1];
        root[0] = null;
        for (register int i = 1; i <= n + 1; i++)
            insert(root[i], root[i - 1], a[i]);
        n++;
        while (m--) {
            register char op;
            io >> op;
            if (op == 'A') {
                register int tmp;
                io >> tmp;
                a[++n] = tmp;
                a[n] ^= a[n - 1];
                insert(root[n], root[n - 1], a[n]);
            } else {
                register int l, r, x;
                io >> l >> r >> x;
                x ^= a[n];
                io << query(root[l - 1], root[r], x) << '\n';
            }
        }
    }
} task;
}

int main() {
    task.solve();
    return 0;
}
