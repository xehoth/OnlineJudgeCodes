/*
 * created by xehoth on 25-03-2017
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
    static char c;
    static bool iosig;
    for (iosig = false, c = read(); !isdigit(c); c = read()) {
        if (c == -1) return;
        if (c == '-') iosig = true;
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

namespace SegmentTree {

const int MAXN = 500010;

struct Node {
    Node *lc, *rc;
    int cnt;

    Node(int cnt = 0) : cnt(cnt), lc(NULL), rc(NULL) {}

    Node(int cnt, Node *p) { *this = *p, this->cnt = cnt; }

    inline void *operator new(size_t);
} pool[MAXN * 20], *cur = pool, *last, *root[MAXN];

inline void *Node::operator new(size_t) { return cur++; }

inline void insert(Node *&cur, Node *p, int l, int r, int val) {
    cur = new Node(p->cnt + 1, p);
    if (l == r) return;
    register int mid = l + r >> 1;
    if (val <= mid)
        insert(cur->lc, p->lc, l, mid, val);
    else
        insert(cur->rc, p->rc, mid + 1, r, val);
}

int n, m, limit;

inline int query(int l, int r) {
    register int len = r - l + 1 >> 1;
    Node *p = root[l - 1], *q = root[r];
    register int s = 1, t = limit;
    if (q->cnt - p->cnt <= len) return 0;
    while (s != t) {
        register int mid = s + t >> 1;
        if (q->lc->cnt - p->lc->cnt > len)
            q = q->lc, p = p->lc, t = mid;
        else if (q->rc->cnt - p->rc->cnt > len)
            q = q->rc, p = p->rc, s = mid + 1;
        else
            return 0;
    }
    return s;
}

inline void solve() {
    read(n), read(limit);
    root[0] = new Node(), root[0]->lc = root[0]->rc = root[0];
    for (register int i = 1, x; i <= n; i++) {
        read(x), insert(root[i], root[i - 1], 1, limit, x);
    }
    read(m);
    for (register int i = 1, l, r, num; i <= m; i++) {
        read(l), read(r), num = query(l, r);
        num ? (print('y'), print('e'), print('s'), print(' '), print(num))
            : (print('n'), print('o'));
        print('\n');
    }
}
}  // namespace SegmentTree

int main() {
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
#endif
    SegmentTree::solve();
    flush();
    return 0;
}
