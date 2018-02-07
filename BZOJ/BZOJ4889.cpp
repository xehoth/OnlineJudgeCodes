/*
 * created by xehoth 04-05-2017
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

const int OUT_LEN = 1000000;
char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
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

}  // namespace IO

namespace SharedData {

const int MAXN = 50000 + 10;
const int MAXM = 50000 + 10;
const int MOD = 1e9 + 7;

int n, m, x, y;
int a[MAXN], v[MAXN];

inline void add(int &x, int v) {
    (x += v) >= MOD ? x -= MOD : (x < 0 ? x += MOD : 0);
}

struct Data {
    int sum, cnt;

    Data(int sum = 0, int cnt = 0) : sum(sum), cnt(cnt) {}

    inline void add(const Data &b) {
        SharedData::add(sum, b.sum), cnt += b.cnt;
    }

    inline void minus(const Data &b) {
        SharedData::add(sum, -b.sum), cnt -= b.cnt;
    }
};

}  // namespace SharedData

namespace SegmentTree {

using namespace SharedData;

struct Node {
    Node *lc, *rc;
    int sum, cnt;

    Node(int sum = 0, int cnt = 0) : sum(sum), cnt(cnt) {}

    inline void *operator new(size_t);
} pool[MAXN * 200], *cur = pool, *root[MAXN];

inline void *Node::operator new(size_t) { return cur++; }

inline void modify(Node *&p, int l, int r, int pos, int cnt, int k) {
    !p ? p = new Node() : 0, add(p->sum, cnt), p->cnt += k;
    if (l == r) return;
    register int mid = l + r >> 1;
    (pos <= mid) ? modify(p->lc, l, mid, pos, cnt, k)
                 : modify(p->rc, mid + 1, r, pos, cnt, k);
}

inline Data query(const Node *p, int l, int r, int ql, int qr) {
    if (p == NULL) return Data();
    if (ql <= l && r <= qr) return Data(p->sum, p->cnt);
    register int mid = l + r >> 1;
    Data ans(0, 0);
    if (ql <= mid) ans.add(query(p->lc, l, mid, ql, qr));
    if (qr > mid) ans.add(query(p->rc, mid + 1, r, ql, qr));
    return ans;
}

}  // namespace SegmentTree

namespace BinaryIndexedTree {

using namespace SharedData;

using namespace SegmentTree;

inline void modify(int i, int pos, int cnt, int k) {
    for (; i <= n; i += i & -i) modify(root[i], 1, n, pos, cnt, k);
}

inline Data query(int l, int r, int ql, int qr) {
    if (l > r || ql > qr) return Data(0, 0);
    Data ans = Data(0, 0);
    for (register int i = r; i; i ^= i & -i)
        ans.add(query(root[i], 1, n, ql, qr));
    for (register int i = l - 1; i; i ^= i & -i)
        ans.minus(query(root[i], 1, n, ql, qr));
    return ans;
}

#define long long long

inline int mul(int x, int y) {
    register int ret;
    __asm__ __volatile__("\tmull %%ebx\n\tdivl %%ecx\n"
                         : "=d"(ret)
                         : "a"(x), "b"(y), "c"(MOD));
    return ret;
}

inline void solve() {
    using namespace IO;
    read(n), read(m);
    for (register int i = 1; i <= n; i++) read(a[i]), read(v[i]);
    register int sum = 0;
    m--;
    read(x), read(y), std::swap(a[x], a[y]), std::swap(v[x], v[y]);
    for (register int i = 1; i <= n; i++) modify(i, a[i], v[i], 1);
    Data ans;
    for (register int i = 1; i <= n; i++) {
        ans = query(1, i - 1, a[i] + 1, n);
        add(sum, ans.sum), add(sum, mul(ans.cnt, v[i]));
    }
    print(sum), print('\n');
    while (m--) {
        read(x), read(y);
        if (x == y) {
            print(sum), print('\n');
            continue;
        }
        (x > y) ? (std::swap(x, y), 0) : 0;
#define opt(l, r, ql, qr, x) \
    ans = query(l, r, ql, qr), add(sum, -ans.sum), add(sum, -mul(ans.cnt, v[x]))

        opt(1, x - 1, a[x] + 1, n, x), opt(x + 1, n, 1, a[x] - 1, x),
            opt(1, y - 1, a[y] + 1, n, y), opt(y + 1, n, 1, a[y] - 1, y);

#undef opt

        (a[x] > a[y]) ? (add(sum, v[x] + v[y]), 0) : 0;
        modify(x, a[x], -v[x], -1), modify(y, a[y], -v[y], -1);
        std::swap(a[x], a[y]), std::swap(v[x], v[y]);
        modify(x, a[x], v[x], 1), modify(y, a[y], v[y], 1);

#define opt(l, r, ql, qr, x) \
    ans = query(l, r, ql, qr), add(sum, ans.sum), add(sum, mul(ans.cnt, v[x]))

        opt(1, x - 1, a[x] + 1, n, x), opt(x + 1, n, 1, a[x] - 1, x),
            opt(1, y - 1, a[y] + 1, n, y), opt(y + 1, n, 1, a[y] - 1, y);

#undef opt

        (a[x] > a[y]) ? (add(sum, -(v[x] + v[y])), 0) : 0;
        print((sum % MOD + MOD) % MOD), print('\n');
    }
}

#undef long
}  // namespace BinaryIndexedTree

int main() {
    // freopen("book.in", "r", stdin);
    // freopen("book.out", "w", stdout);
    BinaryIndexedTree::solve();
    IO::flush();
    return 0;
}