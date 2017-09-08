/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1030」文本生成器 08-09-2017
 * AC 自动机 + dp
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
}

namespace {

using IO::io;

const int MOD = 10007;
const int SIGMA = 26;
const int MAX_LEN = 100;
#define long long long

struct Node {
    Node *c[SIGMA], *fail;
    int cnt;

    Node();

    inline void *operator new(size_t);

} pool[MAX_LEN * 60 + 61], *cur = pool + 1, *null = pool;

inline void *Node::operator new(size_t) { return cur++; }

inline Node *newNode() { return cur++; }

Node::Node() : fail(null), cnt(0) {
    for (register int i = 0; i < SIGMA; i++) c[i] = null;
}

struct AhoCrasickAutomation {
    Node *root;

    inline void build() {
        static std::queue<Node *> q;
        q.push(root);
        for (Node *p, *u; !q.empty();) {
            p = q.front(), q.pop();
            for (register int i = 0; i < SIGMA; i++) {
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
            if (p->c[*s - 'A'] == null) p->c[*s - 'A'] = newNode();
            p = p->c[*s - 'A'];
        }
        p->cnt++;
    }

    int f[MAX_LEN + 1][MAX_LEN * 60 + 61];

    inline void dp(int m) {
        f[0][1] = 1;
        for (register int i = 1; i <= m; i++)
            for (Node *p = root; p != cur; p++)
                if (!p->cnt)
                    for (register int k = 0; k < SIGMA; k++)
                        f[i][p->c[k] - pool] =
                            (f[i][p->c[k] - pool] + f[i - 1][p - pool]) % MOD;
    }

    inline void init() {
        null->fail = null, root = new Node();
        for (register int i = 0; i < SIGMA; i++) null->c[i] = root;
        null->cnt = 0;
    }

    inline int modPow(int a, int b) {
        register int ret = 1;
        for (; b; b >>= 1, a = a * a % MOD) (b & 1) ? ret = ret * a % MOD : 0;
        return ret;
    }

    inline void solve() {
        register int n, m;
        io >> n >> m;
        init();
        static char s[MAX_LEN];
        for (register int i = 1; i <= n; i++) io >> s, insert(s);
        build();
        dp(m);
        register int ans = 0;
        for (Node *p = root; p != cur; p++)
            if (!p->cnt) ans = (ans + f[m][p - pool]) % MOD;
        io << (modPow(26, m) - ans + MOD) % MOD;
    }

} task;
}
int main() {
    task.solve();
    return 0;
}
