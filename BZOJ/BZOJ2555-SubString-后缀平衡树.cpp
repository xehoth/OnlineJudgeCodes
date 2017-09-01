/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 2555」SubString 01-09-2017
 * 后缀平衡树
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
}

namespace {

using IO::io;

const int MAXN = 3000005;
const double ALPHA = 0.75;

struct Node {
    Node *c[2];
    double v;
    int id, size;

    Node();

    Node(double v, int id);

    inline void maintain() { size = c[0]->size + c[1]->size + 1; }

    inline bool check() {
        return std::max(c[0]->size, c[1]->size) > size * ALPHA;
    }

    inline void *operator new(size_t);
} pool[MAXN + 1], *null = pool, *cur = pool + 1;

Node::Node() {
    c[0] = null;
    c[1] = null;
    id = 0;
    v = 0;
    size = 0;
}

Node::Node(double v, int id) {
    c[0] = null;
    c[1] = null;
    this->v = v;
    this->id = id;
    this->size = 1;
}

inline void *Node::operator new(size_t) { return cur++; }

struct SuffixBalancedTree {
    Node *root, *pos[MAXN + 1];

    int n;

    inline void init() {
        root = new Node(0.5, 0);
        pos[0] = root;
        s[0] = 'A' - 1;
        for (register int i = 1; i <= n; i++) insert(i);
    }

    double badL, badR;

    char s[MAXN + 1], qs[MAXN + 1];

    int qlen, last;

    inline bool cmpSuffix(int a, int b) {
        return s[a] < s[b] || (s[a] == s[b] && pos[a - 1]->v < pos[b - 1]->v);
    }

    inline Node **insert(Node *&p, double l, double r, int id) {
        if (p == null) {
            pos[id] = p = new Node((l + r) / 2, id);
            return &null;
        } else {
            p->size++;
            Node **res = cmpSuffix(id, p->id)
                             ? insert(p->c[0], l, (l + r) / 2, id)
                             : insert(p->c[1], (l + r) / 2, r, id);

            if (p->check()) res = &p, badL = l, badR = r;
            return res;
        }
    }

    inline void travel(Node *p, std::vector<Node *> &v) {
        if (p == null) return;
        travel(p->c[0], v), v.push_back(p), travel(p->c[1], v);
    }

    inline void dfs(Node *p) {
        if (p == null) return;
        dfs(p->c[0]), std::cerr << p->id << std::endl, dfs(p->c[1]);
    }

    inline Node *divide(std::vector<Node *> &v, int l, int r, double lv,
                        double rv) {
        if (l >= r) return null;
        register int mid = l + r >> 1;
        Node *p = v[mid];
        p->v = (lv + rv) / 2;
        p->c[0] = divide(v, l, mid, lv, (lv + rv) / 2);
        p->c[1] = divide(v, mid + 1, r, (lv + rv) / 2, rv);
        p->maintain();
        return p;
    }

    inline void rebuild(Node *&p) {
        static std::vector<Node *> v;
        v.clear(), travel(p, v), p = divide(v, 0, v.size(), badL, badR);
    }

    inline void insert(int id) {
        Node **p = insert(root, 0, 1, id);
        if (*p != null) rebuild(*p);
    }

    int mask;

    inline void decode(int mask) {
        qlen = IO::read(qs + 1);

        char *t = qs + 1;
        for (register int i = 0; i < qlen; i++) {
            mask = (mask * 131 + i) % qlen;
            std::swap(t[i], t[mask]);
        }
    }

    inline int rank() {
        register int ans = 1;
        for (Node *p = root; p != null;) {
            for (register int i = 1; i <= qlen; i++) {
                if (s[p->id - i + 1] != qs[i]) {
                    if (s[p->id - i + 1] < qs[i]) {
                        ans += p->c[0]->size + 1, p = p->c[1];
                        break;
                    } else {
                        p = p->c[0];
                        break;
                    }
                }
            }
        }
        return ans - 2;
    }

    inline void query() {
        if (qlen > n) {
            last = 0, io << last << '\n';
            mask ^= last;
            return;
        }
        std::reverse(qs + 1, qs + qlen + 1);
        qs[++qlen] = 'A' - 2;
        last = -rank();
        qs[qlen] = 'Z' + 2;
        last += rank();
        io << last << '\n';
        mask ^= last;
    }

    inline void solve() {
        register int q = 0;
        io >> q;
        n = IO::read(s + 1);
        init();
        static char cmd[10];
        for (; q--;) {
            io >> cmd;
            decode(mask);
            switch (cmd[0]) {
                case 'Q':
                    query();
                    break;
                case 'A':
                    for (register int i = 1; i <= qlen; i++)
                        s[++n] = qs[i], insert(n);
                    break;
            }
        }
    }
} suffixBalancedTree;
}

int main() {
    suffixBalancedTree.solve();
    return 0;
}