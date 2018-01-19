/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 4170」极光 04-01-2018
 * 树状数组套可持久化线段树
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

template <typename T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        iosig |= c == '-';
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig && (x = -x);
}

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == '-') {
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
    (oh == obuf + OUT_LEN) && (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf);
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[21], cnt;
    if (x != 0) {
        (x < 0) && (print('-'), x = -x);
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    } else {
        print('0');
    }
}

struct InputOutputStream {
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

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

const int MAXD = 300000;
const int MAXM = 100000;
const int MAXN = 60000 + 9;
const int MAX_LOG = 17;

struct Node *null;

struct Node {
    static const int NODE_SIZE;
    Node *lc, *rc;
    int cnt;

    Node() : lc(null), rc(null) {}

    inline void *operator new(size_t) {
        static Node *pool, *cur;
        return (cur == pool) &&
                   (pool = (cur = new Node[1 << 20 | 1]) + (1 << 20 | 1)),
               cur++;
    }
};

const int Node::NODE_SIZE = sizeof(Node);

char pool[Node::NODE_SIZE];

Node *d[MAXD + 1];

inline void insert(Node *&p, int l, int r, int v) {
    p = new Node(*p);
    p->cnt++;
    if (l == r) return;
    register int mid = (l + r) >> 1;
    v <= mid ? insert(p->lc, l, mid, v) : insert(p->rc, mid + 1, r, v);
}

inline int query(Node *p, int l, int r, int s, int t) {
    if (p == null) return 0;
    if (s <= l && t >= r) return p->cnt;
    register int mid = (l + r) >> 1;
    return (s <= mid ? query(p->lc, l, mid, s, t) : 0) +
           (t > mid ? query(p->rc, mid + 1, r, s, t) : 0);
}

struct Point {
    int x, y;

    Point() {}
    Point(const int x, const int y) : x(x), y(y) {}

    inline Point rotate() {
        register int nx = x - y + MAXM, ny = x + y + MAXM;
        (nx < 1) ? (nx = 1) : ((nx > MAXD) && (nx = MAXD));
        (ny < 1) ? (ny = 1) : ((ny > MAXD) && (ny = MAXD));
        return Point(nx, ny);
    }
} p[MAXN];

inline void solve() {
    null = (Node *)pool;
    null->lc = null->rc = null;
    for (register int i = 0; i <= MAXD; i++) d[i] = null;
    register int n, m;
    io >> n >> m;
    for (register int i = 1; i <= n; i++) {
        p[i].x = i;
        io >> p[i].y;
        const Point &tmp = p[i].rotate();
        for (register int k = tmp.x; k <= MAXD; k += k & -k)
            insert(d[k], 1, MAXD, tmp.y);
    }
    char cmd[11];
    for (register int x, y; m--;) {
        io >> cmd >> x >> y;
        switch (cmd[0]) {
            case 'Q': {
                const Point &l = Point(p[x].x - y, p[x].y).rotate();
                const Point &r = Point(p[x].x + y, p[x].y).rotate();
                register int ans = 0;
                for (register int k = l.x - 1; k; k ^= k & -k)
                    ans -= query(d[k], 1, MAXD, l.y, r.y);
                for (register int k = r.x; k; k ^= k & -k)
                    ans += query(d[k], 1, MAXD, l.y, r.y);
                io << ans << '\n';
                break;
            }
            case 'M': {
                p[x].y = y;
                const Point &tmp = p[x].rotate();
                for (register int k = tmp.x; k <= MAXD; k += k & -k)
                    insert(d[k], 1, MAXD, tmp.y);
                break;
            }
        }
    }
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}