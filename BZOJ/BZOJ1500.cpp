#include <bits/stdc++.h>

const int MAXN = 500010;
const int INIT_VAL = 1001;
const int INF = 2000000000;

inline int read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *h, *t;
    if (h == t) {
        t = (h = buf) + fread(buf, 1, IN_LEN, stdin);
        if (h == t) return -1;
    }
    return *h++;
}

template <class T>
inline bool read(T &x) {
    static bool iosig = 0;
    static char c;
    for (iosig = 0, c = read(); !isdigit(c); c = read()) {
        if (c == -1) return false;
        if (c == '-') iosig = 1;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    if (iosig) x = -x;
    return true;
}

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = '\0';
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = '\0';
    return s;
}

const int OUT_LEN = 10000000;
char obuf[OUT_LEN], *oh = obuf;
inline void print(const char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}

template <class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (!x)
        print('0');
    else {
        if (x < 0) print('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

namespace Treap {

inline int rand() {
    static int seed = 495;
    seed += seed << 1 | 1;
    return seed;
}

inline int max(int a, int b) { return a > b ? a : b; }
#define bitmax(x, y) (x ^ ((x ^ y) & -(x < y)))

struct Node *null;

struct Node {
    Node *lc, *rc;
    int key, rank, rev, tag, size;
    int sum, lSum, rSum, maxSum;

    Node(int key = 0)
        : key(key),
          rank(rand()),
          lc(null),
          rc(null),
          rev(0),
          tag(INIT_VAL),
          size(1) {
        sum = lSum = rSum = maxSum = key;
    }

    inline void maintain() {
        size = lc->size + rc->size + 1;
        sum = lc->sum + rc->sum + key;
        lSum = max(lc->lSum, lc->sum + key + bitmax(0, rc->lSum));
        rSum = max(rc->rSum, rc->sum + key + bitmax(0, lc->rSum));
        maxSum = bitmax(0, lc->rSum) + key + bitmax(0, rc->lSum);
        maxSum = max(maxSum, bitmax(lc->maxSum, rc->maxSum));
    }

    inline void cover(int v) {
        if (this == null) return;
        key = v, sum = v * size;
        lSum = rSum = maxSum = bitmax(sum, v);
        tag = v;
    }

    inline void reverse() {
        if (this == null) return;
        std::swap(lc, rc);
        std::swap(lSum, rSum);
        rev ^= 1;
    }

    inline void pushDown() {
        if (this == null) return;
        if (tag != INIT_VAL) {
            lc->cover(tag), rc->cover(tag);
            tag = INIT_VAL;
        }
        if (rev) {
            lc->reverse(), rc->reverse();
            rev = 0;
        }
    }

    inline void *operator new(size_t);

    inline void operator delete(void *o);
};
Node *rt;

const int NODE_SIZE = sizeof(Node);
char pool[MAXN * NODE_SIZE], *bin[MAXN], **top = bin;

typedef std::pair<Node *, Node *> Pair;

inline void *Node::operator new(size_t) { return (void *)*--top; }

inline void Node::operator delete(void *o) { *top++ = (char *)o; }

inline Node *build(int *a, int n) {
    static Node *stack[MAXN], *u, *pre;
    register int top = 0;
    for (register int i = 1; i <= n; i++) {
        u = new Node(a[i]);
        pre = null;
        while (top && stack[top]->rank > u->rank) {
            stack[top]->maintain();
            pre = stack[top];
            stack[top--] = null;
        }
        if (top) stack[top]->rc = u;
        u->lc = pre;
        stack[++top] = u;
    }
    while (top) stack[top--]->maintain();
    return stack[1];
}

inline Node *merge(Node *u, Node *v) {
    if (u == null) return v;
    if (v == null) return u;
    if (u->rank < v->rank) {
        u->pushDown();
        u->rc = merge(u->rc, v);
        u->maintain();
        return u;
    } else {
        v->pushDown();
        v->lc = merge(u, v->lc);
        v->maintain();
        return v;
    }
}

inline Pair split(Node *u, int k) {
    if (u == null) return Pair(null, null);
    Pair t;
    u->pushDown();
    if (u->lc->size >= k) {
        t = split(u->lc, k);
        u->lc = t.second, t.second = u;
    } else {
        t = split(u->rc, k - u->lc->size - 1);
        u->rc = t.first, t.first = u;
    }
    u->maintain();
    return t;
}

inline void recycle(Node *u) {
    if (u == null) return;
    recycle(u->lc), recycle(u->rc);
    delete u;
}

int pos, cnt;

inline void insert(int *a) {
    for (register int i = 1; i <= cnt; i++) read(a[i]);
    Node *nr = build(a, cnt);
    Pair t = split(rt, pos);
    rt = merge(merge(t.first, nr), t.second);
}

inline void remove() {
    Pair t1 = split(rt, pos + cnt - 1), t2 = split(t1.first, pos - 1);
    recycle(t2.second);
    rt = merge(t2.first, t1.second);
}

inline void reverse() {
    Pair t1 = split(rt, pos + cnt - 1), t2 = split(t1.first, pos - 1);
    t2.second->reverse();
    rt = merge(merge(t2.first, t2.second), t1.second);
}

inline void makeSame() {
    register int v;
    read(v);
    Pair t1 = split(rt, pos + cnt - 1), t2 = split(t1.first, pos - 1);
    t2.second->cover(v);
    rt = merge(merge(t2.first, t2.second), t1.second);
}

inline void querySum() {
    Pair t1 = split(rt, pos + cnt - 1), t2 = split(t1.first, pos - 1);
    print(t2.second->sum), print('\n');
    rt = merge(merge(t2.first, t2.second), t1.second);
}

inline void queryMaxSum() { print(rt->maxSum), print('\n'); }

inline void solve() {
    static int n, m, a[MAXN];
    read(n), read(m);
    for (register int i = 1; i <= n; i++) read(a[i]);
    for (register int i = 0, j = 0; i < MAXN; i++, j += NODE_SIZE)
        *top++ = &pool[j];
    null = new Node(-INF);
    null->size = null->sum = 0;
    rt = build(a, n);
    char cmd[20];
    for (register int i = 1; i <= m; i++) {
        read(cmd);
        if (cmd[2] != 'X') read(pos), read(cnt);
        switch (cmd[2]) {
            case 'S':
                insert(a);
                break;
            case 'L':
                remove();
                break;
            case 'V':
                reverse();
                break;
            case 'T':
                querySum();
                break;
            case 'X':
                queryMaxSum();
                break;
            case 'K':
                makeSame();
                break;
        }
    }
}
}  // namespace Treap
int main() {
    // freopen("in.in", "r", stdin);
    // srand(495);
    Treap::solve();
    flush();
    return 0;
}