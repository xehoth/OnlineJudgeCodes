/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4212」神牛的养成计划 13-12-2017
 * Trie
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

const int SIGMA = 26;
const int MAXN = 2000000 + 9;
const int MAXM = 2000 + 9;
namespace Trie {

char *cur;
struct Node {
    static const int NODE_SIZE;
    Node *c[SIGMA];
    int l, r;

    std::vector<int> str;

    Node() : l(INT_MAX), r(INT_MIN) {}

    inline void *operator new(size_t) { return cur += NODE_SIZE; }
};

const int Node::NODE_SIZE = sizeof(Node);

Node *root;

char pool[MAXN * Node::NODE_SIZE];

inline void insert(const char *s, const int id) {
    register Node *p = root;
    for (register int idx; *s; s++) {
        if (!p->c[idx = *s - 'a']) p->c[idx] = new Node();
        p = p->c[idx];
    }
    p->str.push_back(id);
}

inline void init() {
    cur = pool;
    root = new Node();
}

inline void update(const char *s, const int id, const int len) {
    register Node *p = root;
    for (register int i = 0; i < len; i++) {
        p = p->c[s[i] - 'a'];
        p->l = std::min(p->l, id), p->r = std::max(p->r, id);
    }
}

inline void query(const char *s, int &l, int &r) {
    register Node *p = root;
    for (register int idx; *s; s++) {
        if (!p->c[idx = *s - 'a']) {
            l = 0, r = -1;
            return;
        }
        p = p->c[idx];
    }
    l = p->l, r = p->r;
}

void dfs(Node *p, int *id, int &cnt) {
    for (register int i = 0; i < p->str.size(); i++) id[++cnt] = p->str[i];
    for (register int i = 0; i < SIGMA; i++)
        if (p->c[i]) dfs(p->c[i], id, cnt);
}

}  // namespace Trie

namespace PersistentTrie {

char *cur;
struct Node {
    static const int NODE_SIZE;
    Node *c[SIGMA];
    int cnt;

    inline void *operator new(size_t) { return cur += NODE_SIZE; }
};

const int Node::NODE_SIZE = sizeof(Node);

char pool[MAXN * Node::NODE_SIZE];

Node *root[MAXM];

inline void init(const int n) {
    cur = pool;
    root[0] = new Node();
}

inline void insert(Node *&p, Node *pre, const char *s, const int len) {
    p = new Node(*pre), p->cnt++;
    if (len == 0) return;
    register int idx = s[len - 1] - 'a';
    insert(p->c[idx], (pre->c[idx] ? pre->c[idx] : root[0]), s, len - 1);
}

inline int query(int l, int r, const char *s) {
    if (l > r) return 0;
    register Node *pl = root[l - 1], *pr = root[r];
    for (register int idx; *s; s++) {
        idx = *s - 'a';
        pl = pl->c[idx] ? pl->c[idx] : root[0];
        pr = pr->c[idx] ? pr->c[idx] : root[0];
        if (pl == root[0] && pr == root[0]) return 0;
    }
    return pr->cnt - pl->cnt;
}
}  // namespace PersistentTrie

int n, q;

int begin[MAXM], len[MAXM], id[MAXM];

char s[MAXN];

inline void solve() {
    io >> n;
    Trie::init(), PersistentTrie::init(n);
    for (register int i = 1, last = 0; i <= n; i++) {
        begin[i] = last;
        len[i] = read(s + last);
        Trie::insert(s + last, i);
        last += len[i];
    }

    register int cnt = 0;
    Trie::dfs(Trie::root, id, cnt);
    for (register int i = 1; i <= n; i++) {
        Trie::update(s + begin[id[i]], i, len[id[i]]);
        PersistentTrie::insert(PersistentTrie::root[i],
                               PersistentTrie::root[i - 1], s + begin[id[i]],
                               len[id[i]]);
    }

    io >> q;
    for (register int i = 1, len, l, r, last = 0; i <= q; i++) {
        len = read(s);
        for (register int i = 0; i < len; i++)
            s[i] = (s[i] - 'a' + last) % SIGMA + 'a';
        Trie::query(s, l, r);
        len = read(s);
        for (register int i = 0; i < len; i++)
            s[i] = (s[i] - 'a' + last) % SIGMA + 'a';
        std::reverse(s, s + len);

        io << (last = PersistentTrie::query(l, r, s)) << '\n';
    }
}

}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}