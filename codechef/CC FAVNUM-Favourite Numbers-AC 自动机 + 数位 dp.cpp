/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「CC FAVNUM」Favourite Numbers 26-09-2017
 * AC 自动机 + 数位 dp
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
const int MAXN = 2000;
const int MAX_SIGMA = 10;

struct Node {
    Node *c[MAX_SIGMA], *fail;
    int cnt;

    inline void *operator new(size_t);

    Node();
} pool[MAXN + 2], *null = pool, *cur = pool + 1;

inline void *Node::operator new(size_t) { return cur++; }

Node::Node() {
    fail = null, cnt = 0;
    for (register int i = 0; i < MAX_SIGMA; i++) c[i] = null;
}

struct AhoCrasickAutomation {
    Node *root;
    long f[20][MAXN + 2][2];
    int digit[20];

    AhoCrasickAutomation() {
        root = new Node();
        for (register int i = 0; i < MAX_SIGMA; i++) null->c[i] = root;
    }

    inline void build() {
        static std::queue<Node *> q;
        q.push(root);
        for (Node *p, *u; !q.empty();) {
            p = q.front(), q.pop();
            for (register int i = 0; i < MAX_SIGMA; i++) {
                if (p->c[i] != null) {
                    for (u = p->fail; u->c[i] == null;) u = u->fail;
                    p->c[i]->fail = u->c[i], q.push(p->c[i]);
                    p->c[i]->cnt |= u->c[i]->cnt;
                } else {
                    p->c[i] = p->fail->c[i];
                }
            }
        }
    }

    inline void insert(const char *s) {
        Node *p = root;
        for (; *s; s++) {
            if (p->c[*s - '0'] == null) p->c[*s - '0'] = cur++;
            p = p->c[*s - '0'];
        }
        p->cnt = 1;
    }

    inline long dfs(int pos, Node *p, int status, int limit) {
        if (pos < 1) return status;
        if (!limit && f[pos][p - pool][status] != -1)
            return f[pos][p - pool][status];
        register int end = limit ? digit[pos] : 9;
        register long ret = 0;
        for (register int i = 0; i <= end; i++)
            ret += dfs(pos - 1, p->c[i], status || p->c[i]->cnt,
                       limit && i == end);
        return !limit ? f[pos][p - pool][status] = ret : ret;
    }

    inline long solve(long x) {
        register int len = 0;
        for (; x; x /= 10) digit[++len] = x % 10;
        return dfs(len, root, 0, 1);
    }

    inline void solve() {
        register long l, r, k, n, step, w;
        io >> l >> r >> k >> n;
        static char buf[20];
        for (register int i = 0; i < n; i++) io >> buf, insert(buf);
        build();
        memset(f, -1, sizeof(f));
        k += solve(l - 1);
        if (solve(r) < k) {
            io << "no such number";
            return;
        }
        step = 1, w = l - 1;
        while (step < (r - l + 1)) step <<= 1;
        while (step) {
            if (solve(w + step) < k) w += step;
            step >>= 1;
        }
        io << w + 1;
    }
} task;

#undef long
}

int main() {
    task.solve();
    return 0;
}
