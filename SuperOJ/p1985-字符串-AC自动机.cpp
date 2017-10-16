/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 1985」字符串 15-09-2017
 * AC 自动机
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

const int MAXN = 100000;
const int MAX_SIGMA = 26;

using IO::io;

struct Node {
    Node *c[MAX_SIGMA], *fail;
    int cnt;
    Node();

    inline void *operator new(size_t);
};

const int NODE_SIZE = sizeof(Node);

char pool[MAXN * NODE_SIZE], *cur = pool;

Node *null = (Node *)pool, *root;

Node::Node() : fail(null), cnt(0) {
    for (register int i = 0; i < MAX_SIGMA; i++) c[i] = null;
}

inline void *Node::operator new(size_t) { return cur += NODE_SIZE; }

inline void insert(const char *s) {
    register Node *p = root;
    for (register int idx = 0; *s; s++) {
        if (p->c[idx = *s - 'a'] == null) p->c[idx] = new Node();
        p = p->c[idx];
    }
    p->cnt++;
}

inline void init() {
    null->fail = null, root = new Node();
    for (register int i = 0; i < MAX_SIGMA; i++) null->c[i] = root;
    null->cnt = 0;
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
                p->c[i]->cnt += u->c[i]->cnt;
            } else {
                p->c[i] = p->fail->c[i];
            }
        }
    }
}

inline int query(const char *s, register int len) {
    register int ret = 0;
    register Node *p = root;
    for (register int i = 0; i < len; i++) p = p->c[s[i] - 'a'], ret += p->cnt;
    return ret;
}

char s[MAXN + 1];

inline void solve() {
    init();
    register int n, q, maxLen = 0;
    io >> n >> q;
    for (register int i = 1; i <= n; i++)
        maxLen = std::max(maxLen, IO::read(s)), insert(s);
    build();
    register int len = IO::read(s + 1);
    register char c;
    register int ans = query(s + 1, len);
    io << ans << '\n';
    for (register int pos; q--;) {
        io >> pos >> c;
        ans -=
            query(s + std::max(pos - maxLen, 1),
                  std::min(len, pos + maxLen) - std::max(pos - maxLen, 1) + 1);
        s[pos] = c;
        ans +=
            query(s + std::max(pos - maxLen, 1),
                  std::min(len, pos + maxLen) - std::max(pos - maxLen, 1) + 1);
        io << ans << '\n';
    }
}
}

int main() {
    // freopen("blocks.in", "r", stdin);
    // freopen("blocks.out", "w", stdout);
    solve();
    return 0;
}