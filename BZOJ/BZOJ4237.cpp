#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
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

inline void read(char &c) {
    while (c = read(), isspace(c) && c != -1)
        ;
}

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = 0;
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = 0;
    return s;
}

const int OUT_LEN = 1000000;

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

    ~InputOutputStream() { flush(); }
} io;
}  // namespace IO

namespace {

using IO::io;

const int MAXN = 200000;

#define long long long

struct Point {
    int x, y;

    Point(int x = 0, int y = 0) : x(x), y(y) {}

    inline bool operator<(const Point &p) const { return y > p.y; }
};

inline bool cmp(const Point &a, const int x) { return a.y < x; }

struct Node {
    Node *lc, *rc;
    std::vector<Point> st;

    inline void maintainStack(const Point &p) {
        while (!st.empty() && p.x <= st.back().x) st.pop_back();
        st.push_back(p);
    }

    inline int precursor(const int x) {
        for (register int l = 0, r = st.size(), mid;;) {
            if (l == r) return l;
            mid = l + r >> 1;
            if (st[mid].y > x)
                l = mid + 1;
            else
                r = mid;
        }
    }

    Node();

    inline void *operator new(size_t);
} pool[MAXN * 4 + 4], *null = pool, *cur = pool + 1;

Node::Node() : lc(null), rc(null) {}

inline void *Node::operator new(size_t) { return cur++; }

struct SegmentTree {
    Node *root;
    int n;
    long cnt;

    inline void build(Node *&p, const int l, const int r) {
        p = new Node();
        if (l == r) return;
        register int mid = l + r >> 1;
        build(p->lc, l, mid), build(p->rc, mid + 1, r);
    }

    inline void build(const int n) { this->n = n, build(root, 0, n); }

    inline void insert(Node *cur, const int l, const int r, const Point &p) {
        if (l > p.x || r < p.x) return;
        cur->maintainStack(p);
        if (l >= r) return;
        register int mid = l + r >> 1;
        insert(cur->lc, l, mid, p), insert(cur->rc, mid + 1, r, p);
    }

    inline int query(Node *p, int l, int r, int x) {
        if (r < x) return INT_MAX;
        register int mid = l + r >> 1;
        if (x <= mid + 1) {
            register int ret = query(p->lc, l, mid, x);
            cnt += p->rc->st.size() - p->rc->precursor(ret);
            return !p->rc->st.empty() ? std::min(p->rc->st.back().y, ret) : ret;
        } else {
            return l == r ? INT_MAX : query(p->rc, mid + 1, r, x);
        }
    }

    inline void query(int x) { query(root, 0, n, x); }

    inline void insert(const Point &p) { insert(root, 0, n, p); }
};

struct Task {
    Point p[MAXN + 1];
    int n;

    SegmentTree segmentTree;

    inline void discretization() {
        static int buc[MAXN + 1];
        for (register int i = 0; i < n; i++) buc[i] = p[i].x;
        std::sort(buc, buc + n);
        for (register int i = 0; i < n; i++)
            p[i].x = std::lower_bound(buc, buc + n, p[i].x) - buc;
        for (register int i = 0; i < n; i++) buc[i] = p[i].y;
        std::sort(buc, buc + n);
        for (register int i = 0; i < n; i++)
            p[i].y = std::lower_bound(buc, buc + n, p[i].y) - buc;
    }

    inline void solve() {
        io >> n;
        for (register int i = 0; i < n; i++) io >> p[i].x >> p[i].y;
        discretization();
        std::sort(p, p + n);
        segmentTree.build(n - 1);
        for (register int i = 0; i < n; i++) {
            segmentTree.query(p[i].x + 1);
            segmentTree.insert(p[i]);
        }
        io << segmentTree.cnt;
    }
} task;

#undef long
}  // namespace

int main() {
    // freopen("scarecrows.in", "r", stdin);
    // freopen("scarecrows.out", "w", stdout);
    task.solve();
    return 0;
}
