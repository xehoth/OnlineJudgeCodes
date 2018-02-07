/*
 * created by xehoth on 07-03-2017
 */
#include <bits/stdc++.h>
#ifndef XEHOTH_HEADER
#define XEHOTH_HEADER
namespace xehoth {

namespace io {

template <class T>
inline T parseFloat(char *str) {
    char *s = str;
    if (*s == '0' || *s == '\0') return 0.0;
    register T sum = 0.0;
    register int flag = 1, pow = 0;
    if (*s == '-') flag = -1, s++;
    while (*s != '\0') {
        if (*s == '.') {
            pow = 1, s++;
            continue;
        }
        sum = *s - '0' + sum * 10, pow *= 10, s++;
    }
    return flag * sum / pow;
}

template <size_t size = 1000000>
struct BufferedInputStream {
    char buf[size], *s, *t;

    inline char read() {
        if (s == t) {
            t = (s = buf) + fread(buf, 1, size, stdin);
            if (s == t) return -1;
        }
        return *s++;
    }

    inline void read(char &c) { c = read(); }

    template <class T>
    inline void read(T &x) {
        static bool iosig;
        static char c;
        for (iosig = false, c = read(); !isdigit(c); c = read()) {
            if (c == '-') iosig = true;
            if (c == -1) return;
        }
        for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
        if (iosig) x = -x;
    }

    inline int read(char *buf) {
        register size_t s = 0;
        register char ch;
        while (ch = read(), isspace(ch) && ch != -1)
            ;
        if (ch == EOF) {
            *buf = '\0';
            return -1;
        }
        do
            buf[s++] = ch;
        while (ch = read(), !isspace(ch) && ch != -1);
        buf[s] = '\0';
        return s;
    }

    inline void read(float &x) {
        static char buf[64];
        read(buf);
        x = parseFloat<float>(buf);
    }

    inline void read(double &x) {
        static char buf[128];
        read(buf);
        x = parseFloat<float>(buf);
    }

    template <class T1, class T2>
    inline void read(T1 &a, T2 &b) {
        read(a), read(b);
    }

    template <class T1, class T2, class T3>
    inline void read(T1 &a, T2 &b, T3 &c) {
        read(a), read(b), read(c);
    }

    template <class T1, class T2, class T3, class T4>
    inline void read(T1 &a, T2 &b, T3 &c, T4 &d) {
        read(a), read(b), read(c), read(d);
    }

    inline int nextInt() {
        register int i;
        read(i);
        return i;
    }

    inline long nextLong() {
        register long i;
        read(i);
        return i;
    }

    inline float nextFloat() {
        register float i;
        read(i);
        return i;
    }

    inline double nextDouble() {
        register double i;
        read(i);
        return i;
    }
};

template <size_t size = 1000000>
struct BufferedOutputStream {
    char buf[size], *s;

    inline void print(char c) {
        if (s == buf + size) fwrite(buf, 1, size, stdout), s = buf;
        *s++ = c;
    }

    inline void print(const char *s) {
        char *p = s;
        while (*p != '\0') print(*p++);
    }

