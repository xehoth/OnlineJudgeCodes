/*
 * created by xehoth on 12-03-2017
 */
#include <bits/stdc++.h>

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
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

const int OUT_LEN = 1000000;
char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}

template <class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        if (x < 0) print('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

const int MAXN = 4000010;

std::vector<int> g[MAXN];
int n, m, c[MAXN], in[MAXN];

namespace SuffixAutomation {

struct Node {
    Node *fa, *next[10];
    int max;

    Node(int max = 0) : max(max) {}

    inline void *operator new(size_t);
} pool[MAXN], *cur = pool, *root;

inline void *Node::operator new(size_t) { return cur++; }

inline Node *extend(int c, Node *p) {
    Node *np = new Node(p->max + 1);
    while (p && !p->next[c]) p->next[c] = np, p = p->fa;
    if (!p) {
        np->fa = root;
    } else {
        Node *q = p->next[c];
        if (p->max + 1 == q->max) {
            np->fa = q;
        } else {
            Node *nq = new Node(p->max + 1);
            memcpy(nq->next, q->next, sizeof(q->next));
            nq->fa = q->fa, q->fa = nq, np->fa = nq;
            while (p && p->next[c] == q) p->next[c] = nq, p = p->fa;
        }
    }
    return np;
}

inline void dfs(int u, int fa, Node *p) {
    Node *t = extend(c[u], p);
    for (register int i = 0, v; i < g[u].size(); i++) {
        v = g[u][i];
        if (v != fa) dfs(v, u, t);
    }
}

inline void solve() {
    root = new Node();
    read(n), read(m);
    for (register int i = 1; i <= n; i++) read(c[i]);
    int u, v;
    for (register int i = 1, u, v; i < n; i++) {
        read(u), read(v);
        g[u].push_back(v), g[v].push_back(u);
        in[v]++, in[u]++;
    }
    for (register int i = 1; i <= n; i++)
        if (in[i] == 1) dfs(i, -1, root);
    register long long ans = 0;
    for (register Node *p = pool; p != cur; p++)
        ans += p->max - (p->fa ? p->fa->max : 0);
    print(ans);
}

};  // namespace SuffixAutomation

int main() {
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
#endif
    SuffixAutomation::solve();
    flush();
    return 0;
}