#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
inline bool read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return false;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
    return true;
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

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

struct InputOutputStream {
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

    ~InputOutputStream() { flush(); }
} io;
}  // namespace IO

namespace {
#define A 0.73

const int MAXN = 120000;

int n, m, L, w;
int s[MAXN], dep[MAXN];

struct Node {
    int fa, son[2], siz, c[2], x;
} d[MAXN << 2];
int root;
int ti[MAXN];

inline void maintain(const int &x) {
    int &lc = d[x].son[0], &rc = d[x].son[1];
    d[x].c[0] = d[lc].c[0] ^ d[rc].c[0];
    d[x].c[1] = d[lc].c[1] ^ d[rc].c[1];
    if (x & 1) d[x].c[dep[d[x].x] & 1] ^= s[d[x].x];
}
int id[MAXN], cnt;
inline void dfs(const int &x) {
    if (d[x].son[0]) dfs(d[x].son[0]), d[x].son[0] = 0;
    id[++cnt] = x;
    if (d[x].son[1]) dfs(d[x].son[1]), d[x].son[1] = 0;
}
inline int build(const int l, const int r, const int father) {
    int mid = l + r >> 1, x = id[mid];
    d[x].siz = r - l + 1;
    d[x].fa = father;
    if (l != mid) d[x].son[0] = build(l, mid - 1, x);
    if (mid != r) d[x].son[1] = build(mid + 1, r, x);
    maintain(x);
    return x;
}
int rebuild(int x) {
    cnt = 0;
    dfs(x);
    int father = d[x].fa, t = d[father].son[1] == x;
    x = build(1, cnt, father);
    d[father].son[t] = x;
    return x;
}
int bad;
inline void insert(const int u, const int v) {
    register int x = (u << 1) - 1, y = (v << 1) - 1, y2 = y + 1, t = dep[v] & 1;

    if (!d[x].son[1])
        d[x].son[1] = y;
    else {
        x = d[x].son[1];
        while (d[x].son[0]) x = d[x].son[0];
        d[x].son[0] = y;
    }
    d[y].fa = x, d[y].son[1] = y2, d[y].siz = 2, d[y].x = v;
    d[y].c[t] = s[v], d[y2].fa = y, d[y2].siz = 1, d[y2].x = v;

    for (register int i = x; i; i = d[i].fa) {
        d[i].c[t] ^= s[v], d[i].siz += 2;
        if (d[d[i].son[0]].siz > A * d[i].siz ||
            d[d[i].son[1]].siz > A * d[i].siz)
            bad = i;
    }
}
inline void mark(int x) {
    while (x) ti[x] = m + 1, x = d[x].fa;
}
inline int query(const int u) {
    register int x = 2 * u - 1, y = u << 1, t = dep[u] & 1 ^ 1, ans = 0;
    mark(y);
    int mul = x;
    if (ti[x] != m + 1) {
        ans ^= d[d[x].son[1]].c[t];
        for (int la = x, i = d[x].fa; i; la = i, i = d[i].fa) {
            if (ti[i] == m + 1) {
                if ((i & 1) && (dep[d[i].x] & 1) == t) ans ^= s[d[i].x];
                mul = i;
                break;
            }
            if (la == d[i].son[0]) {
                if ((i & 1) && (dep[d[i].x] & 1) == t) ans ^= s[d[i].x];
                ans ^= d[d[i].son[1]].c[t];
            }
        }
    }
    if (mul != y) {
        ans ^= d[d[y].son[0]].c[t];
        for (int la = y, i = d[y].fa; i != mul; la = i, i = d[i].fa) {
            if (la == d[i].son[1]) {
                if ((i & 1) && (dep[d[i].x] & 1) == t) ans ^= s[d[i].x];
                ans ^= d[d[i].son[0]].c[t];
            }
        }
    }

    return ans;
}
inline void modify(const int u, const int c) {
    int x = (u << 1) - 1;
    int t = dep[u] & 1;
    s[u] ^= c;
    for (register int i = x; i; i = d[i].fa) d[i].c[t] ^= s[u];
    s[u] = c;
}

struct Edge {
    int y, next;
    Edge() {}
    Edge(const int y, const int next) : y(y), next(next) {}
} a[MAXN << 1];
int len, head[MAXN];
inline void addEdge(const int x, const int y) {
    a[++len] = Edge(y, head[x]);
    head[x] = len;
}

inline void dfs(const int x, const int father) {
    d[id[++cnt] = x * 2 - 1].x = x;
    for (register int k = head[x]; k; k = a[k].next)
        if (a[k].y != father) dep[a[k].y] = dep[x] + 1, dfs(a[k].y, x);
    d[id[++cnt] = x << 1].x = x;
}

using IO::io;

inline void solve() {
    io >> n >> L;
    for (register int i = 1; i <= n; i++) io >> s[i], s[i] %= (L + 1);
    for (register int i = 1, x, y; i < n; i++)
        io >> x >> y, addEdge(x, y), addEdge(y, x);
    cnt = 0, dep[1] = 1, dfs(1, 0);
    root = build(1, cnt, 0);
    io >> m;
    register int dec = 0;
    while (m--) {
        register int k, x, y, u, v;
        io >> k;
        if (k == 1) {
            io >> x, x ^= dec;
            if (query(x))
                dec++, io << "MeiZ\n";
            else
                io << "GTY\n";
        } else if (k == 2) {
            io >> x >> y, x ^= dec, y ^= dec, y %= (L + 1);
            modify(x, y);
        } else {
            io >> u >> v >> x, u ^= dec, v ^= dec, x ^= dec, x %= (L + 1);
            s[v] = x, dep[v] = dep[u] + 1, bad = 0, insert(u, v);
            if (!bad) continue;
            if (bad == root)
                root = rebuild(root);
            else
                rebuild(bad);
        }
    }
}
}

int main() {
    solve();
    return 0;
}