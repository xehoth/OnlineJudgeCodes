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

inline void read(char &c) {
    while (c = read(), isspace(c) && c != -1)
        ;
}

inline void read(double &t) {
    static char c;
    static bool iosig;
    register int x = 0;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    if (c == '.') {
        register long long y = 0, cnt = 1;
        for (c = read(); isdigit(c); c = read())
            y = y * 10 + (c ^ '0'), cnt *= 10;
        t = x + (double)y / cnt;
    } else {
        t = x;
    }
    iosig ? t = -t : 0;
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

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

const double EPS = 1e-9;

inline int sign(const double x) { return (x > EPS) - (x < -EPS); }

inline void print(double x) {
    register int sig = sign(x);
    if (sig == 0) {
        print('0');
        return;
    }
    (sig == -1) && (print('-'), x = -x);
    print((int)x);
    x = x - (int)x;
    print('.');
    static char buf[21], cnt;
    x *= 1000000000;
    register int t = round(x);
    for (cnt = 0; t; t /= 10) buf[++cnt] = t % 10 | 48;
    for (register int i = 9; i > cnt; i--) print('0');
    while (cnt) print((char)buf[cnt--]);
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

const int MAXN = 100000 + 9;
const int MAX_EXP = 11;

struct Node *null;

struct Node {
    double f[MAX_EXP], sum[MAX_EXP];
    Node *c[2], *fa, *top;
    bool rev;

    inline bool relation() { return this == fa->c[1]; }

    inline void reverse() { rev ^= 1, std::swap(c[0], c[1]); }

    inline void pushDown() {
        rev && (c[0]->reverse(), c[1]->reverse(), rev = false);
    }

    inline void maintain() {
#define opt(i) sum[i] = f[i] + c[0]->sum[i] + c[1]->sum[i]
        opt(0);
        opt(1), opt(2), opt(3), opt(4), opt(5);
        opt(6), opt(7), opt(8), opt(9), opt(10);
#undef opt
    }

    inline void rotate(register bool f) {
        register Node *o = fa;
        top = o->top;
        o->pushDown();
        pushDown();
        (fa = o->fa)->c[o->relation()] = this;
        (o->c[f] = c[!f])->fa = o;
        (c[!f] = o)->fa = this;
        o->maintain();
    }

    inline void splay() {
        register bool f;
        for (pushDown(); fa != null;) {
            (f = relation(), fa->fa == null)
                ? rotate(f)
                : (f == fa->relation() ? (fa->rotate(f), rotate(f))
                                       : (rotate(f), rotate(!f)));
        }
        maintain();
    }

    inline void expose() {
        splay();
        if (c[1] != null) {
            c[1]->top = this;
            c[1]->fa = null;
            c[1] = null;
            maintain();
        }
    }

    inline bool splice() {
        splay();
        if (top == null) return false;
        top->expose();
        top->c[1] = this;
        fa = top;
        top = null;
        fa->maintain();
        return true;
    }

    inline void access() {
        for (expose(); splice();)
            ;
    }

    inline void evert() { access(), splay(), reverse(); }
} pool[MAXN];

inline void link(register Node *u, register Node *v) {
    u->evert();
    u->top = v;
}

inline void cut(register Node *u, register Node *v) {
    u->expose();
    v->expose();
    if (u->top == v) u->top = null;
    if (v->top == u) v->top = null;
}

inline void modify(register Node *p, const int cmd, const double a,
                   const double b) {
    memset(p->f, 0, sizeof(p->f));
    p->splay();
    switch (cmd) {
        case 1: {
            // sin(b), cos(b) * a, -sin(b) * a ^ 2
            // -cos(b) * a ^ 3, sin(b) * a ^ 4
            register double expN = a;
            const double SIN_B = sin(b), COS_B = cos(b);
            p->f[0] = SIN_B;
            p->f[1] = COS_B * expN;
            expN *= a;
            p->f[2] = -(SIN_B * expN) / 2;
            expN *= a;
            p->f[3] = -(COS_B * expN) / 6;
            expN *= a;
            p->f[4] = (SIN_B * expN) / 24;

#define opt(i) p->f[i] = p->f[i - 4] * expN / (i - 3) / (i - 2) / (i - 1) / i
            opt(5), opt(6), opt(7), opt(8), opt(9), opt(10);
#undef opt
            break;
        }
        case 2: {
            // a ^ i \cdot e ^ b
            p->f[0] = exp(b);

#define opt(i) p->f[i] = p->f[i - 1] * a / i
            opt(1), opt(2), opt(3), opt(4), opt(5);
            opt(6), opt(7), opt(8), opt(9), opt(10);
#undef opt
            break;
        }
        case 3: {
            p->f[0] = b, p->f[1] = a;
            break;
        }
    }
    p->maintain();
}

inline void query(register Node *u, register Node *v, register double x) {
    register double expN = 1, ans = 0;
    u->evert(), v->access(), v->splay();
    register Node *p = v;
    for (; p->c[0] != null; p = p->c[0]) p->pushDown();
    if (p != u) {
        io << "unreachable\n";
        return;
    }
    for (register int i = 0; i < MAX_EXP; i++) {
        ans += expN * v->sum[i];
        expN *= x;
    }
    io << ans << '\n';
}

inline void solve() {
    null = pool;
    null->c[0] = null->c[1] = null;
    null->fa = null;
    null->top = null;
    register int n, m;
    static char cmd[21];
    io >> n >> m >> cmd;

    for (register int i = 1, type; i <= n; i++) {
        pool[i].c[0] = pool[i].c[1] = null;
        pool[i].fa = null;
        pool[i].top = null;
        register double a, b;
        io >> type >> a >> b;
        modify(pool + i, type, a, b);
    }

    while (m--) {
        io >> cmd;
        switch (cmd[0]) {
            case 'a': {
                register int u, v;
                io >> u >> v;
                link(pool + u + 1, pool + v + 1);
                break;
            }
            case 'd': {
                register int u, v;
                io >> u >> v;
                cut(pool + u + 1, pool + v + 1);
                break;
            }
            case 'm': {
                register int c, f;
                register double a, b;
                io >> c >> f >> a >> b;
                modify(pool + c + 1, f, a, b);
                break;
            }
            case 't': {
                register int u, v;
                register double x;
                io >> u >> v >> x;
                query(pool + u + 1, pool + v + 1, x);
                break;
            }
        }
    }
}
}  // namespace

int main() {
    // freopen("math.in", "r", stdin);
    // freopen("math.out", "w", stdout);
    solve();
    return 0;
}