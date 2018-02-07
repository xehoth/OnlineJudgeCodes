/*
 * created by xehoth on 15-03-2017
 * clang C++14
 */
// #pragma GCC diagnostic error "-std=c++11"
#include <bits/stdc++.h>

const int IN_LEN = 1000000;
char buf[IN_LEN], *s, *t;

inline char read() {
    if (s == t) {
        t = (s = buf) + fread(buf, 1, IN_LEN, stdin);
        if (s == t) return -1;
    }
    return *s++;
}

template <class T>
inline void read(T &x) {
    static bool iosig;
    static char c;
    for (iosig = false, c = read(); !isdigit(c); c = read()) {
        if (c == '-') iosig = true;
        if (c == -1) return;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    if (iosig) x = -x;
}

inline int read(char *buf) {
    register size_t s = 0;
    register char ch;
    while (ch = read(), isspace(ch) && ch != -1)
        ;
    if (ch == EOF) {
        *buf = '\0';
        return -1;
    }
    do
        buf[s++] = ch;
    while (ch = read(), !isspace(ch) && ch != -1);
    buf[s] = '\0';
    return s;
}

namespace SuffixAutomation {

const int MAXN = 200001;

struct Node {
    Node *next[26], *fa;

    int max;
    int minMatchLen, maxMatchLen;
    Node(int max = 0) : max(max), minMatchLen(max), maxMatchLen(0), fa(NULL) {}

    Node(int max, Node *p) {
        *this = *p, this->max = max, minMatchLen = max, maxMatchLen = 0;
    }

    inline void *operator new(size_t);
} pool[MAXN], *cur = pool, *root, *last;

inline void *Node::operator new(size_t) { return cur++; }

inline void init() {
    cur = pool;
    root = last = new Node();
}

inline Node *extend(int c, Node *p) {
    Node *np = new Node(p->max + 1);
    while (p && !p->next[c]) p->next[c] = np, p = p->fa;
    if (!p) {
        np->fa = root;
    } else {
        Node *q = p->next[c];
        if (q->max == p->max + 1) {
            np->fa = q;
        } else {
            Node *nq = new Node(p->max + 1, q);
            q->fa = np->fa = nq;
            while (p && p->next[c] == q) p->next[c] = nq, p = p->fa;
        }
    }
    return np;
}

std::vector<Node *> buc[MAXN];
Node *top[MAXN];
int tot;

inline void topoSort(const int n) {
    for (Node *p = pool; p != cur; p++) buc[p->max].push_back(p);
    for (register int i = 0; i <= n; i++)
        for (register int p = 0; p < buc[i].size(); p++) top[tot++] = buc[i][p];
}

inline int solve(const char *s, const int len) {
    Node *p = root;
    register int res = 0, max = 0;
    for (register int i = 0; i < len; i++) {
        if (p->next[s[i] - 'a']) {
            max++, p = p->next[s[i] - 'a'];
        } else {
            while (p && !p->next[s[i] - 'a']) p = p->fa;
            if (!p)
                max = 0, p = root;
            else
                max = p->max + 1, p = p->next[s[i] - 'a'];
        }
        p->maxMatchLen = std::max(p->maxMatchLen, max);
    }
    for (register int i = tot - 1; i; i--) {
        top[i]->minMatchLen =
            std::min(top[i]->minMatchLen, top[i]->maxMatchLen);
        top[i]->fa->maxMatchLen =
            std::max(top[i]->maxMatchLen, top[i]->fa->maxMatchLen);
        top[i]->maxMatchLen = 0, res = std::max(res, top[i]->minMatchLen);
    }
    return res;
}

inline void solve() {
    init();
    static char s[MAXN];
    register int n;
    read(n), n = read(s);
    for (register int i = 0; i < n; i++) last = extend(s[i] - 'a', last);
    topoSort(n);
    register int ans = n;
    while (~(n = read(s))) {
        ans = solve(s, n);
    }
    std::cout << ans;
}

}  // namespace SuffixAutomation

int main() {
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
#endif
    SuffixAutomation::solve();
    return 0;
}
