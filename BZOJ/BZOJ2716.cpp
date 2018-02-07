/*
 * created by xehoth on 09-01-2017
 */
#include <bits/stdc++.h>

const int MAXN = 500010;
const int IN_LEN = 10000000;
const int OUT_LEN = 10000000;
const int INF = 0x3f3f3f3f;

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
    int x, y;

    Point() {}

    Point(int x, int y) : x(x), y(y) {}

    inline void read() {
        ::read(x);
        ::read(y);
    }
};

struct KDimensionalTree {
    int x0, y0;
    int x1, y1;
    KDimensionalTree *son[2];
    Point root;

    inline void init(const Point &root, KDimensionalTree *p1,
                     KDimensionalTree *p2) {
        this->root = root;
        this->son[0] = p1;
        this->son[1] = p2;
        this->x0 = this->x1 = root.x;
        this->y0 = this->y1 = root.y;
    }

    inline void *operator new(size_t, const Point &p, KDimensionalTree *p1,
                              KDimensionalTree *p2) {
        static KDimensionalTree memoryPool[MAXN << 1], *s = memoryPool;
        s->init(p, p1, p2);
        return s++;
    }

    inline int dis(const Point &p) const {
        register int ret = 0;
        if (p.x < x0) ret += x0 - p.x;
        if (p.x > x1) ret += p.x - x1;
        if (p.y < y0) ret += y0 - p.y;
        if (p.y > y1) ret += p.y - y1;
        return ret;
    }

    inline void maintain(KDimensionalTree *a) {
        x0 = std::min(x0, a->x0);
        x1 = std::max(x1, a->x1);
        y0 = std::min(y0, a->y0);
        y1 = std::max(y1, a->y1);
    }
};

bool flag;
int ans;
Point point[MAXN << 1];
KDimensionalTree none, *nil = &none, *root;

inline bool cmp(const Point &p1, const Point &p2) {
    if (flag) return p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y);
    return p1.y < p2.y || (p1.y == p2.y && p1.x < p2.x);
}

inline int dis(const Point &p1, const Point &p2) {
    return std::abs(p1.x - p2.x) + std::abs(p1.y - p2.y);
}

inline KDimensionalTree *build(const int l, const int r, bool d) {
    if (l > r) return nil;
    flag = d;
    register int mid = l + r >> 1;
    std::nth_element(point + l, point + mid, point + r + 1, cmp);
    KDimensionalTree *ret = new (point[mid], build(l, mid - 1, !d),
                                 build(mid + 1, r, !d)) KDimensionalTree;
    if (ret->son[0] != nil) ret->maintain(ret->son[0]);
    if (ret->son[1] != nil) ret->maintain(ret->son[1]);
    return ret;
}

inline void query(KDimensionalTree *a, const Point &p) {
    register int tmp = dis(a->root, p);
    ans = std::min(ans, tmp);
    register int l = a->son[0] == nil ? INF : a->son[0]->dis(p);
    register int r = a->son[1] == nil ? INF : a->son[1]->dis(p);
    if (l < r) {
        if (a->son[0] != nil) query(a->son[0], p);
        if (r < ans && a->son[1] != nil) query(a->son[1], p);
    } else {
        if (a->son[1] != nil) query(a->son[1], p);
        if (l < ans && a->son[0] != nil) query(a->son[0], p);
    }
}

inline void insert(const Point &p) {
    KDimensionalTree *now = root;
    KDimensionalTree *_new = new (p, nil, nil) KDimensionalTree;
    flag = false;
    while (true) {
        now->maintain(_new);
        if (cmp(p, now->root)) {
            if (now->son[0] == nil) {
                now->son[0] = _new;
                break;
            } else {
                now = now->son[0];
            }
        } else {
            if (now->son[1] == nil) {
                now->son[1] = _new;
                break;
            } else {
                now = now->son[1];
            }
        }
        flag ^= 1;
    }
}

int main() {
    int n, q;
    read(n), read(q);
    for (int i = 1; i <= n; i++) point[i].read();
    root = build(1, n, 0);
    for (int i = 1, x, y, cmd; i <= q; i++) {
        read(cmd), read(x), read(y);
        if (cmd == 1) {
            insert(Point(x, y));
        } else {
            ans = INF;
            query(root, Point(x, y));
            write(ans), writeChar('\n');
        }
    }
    flush();
    return 0;
}