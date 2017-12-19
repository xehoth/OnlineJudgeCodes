/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 4698」Sandy的卡片 19-12-2017
 * 后缀自动机
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

inline char read() {
    static const int IN_LEN = 1 << 18 | 1;
    static char buf[IN_LEN], *s, *t;
    return (s == t) && (t = (s = buf) + fread(buf, 1, IN_LEN, stdin)),
           s == t ? -1 : *s++;
}

struct InputStream {
    template <typename T>
    inline InputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        iosig && (x = -x);
        return *this;
    }
} io;

char *cur;

const int MAXN = 10000 + 9;

struct Node {
    static const int NODE_SIZE;

    std::map<int, Node *> c;
    Node *fa;
    int max, minMatch, maxMatch;

    Node(int max) : max(max), minMatch(max) {}

    inline void *operator new(size_t) { return cur += NODE_SIZE; }
};

const int Node::NODE_SIZE = sizeof(Node);

char pool[MAXN * 2 * Node::NODE_SIZE];

Node *root, *last;

inline Node *extend(register int c, register Node *p = last) {
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
            nq->minMatch = nq->max = p->max + 1;
            q->fa = np->fa = nq;
            while (p && p->c[c] == q) p->c[c] = nq, p = p->fa;
        }
    }
    return np;
}

Node *topo[MAXN * 2 - 1];

inline void topoSort() {
    static int buc[MAXN * 2 - 1];
    register int max = 0;

    for (register Node *p = (Node *)pool, *end = (Node *)cur; p != end; p++) {
        buc[p->max]++;
        max = std::max(max, p->max);
    }
    for (register int i = 1; i <= max; i++) buc[i] += buc[i - 1];
    for (register Node *p = (Node *)pool, *end = (Node *)cur; p != end; p++)
        topo[--buc[p->max]] = p;
}

inline int add(const std::vector<int> &a) {
    register Node *p = root;
    register std::map<int, Node *>::iterator it;
    register int ret = 0, max = 0;
    for (register int i = 1; i < (int)a.size(); i++) {
        it = p->c.find(a[i]);
        if (it != p->c.end()) {
            max++;
            p = it->second;
        } else {
            while (p && !p->c.count(a[i])) p = p->fa;
            if (!p) {
                max = 0;
                p = root;
            } else {
                max = p->max + 1;
                p = p->c[a[i]];
            }
        }
        p->maxMatch = std::max(p->maxMatch, max);
    }
    register int size = (Node *)cur - (Node *)pool;
    for (register int i = size - 1; i; i--) {
        topo[i]->minMatch = std::min(topo[i]->minMatch, topo[i]->maxMatch);
        topo[i]->fa->maxMatch =
            std::max(topo[i]->fa->maxMatch, topo[i]->maxMatch);
        topo[i]->maxMatch = 0, ret = std::max(ret, topo[i]->minMatch);
    }
    return ret;
}

inline int lcs(const std::vector<int> *a, const int n) {
    for (register int i = 1; i < (int)a[1].size(); i++) last = extend(a[1][i]);
    register int ans = a[1].size() - 1;
    topoSort();
    for (register int i = 2; i <= n; i++) {
        ans = std::min(ans, add(a[i]));
    }
    return ans;
}

inline void solve() {
    cur = pool;
    root = last = (Node *)pool;
    root->c = std::map<int, Node *>();
    register int n;
    io >> n;
    static std::vector<int> a[MAXN];

    register int max = INT_MAX;
    for (register int i = 1, m; i <= n; i++) {
        io >> m;
        max = std::min(max, m);
        a[i].resize(m + 1);
        for (register int j = 1; j <= m; j++) io >> a[i][j];
        for (register int j = m; j > 1; j--) a[i][j] -= a[i][j - 1];
    }
    register std::vector<int> *minSize = a + 1;
    for (register int i = 2; i <= n; i++) {
        if (a[i].size() < minSize->size()) {
            minSize = a + i;
        }
    }
    if (minSize != a + 1) minSize->swap(a[1]);
    printf("%d", std::min(lcs(a, n) + 1, max));
}
}  // namespace

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    solve();
    return 0;
}