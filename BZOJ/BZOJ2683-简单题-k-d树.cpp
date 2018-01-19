/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 2683」简单题 05-01-2018
 * K-D 树
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

const int MAXN = 500000 + 9;
const int MAXM = 200000 + 9;

struct Point {
    int x, y;

    inline bool operator==(const Point &p) const {
        return x == p.x && y == p.y;
    }
};

inline bool cmpX(const Point &a, const Point &b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}

inline bool cmpY(const Point &a, const Point &b) {
    return a.y < b.y || (a.y == b.y && a.x < b.x);
}

const double ALPHA = 0.75;

char *cur;

struct Node *null;

struct Node {
    static const int NODE_SIZE;
    Point p, min, max;
    int v, sum, size;
    Node *c[2];
    bool (*cmp)(const Point &, const Point &);

    Node(const Point &p, const int v)
        : p(p), min(p), max(p), v(v), sum(v), size(1), cmp(cmpX) {
        c[0] = c[1] = null;
    }

    inline void clear() { min = max = p; }

    inline void maintain() {
        if (c[0] != null) {
            min.x = std::min(min.x, c[0]->min.x);
            min.y = std::min(min.y, c[0]->min.y);
            max.x = std::max(max.x, c[0]->max.x);
            max.y = std::max(max.y, c[0]->max.y);
        }
        if (c[1] != null) {
            min.x = std::min(min.x, c[1]->min.x);
            min.y = std::min(min.y, c[1]->min.y);
            max.x = std::max(max.x, c[1]->max.x);
            max.y = std::max(max.y, c[1]->max.y);
        }
        sum = v + c[0]->sum + c[1]->sum;
        size = c[0]->size + c[1]->size + 1;
    }

    inline void *operator new(size_t) { return cur += NODE_SIZE; }

    inline bool check() {
        return std::max(c[0]->size, c[1]->size) > ALPHA * size;
    }
} * root;

const int Node::NODE_SIZE = sizeof(Node);

char pool[MAXM * Node::NODE_SIZE];

inline bool (*getCmp(bool flag))(const Point &, const Point &) {
    return flag ? cmpY : cmpX;
}

bool (*cmpFunc)(const Point &, const Point &);

inline bool cmp(const Node *a, const Node *b) { return cmpFunc(a->p, b->p); }

template <typename T>
static inline T square(const register T &x) {
    return x * x;
}

inline bool getSplit(const std::vector<Node *> &p, int l, int r) {
    register double vx = 0, vy = 0;
    register double ax = 0, ay = 0;
    for (register int i = l; i <= r; i++) {
        ax += p[i]->p.x;
        ay += p[i]->p.y;
    }
    ax /= r - l + 1;
    ay /= r - l + 1;
    for (register int i = l; i <= r; i++) {
        vx += square(p[i]->p.x - ax);
        vy += square(p[i]->p.y - ay);
    }
    return vx < vy;
}

inline Node *divide(std::vector<Node *> &v, int l, int r) {
    if (l > r) return null;
    register int mid = (l + r) >> 1;
    cmpFunc = getCmp(getSplit(v, l, r));
    std::nth_element(v.begin() + l, v.begin() + mid, v.begin() + r + 1, cmp);
    register Node *p = v[mid];
    p->clear();
    p->cmp = cmpFunc;
    p->c[0] = divide(v, l, mid - 1);
    p->c[1] = divide(v, mid + 1, r);
    p->maintain();
    return p;
}

inline void travel(Node *p, std::vector<Node *> &v) {
    if (p != null) {
        travel(p->c[0], v);
        v.push_back(p);
        travel(p->c[1], v);
    }
}

inline void rebuild(Node *&p) {
    static std::vector<Node *> v;
    v.clear();
    travel(p, v);
    p = divide(v, 0, (int)v.size() - 1);
}

inline Node **insert(Node *&p, const Point &pt, const int v) {
    if (p->p == pt) {
        p->v += v;
        p->sum += v;
        return &null;
    }
    if (p == null) {
        p = new Node(pt, v);
        return &null;
    }
    register Node **res = insert(p->c[p->cmp(p->p, pt)], pt, v);
    p->maintain();
    if (p->check()) res = &p;
    return res;
}

inline void insert(const Point &p, const int v) {
    Node **res = insert(root, p, v);
    if (*res != null) rebuild(*res);
}

inline int query(Node *p, const Point &l, const Point &r) {
    if (p == null || p->min.x > r.x || p->max.x < l.x || p->min.y > r.y ||
        p->max.y < l.y)
        return 0;
    if (p->max.x <= r.x && p->min.x >= l.x && p->max.y <= r.y &&
        p->min.y >= l.y)
        return p->sum;
    register int ret = 0;
    if (p->p.x <= r.x && p->p.x >= l.x && p->p.y <= r.y && p->p.y >= l.y)
        ret += p->v;
    return ret + query(p->c[0], l, r) + query(p->c[1], l, r);
}

inline void solve() {
    null = (Node *)pool;
    null->c[0] = null->c[1] = null;
    null->min.x = null->min.y = INT_MAX;
    null->max.x = null->max.y = INT_MIN;
    null->cmp = cmpX;
    null->p = null->min;
    root = null;
    cur = pool;
    register int n, ans = 0;
    io >> n;
    Point l, r;
    for (register int cmd, v;;) {
        io >> cmd;
        switch (cmd) {
            case 1: {
                io >> l.x >> l.y >> v;
                /*l.x ^= ans;
                l.y ^= ans;*/
                insert(l, v /*^ ans*/);
                break;
            }
            case 2: {
                io >> l.x >> l.y >> r.x >> r.y;
                /*l.x ^= ans;
                l.y ^= ans;
                r.x ^= ans;
                r.y ^= ans;*/
                ans = query(root, l, r);
                io << ans << '\n';
                break;
            }
            case 3: {
                return;
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