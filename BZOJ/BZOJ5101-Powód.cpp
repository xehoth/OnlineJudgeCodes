/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 5101」Powód 06-02-2018
 *
 * @author xehoth
 */
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>

inline void *operator new(size_t size) {
    static const int POOL_SIZE = 1024 * 1024 * 30;
    static char pool[POOL_SIZE], *s = pool;
    char *t = s;
    s += size;
    return t;
}

inline void operator delete(void *) {}

struct InputOutputStream {
    enum { SIZE = 1 << 18 | 1 };
    char ibuf[SIZE], *s, *t;

    InputOutputStream() : s(), t() {}

    inline char read() {
        return (s == t) && (t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin)),
               s == t ? -1 : *s++;
    }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        iosig && (x = -x);
        return *this;
    }
} io;

const int MOD = 1e9 + 7;

struct Node {
    int u, v, w;
    Node(int u, int v, int w) : u(u), v(v), w(w) {}

    inline bool operator<(const Node &p) const { return w < p.w; }
};

std::vector<Node> edge;
std::vector<int> fa, g, h;
int n, m, H;

inline int get(const int x) { return x == fa[x] ? x : fa[x] = get(fa[x]); }

int main() {
    io >> n >> m >> H;
    edge.reserve(n * m * 2);
    for (int i = 1, u = 0, w; i <= n; i++) {
        for (int j = 1; j < m; j++) {
            io >> w;
            edge.push_back(Node(u + j, u + j + 1, w));
        }
        u += m;
    }
    for (int i = 1, u = 0, w; i < n; i++) {
        for (int j = 1; j <= m; j++) {
            io >> w;
            edge.push_back(Node(u + j, u + m + j, w));
        }
        u += m;
    }
    std::sort(edge.begin(), edge.end());
    fa.resize(n * m + 1);
    g.assign(n * m + 1, 1);
    h.resize(n * m + 1);
    for (int i = 1; i <= n * m; i++) fa[i] = i;
    g[0] = 0;
    for (std::vector<Node>::iterator p = edge.begin(); p != edge.end(); ++p) {
        int u = get(p->u), v = get(p->v);
        if (u != v) {
            fa[v] = u;
            g[u] = (unsigned long long)(g[u] + p->w - h[u]) *
                   (g[v] + p->w - h[v]) % MOD;
            h[u] = p->w;
        }
    }
    std::cout << (g[get(1)] + H - h[get(1)]) % MOD;
    return 0;
}