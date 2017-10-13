/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 1979」拆墙 13-09-2017
 * 平面图转对偶图 + 最小生成树
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
}

namespace DataAccess {

typedef long double ld;

const int MAXN = 200000 + 10;
const int MAXM = 400000 + 10;

const ld PI = acos((ld)-1);
const ld PI2 = 2 * PI;

int n, m;
}

namespace PlanarGraph {

using namespace DataAccess;
typedef long double ld;

using IO::io;

struct Point {
    ld x, y;

    Point(ld x = 0, ld y = 0) : x(x), y(y) {}

    inline Point operator-(const Point &p) const {
        return Point(x - p.x, y - p.y);
    }

    inline ld operator*(const Point &p) const {
        return (ld)x * p.y - (ld)y * p.x;
    }
} p[MAXN + 1];

struct Edge {
    int u, v, w;
    ld angle;

    Edge(int u = 0, int v = 0, int w = 0) : u(u), v(v), w(w) {
        angle = atan2((ld)(p[v].y - p[u].y), (ld)(p[v].x - p[u].x));
        if (angle < 0) angle += PI2;
    }
} edge[MAXM + 1];

bool vis[MAXM + 1];
int regionCnt, infArea, rank[MAXM + 1], near[MAXM + 1];

std::vector<int> et[MAXN + 1];

void findRegion(int x, int id) {
    if (vis[id]) return;
    register ld area = 0;
    while (!vis[id]) {
        area += p[x] * p[edge[id].v];
        vis[id] = true, near[id] = regionCnt, x = edge[id].v;
        if (!rank[id ^ 1])
            id = et[x].back();
        else
            id = et[x][rank[id ^ 1] - 1];
    }
    if (area < 0) infArea = regionCnt;
    regionCnt++;
}

inline void init() {
    memset(vis, 0, sizeof(vis));
    memset(rank, 0, sizeof(rank));
    memset(near, 0, sizeof(near));
    for (register int i = 0; i < MAXN; i++) et[i].clear();
    regionCnt = 0, infArea = 0;
    memset(p, 0, sizeof(p));
    memset(edge, 0, sizeof(edge));
    for (register int i = 1; i <= n; i++) io >> p[i].x >> p[i].y;
    for (register int i = 0, u, v, w; i < m; i++) {
        io >> u >> v >> w;
        edge[i << 1] = Edge(u, v, w), edge[i << 1 | 1] = Edge(v, u, w);
    }
}

inline void findDualGraph() {
    static std::pair<ld, int> tmp[MAXM + 1];
    memset(tmp, 0, sizeof(tmp));
    for (register int i = 0; i != m << 1; i++)
        tmp[i] = std::make_pair(edge[i].angle, i);
    std::sort(tmp, tmp + (m << 1));
    for (register int i = 0, id; i != m << 1; i++) {
        id = tmp[i].second;
        const Edge &e = edge[id];
        rank[id] = et[e.u].size(), et[e.u].push_back(id);
    }
    for (register int i = 1; i <= n; i++)
        for (register int j = 0; j != et[i].size(); j++)
            findRegion(i, et[i][j]);
}
}

namespace {

using namespace DataAccess;

using IO::io;

struct Edge {
    int u, v, w;

    Edge(int u = 0, int v = 0, int w = 0) : u(u), v(v), w(w) {}

    inline bool operator<(const Edge &p) const { return w < p.w; }
} edge[MAXM + 1];

inline void transPlanarGraphToDualGraph() {
    PlanarGraph::init();
    PlanarGraph::findDualGraph();
    memset(edge, 0, sizeof(edge));
    for (register int i = 0, a; i != m; i++) {
        a = i << 1;
        edge[i].u = PlanarGraph::near[a];
        edge[i].v = PlanarGraph::near[a ^ 1];
        edge[i].w = PlanarGraph::edge[a].w;
    }
}

int fa[MAXN + 1];

int get(int u) { return u == fa[u] ? u : fa[u] = get(fa[u]); }

inline void kruskal() {
    transPlanarGraphToDualGraph();
    for (register int i = 0; i < MAXN; i++) fa[i] = i;
    std::sort(edge, edge + m);
    register int ans1 = 0, ans2 = 0;
    for (register int i = 0, u, v; i != m; i++) {
        u = edge[i].u, v = edge[i].v;
        // if (u == PlanarGraph::infArea || v == PlanarGraph::infArea) continue;
        u = get(u), v = get(v);
        if (u != v) {
            fa[u] = v;
            ans2 += edge[i].w, ans1++;
        }
    }
    io << ans1 << ' ' << ans2 << '\n';
}

inline void solve() {
    register int T;
    io >> T;
    while (T--) {
        io >> n >> m;
        kruskal();
    }
}
}

int main() {
    freopen("wall.in", "r", stdin);
    freopen("wall.out", "w", stdout);
    solve();
    return 0;
}