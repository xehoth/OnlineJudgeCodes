#include <cstdio>
#include <cctype>

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

const int MAXN = 100000;

struct Node *null;

struct Node {
    Node *lc, *rc;
    int size;

    Node() : lc(null), rc(null), size(1) {}
};

const int MAXM = 20 * MAXN * sizeof(Node);

char pool[MAXM];

void *operator new(size_t size) {
    static char *p = pool + MAXM;
    return p -= size;
}

void insert(Node *&p, int l, int r, int v) {
    p = new Node;
    if (l == r) return;
    int mid = (l + r) >> 1;
    v <= mid ? insert(p->lc, l, mid, v) : insert(p->rc, mid + 1, r, v);
}

Node *merge(Node *u, Node *v) {
    if (u == null) return v;
    if (v == null) return u;
    u->lc = merge(u->lc, v->lc);
    u->rc = merge(u->rc, v->rc);
    u->size += v->size;
    return u;
}

int f[MAXN + 1], a[MAXN + 1], n, m;
Node *root[MAXN + 1];

int query(Node *p, int k) {
    int l = 1, r = n;
    for (int mid; l != r;) {
        mid = (l + r) >> 1;
        if (k <= p->lc->size) {
            p = p->lc;
            r = mid;
        } else {
            k -= p->lc->size;
            p = p->rc;
            l = mid + 1;
        }
    }
    return a[l];
}

int get(int x) { return x == f[x] ? x : f[x] = get(f[x]); }

void put(int x, int y) {
    if ((x = get(x)) != (y = get(y))) {
        f[x] = y;
        root[y] = merge(root[x], root[y]);
    }
}

int query(int u, int k) { return root[u = get(u)]->size < k ? -1 : query(root[u], k); }

int main() {
    null = new Node;
    null->lc = null->rc = null;
    null->size = 0;

    read(n), read(m);
    for (int i = 1; i <= n; i++) f[i] = i;
    for (int i = 1, x; i <= n; i++) {
        read(x);
        a[x] = i;
        insert(root[i], 1, n, x);
    }

    for (int i = 0, x, y; i < m; i++) {
        read(x), read(y);
        put(x, y);
    }

    char cmd;
    int q;
    read(q);
    for (int x, y; q--;) {
        cmd = read();
        while (isspace(cmd)) cmd = read();
        read(x), read(y);
        switch (cmd) {
            case 'B':
                put(x, y);
                break;
            case 'Q':
                print(query(x, y)), print('\n');
                break;
        }
    }
    flush();
}
