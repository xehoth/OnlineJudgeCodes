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
const int MAXN = 100005;
struct Flower {
    int a, b, c;
    inline friend bool operator<(const Flower &a, const Flower &b) {
        return (a.a == b.a) ? ((a.b == b.b) ? (a.c < b.c) : (a.b < b.b))
                            : (a.a < b.a);
    }
} f[MAXN];
struct Node {
    Node *lc, *rc;
    int s, v, w, rank;
} node[5000005], *cur = node;
Node *root[MAXN << 1];
inline void update(Node *k) {
    k->s = (k->lc ? k->lc->s : 0) + (k->rc ? k->rc->s : 0) + k->w;
}
inline void lRotate(Node *&k) {
    Node *t = k->rc;
    k->rc = t->lc, t->lc = k, update(k), update(t), k = t;
}
inline void rRotate(Node *&k) {
    Node *t = k->lc;
    k->lc = t->rc, t->rc = k, update(k), update(t), k = t;
}
inline void insert(Node *&k, int x) {
    if (!k) {
        k = ++cur, k->rank = rand(), k->v = x, k->s = k->w = 1;
        return;
    }
    k->s++;
    if (x == k->v)
        return (void)k->w++;
    else if (x < k->v) {
        insert(k->lc, x);
        if (k->lc->rank < k->rank) rRotate(k);
    } else {
        insert(k->rc, x);
        if (k->rc->rank < k->rank) lRotate(k);
    }
}
inline int rank(Node *k, int x) {
    if (!k) return 0;
    if (x == k->v)
        return (k->lc ? k->lc->s : 0) + k->w;
    else if (x < k->v)
        return rank(k->lc, x);
    else
        return (k->lc ? k->lc->s : 0) + k->w + rank(k->rc, x);
}
int n, m;
inline int lowbit(int k) { return k & -k; }
inline int query(int k, int x) {
    register int ans = 0;
    for (; k; k -= lowbit(k)) ans += rank(root[k], x);
    return ans;
}
inline void update(int k, int x) {
    for (; k <= m; k += lowbit(k)) insert(root[k], x);
}
int ans[MAXN], sum[MAXN];
int main() {
    // freopen("in.in", "r", stdin);
    srand(233);
    read(n), read(m);
    for (register int i = 1; i <= n; i++)
        read(f[i].a), read(f[i].b), read(f[i].c);
    std::sort(f + 1, f + n + 1);
    for (register int i = 1; i <= n; i++) {
        if (f[i].a == f[i + 1].a && f[i].b == f[i + 1].b &&
            f[i].c == f[i + 1].c && i != n)
            sum[i + 1] += sum[i] + 1;
        else
            ans[query(f[i].b, f[i].c)] += sum[i] + 1;
        update(f[i].b, f[i].c);
    }
    for (register int i = 0; i < n; i++) write(ans[i]), writeChar('\n');
    flush();
    return 0;
}