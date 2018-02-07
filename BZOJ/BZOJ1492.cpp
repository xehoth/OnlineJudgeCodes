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

inline void read(double &x) {
    static char buf[30];
    IO::read(buf);
    sscanf(buf, "%lf", &x);
}

struct InputOutputStream {
    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

} io;
}  // namespace IO

namespace {

using IO::io;

const double EPS = 1e-8;
const int MAXN = 200000;

template <typename Type>
class AllocBase {
   public:
    typedef unsigned size_type;
    typedef int difference_type;
    typedef Type *pointer;
    typedef const Type *const_pointer;
    typedef Type &reference;
    typedef const Type &const_reference;
    typedef Type value_type;
    template <typename OtherType>
    struct rebind {
        typedef AllocBase<OtherType> other;
    };

    static Type memoryPool[MAXN + 1], *p;
    static Type *bin[MAXN + 1];
    static int cnt;

    AllocBase() {}
    AllocBase(const AllocBase &) {}
    template <typename OtherType>
    AllocBase(const AllocBase<OtherType> &) {}
    ~AllocBase() {}
    inline pointer address(reference x) const { return &x; }
    inline const_pointer address(const_reference x) const { return &x; }
    inline pointer allocate(size_type n, const void * = 0) const {
        return cnt ? bin[cnt--] : p++;
    }
    inline void deallocate(pointer p, size_type n) const { bin[++cnt] = p; }
    inline size_type max_size() const { return size_type(-1) / sizeof(Type); }
    inline void construct(pointer p, const Type &val) const { *p = Type(val); }
    inline void destroy(pointer p) const {}
};

template <typename Type>
Type AllocBase<Type>::memoryPool[MAXN + 1];
template <typename Type>
Type *AllocBase<Type>::p = memoryPool;
template <typename Type>
Type *AllocBase<Type>::bin[MAXN + 1];
template <typename Type>
int AllocBase<Type>::cnt;
template <typename Type>
inline bool operator==(const AllocBase<Type> &, const AllocBase<Type> &) {
    return true;
}
template <typename Type>
inline bool operator!=(const AllocBase<Type> &, const AllocBase<Type> &) {
    return false;
}

inline int cmp(double a, double b) {
    return fabs(a - b) < EPS ? 0 : (a < b ? -1 : 1);
}

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}

    inline bool operator<(const Point &p) const {
        return cmp(x, p.x) == 0 ? cmp(y, p.y) < 0 : cmp(x, p.x) < 0;
    }

    inline double operator*(const Point &p) const { return x * p.y - y * p.x; }

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }
};

struct Line {
    Point s, t;
    Line() {}
    Line(const Point &s, const Point &t) : s(s), t(t) {}

    inline bool operator<(const Line &l) const {
        return cmp((t - s) * (l.t - l.s), 0) < 0;
    }
};

std::set<Point, std::less<Point>, AllocBase<Point> > pointSet;
std::set<Line, std::less<Line>, AllocBase<Line> > lineSet;

struct Task {
    double f[MAXN + 1];
    double a[MAXN + 1], b[MAXN + 1], r[MAXN + 1];

    inline void initBound() {
        pointSet.insert(Point(0, 0)), pointSet.insert(Point(1e10, -1e20));
        lineSet.insert(Line(Point(0, 0), Point(1e10, -1e20)));
    }

    inline void insert(const Point &x) {
        std::set<Point>::iterator r = pointSet.lower_bound(x), l = r, t;
        l--;
        if (cmp((*r - *l) * (x - *l), 0) < 0) return;
        for (lineSet.erase(Line(*l, *r)), pointSet.insert(x);;) {
            t = r++;
            if (r == pointSet.end()) break;
            if (cmp((*r - x) * (*t - x), 0) > 0) break;
            lineSet.erase(Line(*t, *r)), pointSet.erase(t);
        }
        for (;;) {
            if (l == pointSet.begin()) break;
            t = l--;
            if (cmp((*t - x) * (*l - x), 0) > 0) break;
            lineSet.erase(Line(*l, *t)), pointSet.erase(t);
        }
        pointSet.insert(x), l = r = pointSet.find(x);
        lineSet.insert(Line(*--l, x)), lineSet.insert(Line(x, *++r));
    }

    inline void add(int i) {
        register double x = f[i] / (a[i] * r[i] + b[i]), y = r[i] * x;
        insert(Point(x, y));
    }

    inline double max(const double a, const double b) { return a > b ? a : b; }

    inline void solve() {
        register int n, s;
        io >> n >> s;
        for (register int i = 1; i <= n; i++) io >> a[i] >> b[i] >> r[i];
        initBound();
        f[1] = s, add(1);
        for (register int i = 2; i <= n; i++) {
            std::set<Line>::iterator it =
                lineSet.lower_bound(Line(Point(0, 0), Point(a[i], -b[i])));
            f[i] = max(f[i - 1], b[i] * it->s.x + a[i] * it->s.y);
            add(i);
        }
        std::cout << std::fixed << std::setprecision(3) << f[n];
    }
} task;
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    task.solve();
    return 0;
}
