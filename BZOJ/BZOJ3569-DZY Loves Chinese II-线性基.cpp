/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 3569」DZY Loves Chinese II 05-01-2018
 * 线性基
 * @author xehoth
 */
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

const int OUT_LEN = 1 << 18 | 1;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    (oh == obuf + OUT_LEN) && (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf);
    *oh++ = c;
}

inline void print(const char *s) {
    for (; *s; s++) print(*s);
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
const int MAXM = 500000 + 9;
const int MAX_LOG = 32;

typedef unsigned int uint;

inline uint nextUint() {
    static uint x = 495;
    return x ^= x << 13, x ^= x >> 17, x ^= x << 5, x;
}

struct Node {
    int v, id;
    Node(int v, int id) : v(v), id(id) {}
};

struct Edge {
    int u, v;
    uint val;
} edges[MAXM];

std::vector<Node> edge[MAXN];

int n, m, father[MAXN], val[MAXN];
bool vis[MAXN], use[MAXM];
uint a[33];

typedef std::vector<Node>::reverse_iterator Iterator;

inline void addEdge(const int u, const int v, const int id) {
    edge[u].push_back(Node(v, id));
    edge[v].push_back(Node(u, id));
}

inline void dfs(const int u, const int fa) {
    vis[u] = true;
    for (register Iterator p = edge[u].rbegin(); p != edge[u].rend(); ++p) {
        if (p->v != fa && !vis[p->v]) {
            use[p->id] = true;
            father[p->v] = u;
            dfs(p->v, u);
        }
    }
}

inline void dfs(const int u) {
    for (register Iterator p = edge[u].rbegin(); p != edge[u].rend(); ++p) {
        if (father[p->v] != u) continue;
        dfs(p->v);
        edges[p->id].val ^= val[p->v];
        val[u] ^= val[p->v];
    }
}

inline void solve() {
    io >> n >> m;
    for (register int i = 1; i <= m; i++) {
        io >> edges[i].u >> edges[i].v;
        addEdge(edges[i].u, edges[i].v, i);
    }
    dfs(1, 0);
    for (register int i = 1, x; i <= m; i++) {
        if (!use[i]) {
            x = (nextUint() & 1073741823) + 1;
            edges[i].val = x;
            val[edges[i].u] ^= x;
            val[edges[i].v] ^= x;
        }
    }
    dfs(1);
    register int q, ans = 0;
    io >> q;
    for (register int k, x; q--;) {
        register bool flag = true;
        memset(a, 0, sizeof(a));
        io >> k;
        for (register int i = 1; i <= k; i++) {
            io >> x;
            x ^= ans;
            x = edges[x].val;
            for (register uint p; x;) {
                if (a[p = 31 - __builtin_clz(x)]) {
                    x ^= a[p];
                } else {
                    a[p] = x;
                    break;
                }
            }
            if (x == 0) {
                flag = false;
                for (i++; i <= k; i++) io >> x;
                break;
            }
        }
        if (flag) {
            ans++;
            io << "Connected\n";
        } else {
            io << "Disconnected\n";
        }
    }
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}