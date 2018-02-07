/**
 * 「BJOI 2017」喷式水战改 08-06-2017
 * @author xehoth
 */
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
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = '\0';
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = '\0';
    return s;
}

inline void read(double &x) {
    static char buf[30];
    read(buf), x = atof(buf);
}

inline void read(char &c) {
    while (c = read(), isspace(c))
        ;
}

template <typename T1, typename T2>
inline void read(T1 &a, T2 &b) {
    read(a), read(b);
}

template <typename T1, typename T2, typename T3>
inline void read(T1 &a, T2 &b, T3 &c) {
    read(a), read(b), read(c);
}

template <typename T1, typename T2, typename T3, typename T4>
inline void read(T1 &a, T2 &b, T3 &c, T4 &d) {
    read(a), read(b), read(c), read(d);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
inline void read(T1 &a, T2 &b, T3 &c, T4 &d, T5 &e) {
    read(a), read(b), read(c), read(d), read(e);
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

template <typename T>
inline void println(T x) {
    print(x), print('\n');
}

template <typename T1, typename T2>
inline void print(T1 a, T2 b) {
    print(a), print(b);
}

template <typename T1, typename T2, typename T3>
inline void print(T1 a, T2 b, T3 c) {
    print(a), print(b), print(c);
}

template <typename T1, typename T2, typename T3, typename T4>
inline void print(T1 a, T2 b, T3 c, T4 d) {
    print(a), print(b), print(c), print(d);
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
inline void print(T1 a, T2 b, T3 c, T4 d, T5 e) {
    print(a), print(b), print(c), print(d), print(e);
}

template <typename T1, typename T2>
inline void println(T1 a, T2 b) {
    print(a), print(b), print('\n');
}

template <typename T1, typename T2, typename T3>
inline void println(T1 a, T2 b, T3 c) {
    print(a), print(b), print(c), print('\n');
}

template <typename T1, typename T2, typename T3, typename T4>
inline void println(T1 a, T2 b, T3 c, T4 d) {
    print(a), print(b), print(c), print(d), print('\n');
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
inline void println(T1 a, T2 b, T3 c, T4 d, T5 e) {
    print(a), print(b), print(c), print(d), print(e), print('\n');
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace IO

namespace ScapeGoatTree {

const int MAXN = 250000;
const double ALPHA = 0.75;

#define long long long

struct Data {
    long x[4][4];
    Data() {}

    Data(long a, long b, long c) : x() {
        x[0][0] = x[3][3] = a, x[1][1] = b, x[2][2] = c;
        x[0][1] = std::max(a, b), x[1][2] = std::max(b, c);
        x[2][3] = std::max(a, c);
        x[0][2] = x[1][3] = x[0][3] = std::max(a, std::max(b, c));
    }

    inline Data operator+(const Data &b) const {
        Data c;
        for (register int i = 0, j, k; i < 4; i++)
            for (j = i, k = i, c.x[i][j] = 0; k <= j; k++)
                c.x[i][j] = std::max(c.x[i][j], x[i][k] + b.x[k][j]);
        for (register int i = 0, j, k; i < 3; i++)
            for (j = i + 1, k = i, c.x[i][j] = 0; k <= j; k++)
                c.x[i][j] = std::max(c.x[i][j], x[i][k] + b.x[k][j]);
        for (register int i = 0, j, k; i < 2; i++)
            for (j = i + 2, k = i, c.x[i][j] = 0; k <= j; k++)
                c.x[i][j] = std::max(c.x[i][j], x[i][k] + b.x[k][j]);
        c.x[0][3] = 0;
        c.x[0][3] = std::max(c.x[0][3], x[0][0] + b.x[0][3]);
        c.x[0][3] = std::max(c.x[0][3], x[0][1] + b.x[1][3]);
        c.x[0][3] = std::max(c.x[0][3], x[0][2] + b.x[2][3]);
        c.x[0][3] = std::max(c.x[0][3], x[0][3] + b.x[3][3]);
        return c;
    }
};

struct Node *null;

struct Node {
    Node *ch[2];
    int len, a, b, c, size;
    long r;
    Data d;

    Node() : size(1), len(0), a(0), b(0), c(0), r(0), d() {
        ch[0] = ch[1] = null;
    }

    Node(const Data &d, int len, int a, int b, int c)
        : size(1), len(len), r(len), a(a), b(b), c(c), d(d) {
        ch[0] = ch[1] = null;
    }

    inline void *operator new(size_t);

    inline void maintain() {
        d = Data((long)a * len, (long)b * len, (long)c * len);
        if (ch[0] != null) d = ch[0]->d + d;
        if (ch[1] != null) d = d + ch[1]->d;
        r = ch[0]->r + ch[1]->r + len;
        size = ch[0]->size + ch[1]->size + 1;
    }

    inline bool check() {
        return std::max(ch[0]->size, ch[1]->size) > size * ALPHA;
    }
} pool[MAXN], *cur = pool, *root;

inline void *Node::operator new(size_t) { return cur++; }

inline Node **insert(Node *&p, int a, int b, int c, long pos, int len) {
    if (p == null) {
        p = new Node(Data((long)a * len, (long)b * len, (long)c * len), len, a,
                     b, c);
        return &null;
    } else {
        Node **res =
            (p->ch[0]->r + p->len > pos)
                ? insert(p->ch[0], a, b, c, pos, len)
                : insert(p->ch[1], a, b, c, pos - p->ch[0]->r - p->len, len);
        p->maintain();
        if (p->check()) res = &p;
        return res;
    }
}

inline void init() { null = new Node(), null->size = 0, root = new Node(); }

inline void travel(Node *p, std::vector<Node *> &v) {
    if (p == null) return;
    travel(p->ch[0], v), v.push_back(p), travel(p->ch[1], v);
}

inline Node *divide(std::vector<Node *> &v, int l, int r) {
    if (l >= r) return null;
    register int mid = l + r >> 1;
    Node *p = v[mid];
    p->ch[0] = divide(v, l, mid), p->ch[1] = divide(v, mid + 1, r);
    p->maintain();
    return p;
}

inline void rebuild(Node *&p) {
    static std::vector<Node *> v;
    v.clear(), travel(p, v), p = divide(v, 0, v.size());
}

inline Node *select(long pos, long &sum) {
    register long l = 0, r = 0;
    for (Node *p = root; p != null;) {
        l = p->ch[0]->r, r = l + p->len;
        if ((l < pos || (p->ch[0] == null && pos == 0)) && r >= pos)
            return p;
        else if (l >= pos)
            p = p->ch[0];
        else
            sum += r, p = p->ch[1], pos -= r;
    }
}

inline void modify(Node *p, Node *o, long pos, int len) {
    if (p == o) {
        p->len = len, p->maintain();
        return;
    } else {
        p->ch[0]->r >= pos
            ? modify(p->ch[0], o, pos, len)
            : modify(p->ch[1], o, pos - p->len - p->ch[0]->r, len);
        p->maintain();
    }
}

inline void insert(int a, int b, int c, long pos, int len) {
    Node **p = insert(root, a, b, c, pos, len);
    if (*p != null) rebuild(*p);
}

inline void solve() {
    using namespace IO;
    init();
    register int n;
    read(n);
    register long sum = 0, p, l, pre = 0;
    Node **re;
    for (register int i = 1, a, b, c, x; i <= n; i++) {
        read(p, a, b, c, x), sum = 0;
        Node *o = select(p, sum);
        register int k = o - pool;
        if ((l = sum + o->ch[0]->r + o->len - p) != 0) {
            modify(root, o, p, o->len - l);
            re = insert(root, a, b, c, p, x);
            if (*re != null) rebuild(*re);
            re = insert(root, o->a, o->b, o->c, p + x, l);
        } else {
            re = insert(root, a, b, c, p, x);
        }
        println(root->d.x[0][3] - pre), pre = root->d.x[0][3];
        if (*re != null) rebuild(*re);
    }
}

#undef long
}  // namespace ScapeGoatTree

int main() {
#ifndef ONLINE_JUDGE
#ifdef DBG
    freopen("in.in", "r", stdin);
    freopen("debug.log", "w", stderr);
#else
    freopen("amane.in", "r", stdin);
    freopen("amane.out", "w", stdout);
#endif
#endif
    ScapeGoatTree::solve();
    IO::flush();
    return 0;
}