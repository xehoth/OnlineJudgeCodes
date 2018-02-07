#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 100000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline void read(T& x) {
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

inline void print(const char* s) {
    for (; *s; s++) print(*s);
}

inline void print(const double x) {
    static char buf[30];
    sprintf(buf, "%.2f", x);
    print((const char*)buf);
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
    ~InputOutputStream() { flush(); }

    template <typename T>
    inline InputOutputStream& operator>>(T& x) {
        read(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream& operator>>(const T& x) {
        print(x);
        return *this;
    }
} io;
}  // namespace IO

namespace {

using IO::io;

inline double square(const double x) { return x * x; }

struct Point {
    int x, y;

    Point() {}

    Point(const int x, const int y) : x(x), y(y) {}

    inline bool operator<(const Point& p) const {
        return x < p.x || (x == p.x && y < p.y);
    }

    inline bool operator==(const Point& p) { return x == p.x && y == p.y; }

    inline Point operator-(const Point& p) const {
        return Point(x - p.x, y - p.y);
    }

    inline long long operator*(const Point& p) const {
        return (long long)x * p.y - (long long)y * p.x;
    }

    inline void reverse() { x = -x, y = -y; }

    inline void read() { IO::read(x), IO::read(y); }

    inline double dis(const Point& p) const {
        return sqrt(square(x - p.x) + square(y - p.y));
    }
};

inline long long cross(const Point& a, const Point& b) {
    return (long long)a.x * b.y - (long long)a.y * b.x;
}

const int MAXN = 200010;

char pool[MAXN * sizeof(Point) * 3], *cur = pool;

template <typename T>
class Allocator {
   public:
    static const int NODE_SIZE = sizeof(T);
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;

    template <typename Tp>
    struct rebind {
        typedef Allocator<Tp> other;
    };

    Allocator() {}

    Allocator(const Allocator&) {}

    template <typename Tp>
    Allocator(const Allocator<Tp>&) {}

    inline pointer address(reference x) const { return &x; }

    inline const_pointer address(const_reference x) const { return &x; }

    inline pointer allocate(size_type, const void* = 0) {
        return (pointer)(cur += NODE_SIZE);
    }

    inline void deallocate(pointer, size_type) {}

    inline size_type max_size() const { return size_t(-1) / sizeof(T); }

    inline void construct(pointer p, const T& val) { ::new ((void*)p) T(val); }

    inline void destroy(pointer) {}
};

template <typename T>
const int Allocator<T>::NODE_SIZE;

template <typename T>
inline bool operator==(const Allocator<T>&, const Allocator<T>&) {
    return true;
}

template <typename T>
inline bool operator!=(const Allocator<T>&, const Allocator<T>&) {
    return false;
}

double ans;

typedef std::set<Point, std::less<Point>, Allocator<Point> > Set;
Set top;
Set::iterator l, r, it;
Point x;

inline void eraseL(Set& set) {
    while (l != set.begin() && cross(*--(it = l) - *l, x - *l) >= 0)
        ans -= it->dis(*l), set.erase(l), l = it;
}

inline void eraseR(Set& set) {
    while (++(it = r) != set.end() && cross(x - *r, *it - *r) >= 0)
        ans -= r->dis(*it), set.erase(r), r = it;
}

inline void insert() {
    x.reverse();
    if ((r = top.lower_bound(x)) == top.end()) {
        --(l = r);
        eraseL(top), top.insert(top.end(), x), ans += l->dis(x);
        return;
    }
    if (x == *r) return;
    if (r != top.begin()) {
        if (cross(*--(l = r) - x, *r - x) >= 0) return;
        ans -= l->dis(*r), eraseL(top), ans += l->dis(x);
    }
    eraseR(top), ans += x.dis(*r), top.insert(r, x);
}

Point p[MAXN];

struct Query {
    int cmd, x;
} q[MAXN];

double qAns[MAXN];
int cnt;

bool vis[MAXN];

inline void solve() {
    register int n, ix, iy;
    io >> n >> ix >> iy;
    top.insert(Point(0, 0));
    top.insert(Point(-n, 0));
    top.insert(Point(-ix, -iy));
    ans += Point(0, 0).dis(Point(-ix, -iy)) + Point(-ix, -iy).dis(Point(-n, 0));

    io >> n;
    for (register int i = 1; i <= n; i++) p[i].read();
    register int m;
    io >> m;

    for (register int i = 1; i <= m; i++) {
        io >> q[i].cmd;
        if (q[i].cmd == 1) {
            io >> q[i].x;
            vis[q[i].x] = true;
        }
    }
    for (register int i = 1; i <= n; i++) {
        if (!vis[i]) x = p[i], insert();
    }
    for (register int i = m; i >= 1; --i) {
        if (q[i].cmd == 1)
            x = p[q[i].x], insert();
        else
            qAns[++cnt] = ans;
    }
    for (register int i = cnt; i >= 1; --i) IO::print(qAns[i]), IO::print('\n');
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}