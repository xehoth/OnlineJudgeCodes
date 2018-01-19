/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 3278」⑨ 17-01-2018
 * 平衡树
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 100000;
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

const int OUT_LEN = 100000;

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

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
    ~InputOutputStream() { flush(); }

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
}  // namespace IO

namespace {

using IO::io;

const int MAXN = 2010;

const double ALPHA = 0.80;

struct Point {
    int x, y;
    int w, id;

    inline int operator*(const Point &p) const { return x * p.y - y * p.x; }

    Point() {}

    Point(const int x, const int y) : x(x), y(y) {}

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline bool operator<(const Point &) const;

    inline int dis(const Point &p) const {
        return (x - p.x) * (x - p.x) + (y - p.y) * (y - p.y);
    }
} pts[MAXN];

const Point O(-20000, -20000);

inline bool Point::operator<(const Point &p) const {
    register int xmul = (*this - O) * (p - O);
    return xmul == 0 ? this->dis(O) < p.dis(O) : xmul > 0;
}

struct Node {
    Node *c[2];
    int sum, size;
    const Point *p;

    inline void *operator new(size_t);

    inline bool check() {
        return std::max(c[0]->size, c[1]->size) > ALPHA * size;
    }

    inline void maintain() {
        sum = c[0]->sum + c[1]->sum + p->w;
        size = c[0]->size + c[1]->size + 1;
    }

    Node(const Point *);
};

const int NODE_SIZE = sizeof(Node);

char pool[MAXN * NODE_SIZE], *cur = pool;

Node *null = (Node *)pool, *root;

inline void *Node::operator new(size_t) { return cur += NODE_SIZE; }

Node::Node(const Point *p) : p(p), size(1) {
    sum = p->w, c[0] = null, c[1] = null;
}

Point *cmpId;

inline bool cmp(const Point *a, const Point *b) {
    return (*a - *cmpId) * (*b - *cmpId) > 0;
}

int nowAns;

inline Node **insert(Node *&p, const Point *pt) {
    if (p == null) return p = new Node(pt), &null;
    p->size++, p->sum += pt->w;
    Node **res;
    if (cmp(p->p, pt))
        res = insert(p->c[0], pt);
    else
        nowAns += p->c[0]->sum + p->p->w, res = insert(p->c[1], pt);
    if (p->check()) res = &p;
    return res;
}

inline void travel(Node *p, std::vector<Node *> &v) {
    if (p == null) return;
    travel(p->c[0], v), v.push_back(p), travel(p->c[1], v);
}

inline Node *divide(std::vector<Node *> &v, int l, int r) {
    if (l >= r) return null;
    register int mid = l + r >> 1;
    Node *p = v[mid];
    p->c[0] = divide(v, l, mid), p->c[1] = divide(v, mid + 1, r);
    p->maintain();
    return p;
}

inline void rebuild(Node *&p) {
    static std::vector<Node *> v;
    v.clear(), travel(p, v), p = divide(v, 0, v.size());
}

inline void insert(const Point *pt) {
    Node **p = insert(root, pt);
    if (*p != null) rebuild(*p);
}

int n, m, q, cnt;

int f[MAXN][MAXN], pos[MAXN];

inline void init() {
    std::sort(pts + 1, pts + cnt + 1);
    for (register int i = 1; i <= cnt; i++) pos[pts[i].id] = i;
    for (register int x = 1; x < cnt; x++) {
        root = null, cur = pool;
        cmpId = pts + x;
        for (register int y = x + 1; y <= cnt; y++) {
            nowAns = 0;
            insert(pts + y);
            f[x][y] = f[y][x] = nowAns;
        }
    }
}

int poly[MAXN];

inline void query() {
    register int e;
    io >> e;
    for (register int i = 1; i <= e; i++) io >> poly[i], poly[i] = pos[poly[i]];
    poly[e + 1] = poly[1];
    register int ans = 0;
    for (register int i = 1; i <= e; i++) {
        if ((pts[poly[i]] - O) * (pts[poly[i + 1]] - O) > 0) {
            ans += f[poly[i]][poly[i + 1]];
        } else {
            ans -= f[poly[i]][poly[i + 1]];
        }
    }
    if (ans < 0) ans = -ans;
    io << ans << '\n';
}

inline void solve() {
    null->c[0] = null->c[1] = null, null->sum = 0, null->size = 0;
    io >> n >> m;
    for (register int i = 1; i <= n; i++)
        io >> pts[i].x >> pts[i].y, pts[i].id = i;
    cnt = n;
    for (register int i = 1; i <= m; i++) {
        cnt++;
        io >> pts[cnt].x >> pts[cnt].y >> pts[cnt].w, pts[cnt].id = cnt;
    }
    init();
    io >> q;
    for (register int i = 1; i <= q; i++) query();
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}