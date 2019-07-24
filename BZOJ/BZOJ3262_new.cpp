#include <iostream>
#include <cstdio>
#include <cctype>
#include <algorithm>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 };

    char ibuf[SIZE], obuf[SIZE], *s, *t, *oh;

    InputOutputStream() : s(), t(), oh(obuf) {}
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    char read() {
        if (s == t) t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin);
        return s == t ? -1 : *s++;
    }

    InputOutputStream &operator>>(int &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ 48);
        if (iosig) x = -x;
        return *this;
    }

    void print(char c) {
        if (oh == obuf + SIZE) {
            fwrite(obuf, 1, SIZE, stdout);
            oh = obuf;
        }
        *oh++ = c;
    }

    void print(int x) {
        static int buf[21], cnt;
        if (x != 0) {
            if (x < 0) {
                print('-');
                x = -x;
            }
            for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
            while (cnt) print((char)buf[cnt--]);
        } else {
            print('0');
        }
    }

    template <typename T>
    InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 100000;
const int MAXK = 200000;

struct Flower {
    int a, b, c;

    bool operator<(const Flower &o) const {
        return (a == o.a) ? ((b == o.b) ? (c < o.c) : (b < o.b)) : (a < o.a);
    }
} f[MAXN + 3];

unsigned int gen() {
    static unsigned int x = 495;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

struct Node *null;

struct Node {
    Node *lc, *rc;

    unsigned int rk;
    int val, size;

    Node(int val) : lc(null), rc(null), rk(gen()), val(val), size(1) {}

    void maintain() { size = lc->size + rc->size + 1; }
};

Node *merge(Node *u, Node *v) {
    if (u == null) return v;
    if (v == null) return u;
    if (u->rk < v->rk) {
        u->rc = merge(u->rc, v);
        u->maintain();
        return u;
    } else {
        v->lc = merge(u, v->lc);
        v->maintain();
        return v;
    }
}

void split(Node *p, int v, Node *&l, Node *&r) {
    if (p == null) {
        l = r = null;
        return;
    }
    if (v <= p->val) {
        split(p->lc, v, l, r);
        p->lc = r;
        r = p;
    } else {
        split(p->rc, v, l, r);
        p->rc = l;
        l = p;
    }
    p->maintain();
}

const int MAXM = 20 * MAXN * sizeof(Node);
char memBuf[MAXM];

void *operator new(size_t size) {
    static char *p = memBuf + MAXM;
    return p -= size;
}

struct Treap {
    Node *root;

    void init() { root = null; }

    void insert(int v) {
        Node *l, *r;
        split(root, v, l, r);
        root = merge(l, merge(new Node(v), r));
    }

    int upperRank(int v) {
        int ret = 0;
        for (Node *p = root; p != null;) {
            if (v < p->val) {
                p = p->lc;
            } else {
                ret += p->lc->size + 1;
                p = p->rc;
            }
        }
        return ret;
    }
};

int n, m;

struct BinaryIndexedTree {
    Treap d[MAXK + 3];

    void init() {
        for (int i = 0; i <= m; i++) d[i].init();
    }

    void add(int k, int v) {
        for (; k <= m; k += k & -k) d[k].insert(v);
    }

    int query(int k, int v) {
        int ret = 0;
        for (; k; k ^= k & -k) ret += d[k].upperRank(v);
        return ret;
    }
} bit;

int ans[MAXN + 3], sum[MAXN + 3];

int main() {
    io >> n >> m;
    for (int i = 1; i <= n; i++) io >> f[i].a >> f[i].b >> f[i].c;
    std::sort(f + 1, f + n + 1);

    null = new Node(0);
    null->lc = null->rc = null;
    null->size = 0;

    bit.init();

    for (int i = 1; i <= n; i++) {
        if (f[i].a == f[i + 1].a && f[i].b == f[i + 1].b && f[i].c == f[i + 1].c && i != n)
            sum[i + 1] = sum[i] + 1;
        else
            ans[bit.query(f[i].b, f[i].c)] += sum[i] + 1;
        bit.add(f[i].b, f[i].c);
    }

    for (int i = 0; i < n; i++) io << ans[i] << '\n';

    return 0;
}
