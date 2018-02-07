/*
 * craeted by xehoth on 27-03-2017
 */
#include <bits/stdc++.h>

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    if (s == t) {
        t = (s = buf) + fread(buf, 1, IN_LEN, stdin);
        if (s == t) return -1;
    }
    return *s++;
}

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

const int OUT_LEN = 10000000;
char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
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

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
#define INF 0x3f3f3f3f
#define low(x) ((x) & (-(x)))
#define LL long long
#define eps 1e-9
#define MOD 998244353
using namespace std;

#define int LL
inline int Max(int a, int b) { return a > b ? a : b; }
inline int Min(int a, int b) { return a < b ? a : b; }
inline int Abs(int a) { return a > 0 ? a : -a; }
inline int Sqr(int a) { return a * a; }
#undef int

#define MAXN 100005

namespace SegmentTree {
struct Node {
    int mul;
} x[MAXN * 4];

inline void Build(int l, int r, int num) {
    x[num].mul = 1;
    if (l == r) return;
    int mid = l + r >> 1;
    Build(l, mid, num << 1);
    Build(mid + 1, r, num << 1 | 1);
}
inline void Mult(int nl, int nr, int l, int r, int num, LL d) {
    if (l == nl && r == nr) {
        x[num].mul = (LL)x[num].mul * d % MOD;
        return;
    }
    int mid = l + r >> 1;
    if (nl > mid)
        Mult(nl, nr, mid + 1, r, num << 1 | 1, d);
    else if (nr <= mid)
        Mult(nl, nr, l, mid, num << 1, d);
    else {
        Mult(mid + 1, nr, mid + 1, r, num << 1 | 1, d);
        Mult(nl, mid, l, mid, num << 1, d);
    }
}
inline LL Query(int l, int r, int num, int p) {
    if (!p) return 1;
    if (l == r) return x[num].mul;
    int mid = l + r >> 1;
    LL ret;
    if (p <= mid)
        ret = Query(l, mid, num << 1, p);
    else
        ret = Query(mid + 1, r, num << 1 | 1, p);
    return ret * x[num].mul % MOD;
}
}  // namespace SegmentTree

struct Pa {
    int x, y;
    Pa() {}
    Pa(int _, int __) : x(_), y(__) {}
    inline Pa operator*(const Pa &b) {
        Pa ret = b;
        ret.x = (LL)ret.x * x % MOD;
        ret.y = (LL)ret.y * y % MOD;
        return ret;
    }
};
const int L = 0, R = 1;
struct Node {
    int son[2];
    Pa val;
} x[MAXN * 300];
int sz, root[MAXN];
struct ChiarmanTree {
    inline void Updata(int num) {
        int ls = x[num].son[L], rs = x[num].son[R];
        x[num].val = x[ls].val * x[rs].val;
    }

    inline void Insert(int l, int r, int fr, int &now, int p, Pa d) {
        if (!now) {
            now = ++sz;
            x[now] = x[fr];
            x[now].val = Pa(1, 1);
        }
        if (l == r) {
            x[now].val = x[now].val * d;
            return;
        }
        int mid = l + r >> 1;
        if (p > mid)
            Insert(mid + 1, r, x[fr].son[R], x[now].son[R], p, d);
        else
            Insert(l, mid, x[fr].son[L], x[now].son[L], p, d);
        Updata(now);
    }
    inline Pa Query(int l, int r, int num, int p) {
        if (l == r) return x[num].val;
        int mid = l + r >> 1;
        if (p > mid)
            return Query(mid + 1, r, x[num].son[R], p);
        else
            return Query(l, mid, x[num].son[L], p) * x[x[num].son[R]].val;
    }
} ct[MAXN];

#define SGT SegmentTree

int n, m;
LL ni[MAXN];

inline LL Fpow(LL a, LL p) {
    LL tmp = a, ret = 1;
    while (p) {
        if (p & 1) ret = ret * tmp % MOD;
        p >>= 1;
        tmp = tmp * tmp % MOD;
    }
    return ret;
}

inline void Pre(int n) {
    ni[1] = 1;
    for (int i = 2; i <= n; i++) ni[i] = (MOD - MOD / i) * ni[MOD % i] % MOD;
}

inline void Insert(int x, int y, int d, int t) {
    for (int i = x; i <= n; i += low(i))
        ct[x].Insert(1, n, root[i], root[i], y, Pa(d, t));
}
inline Pa Query(int x, int y) {
    Pa ret(1, 1);
    for (int i = x; i; i -= low(i)) ret = ret * ct[x].Query(1, n, root[i], y);
    return ret;
}

struct Query {
    int l, r, cat, id;
    LL p1, p2, p3;

    inline LL Calc() {
        LL ret = 0, p10, p20, p30;
        p10 = (1 + p1) * ni[2] % MOD;
        p20 = (1 + p2) * ni[2] % MOD;
        p30 = (1 + p3) * ni[2] % MOD;
        ret = (ret + p10 * p20 % MOD * p30 % MOD) % MOD;
        ret = (MOD + ret + p10 * (1 - p20) % MOD * (1 - p30) % MOD) % MOD;
        ret = (MOD + ret + p20 * (1 - p10) % MOD * (1 - p30) % MOD) % MOD;
        ret = (MOD + ret + p30 * (1 - p20) % MOD * (1 - p10) % MOD) % MOD;
        return ret;
    }
} q[MAXN];

int main() {
    read(n), read(m);
    Pre(n);
    SGT::Build(1, n, 1);
    x[0].val = Pa(1, 1);
    for (int cat, i = 1; i <= m; i++) {
        read(q[i].cat), read(q[i].l), read(q[i].r);
        int len = q[i].r - q[i].l + 1;
        q[i].id = i;
        if (q[i].cat == 1)
            SGT::Mult(q[i].l, q[i].r, 1, n, 1, (MOD + len - 2) * ni[len] % MOD);
        else {
            LL ans1, ans2, p1, p2;
            p1 = SGT::Query(1, n, 1, q[i].l - 1);
            p2 = SGT::Query(1, n, 1, q[i].r);
            q[i].l--;
            q[i].p1 = p1;
            q[i].p2 = p2;
            q[i].p3 = 1;
        }
    }
    for (int cnt = 0, i = 1; i <= m; i++) {
        if (q[i].cat == 1) {
            cnt++;
            int len = q[i].r - q[i].l + 1;
            int mu = Fpow((MOD + len - 2) * ni[len] % MOD, MOD - 2);
            Insert(q[i].l, q[i].r, mu, (MOD + len - 4) % MOD * ni[len] % MOD);
        } else {
            Pa mu = Query(q[i].l, q[i].r);
            q[i].p1 = q[i].p1 * mu.x % MOD;
            q[i].p2 = q[i].p2 * mu.x % MOD;
            q[i].p3 = mu.y;
            print((cnt & 1 && !q[i].l) ? (MOD + 1 - q[i].Calc()) % MOD
                                       : q[i].Calc()),
                print('\n');
        }
    }

    flush();
    return 0;
}