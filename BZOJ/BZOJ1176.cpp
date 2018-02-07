/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1176」09-01-2017
 *
 * @author xehoth
 */
#include <bits/stdc++.h>
const int IN_LEN = 10000000, OUT_LEN = 10000000;
inline int nextChar() {
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
    for (iosig = 0, c = nextChar(); !isdigit(c); c = nextChar()) {
        if (c == -1) return false;
        if (c == '-') iosig = 1;
    }
    for (x = 0; isdigit(c); c = nextChar()) x = (x << 1) + (x << 3) + (c ^ '0');
    if (iosig) x = -x;
    return true;
}
char obuf[OUT_LEN], *oh = obuf;
inline void writeChar(const char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}
template <class T>
inline void write(T x) {
    static int buf[30], cnt;
    if (!x)
        writeChar(48);
    else {
        if (x < 0) writeChar('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) writeChar(buf[cnt--]);
    }
}
inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
struct Point {
    int pos[2];
    Point(int a, int b) { pos[0] = a, pos[1] = b; }
    inline int &operator[](int x) { return pos[x]; }
    inline bool operator<(const Point &p) const {
        return pos[0] < p.pos[0] && pos[1] < p.pos[1];
    }
    inline bool operator==(const Point &p) const {
        return pos[0] == p.pos[0] && pos[1] == p.pos[1];
    }
    inline bool operator<=(const Point &p) const {
        return pos[0] <= p.pos[0] && pos[1] <= p.pos[1];
    }
};
inline void updateMin(Point &a, Point &b) {
    a[0] = std::min(a[0], b[0]), a[1] = std::min(a[1], b[1]);
}
inline void updateMax(Point &a, Point &b) {
    a[0] = std::max(a[0], b[0]), a[1] = std::max(a[1], b[1]);
}
bool cmp;
struct Node {
    Node *lc, *rc;
    Point p, l, r;
    int v, sum, size;
    Node(const Point &p, int v)
        : lc(0), rc(0), p(p), l(p), r(p), v(v), sum(v), size(1) {}
    inline int querySum(Point &l, Point &r) {
        if (l <= this->l && this->r <= r) return this->sum;
        if (l[0] > this->r[0] || l[1] > this->r[1] || r[0] < this->l[0] ||
            r[1] < this->l[1])
            return 0;
        register int sum = 0;
        if (l <= p && p <= r) sum += v;
        if (lc) sum += lc->querySum(l, r);
        if (rc) sum += rc->querySum(l, r);
        return sum;
    }
    inline void clear() { lc = rc = 0, l = r = p, sum = v, size = 1; }
    inline void *operator new(size_t) {
        static Node *s, *t;
        static size_t size = sizeof(Node) << 12;
        if (s == t) s = (Node *)malloc(size), t = s + (1 << 12);
        return s++;
    }
    inline void update() {
        l = r = p, sum = v, size = 1;
        if (lc)
            updateMin(l, lc->l), updateMax(r, lc->r), sum += lc->sum,
                size += lc->size;
        if (rc)
            updateMin(l, rc->l), updateMax(r, rc->r), sum += rc->sum,
                size += rc->size;
    }
} * pool[200001];
inline bool less(Node *a, Node *b) { return a->p[cmp] < b->p[cmp]; }
int top = 0;
const double A = 0.7, B = log(1.0 / 0.7);
struct KDimensionalTree {
    Node *rt;
    KDimensionalTree() : rt(0) {}
    inline bool insert(Node *&x, Node *y, int h = 0, bool comp = 0) {
        if (!x) {
            x = new Node(*y);
            return h > log(rt->size) / B;
        }
        if (x->p == y->p) return x->v += y->v, x->sum += y->v, 0;
        cmp = comp;
        if (less(y, x)) {
            bool kre = insert(x->lc, y, h + 1, !comp);
            x->update();
            if (kre) return x->lc->size > x->size * A ? reBuild(x, comp), 0 : 1;
            return 0;
        } else {
            bool kre = insert(x->rc, y, h + 1, !comp);
            x->update();
            if (kre) return x->rc->size > x->size * A ? reBuild(x, comp), 0 : 1;
            return 0;
        }
    }
    inline void insert(const Point &p, int v) {
        Node it(p, v);
        insert(rt, &it);
    }
    inline void tour(Node *x) {
        if (!x) return;
        tour(x->lc), pool[++top] = x, tour(x->rc), x->clear();
    }
    inline void reBuild(Node *&x, bool comp, int lc, int rc) {
        if (rc < lc) {
            x = 0;
            return;
        }
        cmp = comp;
        int mid = lc + rc >> 1;
        std::nth_element(pool + lc, pool + mid, pool + rc + 1, less);
        x = pool[mid], reBuild(x->lc, !comp, lc, mid - 1),
        reBuild(x->rc, !comp, mid + 1, rc), x->update();
    }
    inline void reBuild(Node *&x, bool comp) {
        top = 0, tour(x), reBuild(x, comp, 1, top);
    }
    inline int query(int x1, int y1, int x2, int y2) {
        if (!rt) return 0;
        Point l(x1, y1), r(x2, y2);
        return rt->querySum(l, r);
    }
} tr;
int s, n, o, x, y, a, z, w;
int main() {
    read(s), read(n);
    while (read(o)) {
        if (o == 3) break;
        if (o == 1)
            read(x), read(y), read(a), tr.insert(Point(x, y), a);
        else
            read(x), read(y), read(z), read(w),
                write(tr.query(x, y, z, w) + (z - x + 1) * (w - y + 1) * s),
                writeChar('\n');
    }
    flush();
    return 0;
}