    template <class T>
    inline void println(T x) {
        print(x), print('\n');
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

    template <class T1, class T2>
    inline void print(T1 a, T2 b) {
        print(a), print(b);
    }

    template <class T1, class T2, class T3>
    inline void print(T1 a, T2 b, T3 c) {
        print(a), print(b), print(c);
    }

    template <class T1, class T2, class T3, class T4>
    inline void print(T1 a, T2 b, T3 c, T4 d) {
        print(a), print(b), print(c), print(d);
    }

    template <class T1, class T2>
    inline void println(T1 a, T2 b) {
        print(a), println(b);
    }

    template <class T1, class T2, class T3>
    inline void println(T1 a, T2 b, T3 c) {
        print(a), print(b), println(c);
    }

    template <class T1, class T2, class T3, class T4>
    inline void println(T1 a, T2 b, T3 c, T4 d) {
        print(a), print(b), print(c), println(d);
    }

    BufferedOutputStream() : s(buf) {}

    ~BufferedOutputStream() { fwrite(buf, 1, s - buf, stdout); }
};

}  // namespace io

}  // namespace xehoth

typedef unsigned long long ull;
typedef unsigned int uint;
// #define long long long

xehoth::io::BufferedInputStream<> in;
xehoth::io::BufferedOutputStream<> out;

#endif

using namespace std;
#define kd 2
struct point {
    int d[kd];
    point(int x, int y) { d[0] = x, d[1] = y; }
    inline bool operator==(const point &p) {
        return d[0] == p.d[0] && d[1] == p.d[1];
    }
};
struct node {
    node *l, *r;
    point pid, mi, ma;
    long long data, gcd;
    int s;
    node(const point &p, long long d)
        : l(0), r(0), pid(p), mi(p), ma(p), data(d), gcd(d), s(1) {}
    inline void up() {
        s = 1;
        mi = ma = pid;
        gcd = data;
        if (l) {
            s += l->s;
            for (register int i = 0; i < kd; ++i) {
                mi.d[i] = min(mi.d[i], l->mi.d[i]);
                ma.d[i] = max(ma.d[i], l->ma.d[i]);
            }
            gcd = __gcd(l->gcd, gcd);
        }
        if (r) {
            s += r->s;
            for (register int i = 0; i < kd; ++i) {
                mi.d[i] = min(mi.d[i], r->mi.d[i]);
                ma.d[i] = max(ma.d[i], r->ma.d[i]);
            }
            gcd = __gcd(r->gcd, gcd);
        }
    }
} * root;
vector<node *> A;
struct __cmp {
    int sort_id;
    inline bool operator()(const node *x, const node *y) const {
        return operator()(x->pid, y->pid);
    }
    inline bool operator()(const point &x, const point &y) const {
        if (x.d[sort_id] != y.d[sort_id]) return x.d[sort_id] < y.d[sort_id];
        for (int i = 0; i < kd; ++i) {
            if (x.d[i] != y.d[i]) return x.d[i] < y.d[i];
        }
        return 0;
    }
} cmp;
inline node *build(int k, int l, int r) {
    if (l > r) return 0;
    if (k == kd) k = 0;
    register int mid = l + r >> 1;
    cmp.sort_id = k;
    nth_element(A.begin() + l, A.begin() + mid, A.begin() + r + 1, cmp);
    node *ret = A[mid];
    ret->l = build(k + 1, l, mid - 1);
    ret->r = build(k + 1, mid + 1, r);
    ret->up();
    return ret;
}
const double alpha = 0.75, loga = log2(1.0 / alpha);
inline int size(node *o) { return o ? o->s : 0; }
inline bool isbad(node *o) {
    return size(o->l) > alpha * o->s || size(o->r) > alpha * o->s;
}
inline void flatten(node *u, vector<node *>::iterator &it) {
    if (!u) return;
    flatten(u->l, it);
    *it = u;
    flatten(u->r, ++it);
}
inline bool insert(node *&u, int k, const point &x, long long data, int dep) {
    if (!u) {
        u = new node(x, data);
        return dep <= 0;
    }
    if (u->pid == x) {
        u->data = data;
        u->up();
        return 0;
    }
    cmp.sort_id = k;
    if (insert(cmp(x, u->pid) ? u->l : u->r, (k + 1) % kd, x, data, dep - 1)) {
        ++u->s;
        if (!isbad(u)) {
            u->up();
            return 1;
        }
        if ((int)A.size() < u->s) A.resize(u->s);
        vector<node *>::iterator it = A.begin();
        flatten(u, it);
        u = build(k, 0, u->s - 1);
    }
    u->up();
    return 0;
}
inline void insert(const point &x, long long data) {
    insert(root, 0, x, data, std::__lg(size(root)) / loga);
}
inline bool range_include(node *o, const point &L, const point &R) {
    for (register int i = 0; i < kd; ++i) {
        if (L.d[i] > o->ma.d[i] || R.d[i] < o->mi.d[i]) return 0;
    }
    return 1;
}
inline bool range_in_range(node *o, const point &L, const point &R) {
    for (register int i = 0; i < kd; ++i) {
        if (L.d[i] > o->mi.d[i] || o->ma.d[i] > R.d[i]) return 0;
    }
    return 1;
}
inline bool point_in_range(node *o, const point &L, const point &R) {
    for (register int i = 0; i < kd; ++i) {
        if (L.d[i] > o->pid.d[i] || R.d[i] < o->pid.d[i]) return 0;
    }
    return 1;
}
inline long long query(node *o, const point &L, const point &R) {
    if (!o) return 0;
    if (range_in_range(o, L, R)) return o->gcd;
    register long long ans = point_in_range(o, L, R) ? o->data : 0;
    if (o->l && range_include(o->l, L, R)) ans = __gcd(ans, query(o->l, L, R));
    if (o->r && range_include(o->r, L, R)) ans = __gcd(ans, query(o->r, L, R));
    return ans;
}
int n;
int main() {
    in.read(n), in.read(n), in.read(n);
    while (n--) {
        register int c, x1, y1, x2, y2;
        register long long d;
        in.read(c);
        if (c == 1) {
            in.read(x1, y1, d);
            insert(point(x1, y1), d);
        } else {
            in.read(x1, y1, x2, y2);
            out.println(query(root, point(x1, y1), point(x2, y2)));
        }
    }
    return 0;
}