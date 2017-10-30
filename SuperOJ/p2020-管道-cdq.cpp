/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 2020」管道 30-10-2017
 * cdq - TLE
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

namespace {

const int MAXN = 100000;
const int MAXM = MAXN * 2;
const int MAXQ = MAXM * 2 + 1;
const int MAX_LOG = 19;
const int INF = 0x3f3f3f3f;

struct Edge {
    int u, v, w, id;

    inline bool operator<(const Edge &a) const { return w < a.w; }
} levelEdge[MAX_LOG][MAXM + 1], curEdge[MAXM + 1], tmpEdge[MAXM + 1];

struct Query {
    int k, w;
} query[MAXQ + 1];

bool isInTree[MAXM + 1];
int edgeNum[MAX_LOG], nodeNum[MAX_LOG];

int newV[MAXN + 1], real[MAXM + 1], val[MAXM + 1];

int fa[MAXN + 1], rk[MAXN + 1];

long long ans[MAXQ + 1];

inline int get(register int x) { return x == fa[x] ? x : fa[x] = get(fa[x]); }

inline bool put(int u, int v) {
    if ((u = get(u)) != (v = get(v))) {
        if (rk[u] <= rk[v])
            fa[u] = v, rk[u] == rk[v] ? rk[v]++ : 0;
        else
            fa[v] = u;
        return true;
    }
    return false;
}

inline void clear(const int n) {
    for (register int i = 1; i <= n; i++) fa[i] = i, rk[i] = 0;
}

inline void contraction(int &n, int &m, long long &ret) {
    register int vN = 0, eN = 0;
    clear(n), memset(isInTree, 0, sizeof(bool) * m);
    std::sort(curEdge, curEdge + m);
    for (register int i = 0; i < m; i++) {
        if (put(curEdge[i].u, curEdge[i].v) && curEdge[i].w != -INF)
            ret += curEdge[i].w, isInTree[i] = true;
        else
            tmpEdge[eN++] = curEdge[i];
    }
    clear(n);
    for (register int i = 0; i < m; i++)
        if (isInTree[i]) put(curEdge[i].u, curEdge[i].v);
    for (register int i = 1; i <= n; i++)
        if (get(i) == i) newV[i] = ++vN;
    for (register int i = 1; i <= n; i++) newV[i] = newV[get(i)];
    for (register int i = 0; i < eN; i++) {
        curEdge[i] = tmpEdge[i], real[curEdge[i].id] = i;
        curEdge[i].u = newV[curEdge[i].u], curEdge[i].v = newV[curEdge[i].v];
    }
    n = vN, m = eN;
}

inline void reduction(int &n, int &m) {
    register int eN = 0;
    clear(n), std::sort(curEdge, curEdge + m);
    for (register int i = 0; i < m; i++)
        if (put(curEdge[i].u, curEdge[i].v) || curEdge[i].w == INF)
            real[curEdge[i].id] = eN, curEdge[eN++] = curEdge[i];
    m = eN;
}

void cdq(int l, int r, int cur, long long ret) {
    register int n = nodeNum[cur], m = edgeNum[cur];
    if (l == r) val[query[l].k] = query[l].w;
    for (register int i = 0; i < m; i++) {
        levelEdge[cur][i].w = val[levelEdge[cur][i].id];
        curEdge[i] = levelEdge[cur][i], real[curEdge[i].id] = i;
    }
    if (l == r) {
        if (l & 1) {
            clear(n), std::sort(curEdge, curEdge + m);
            for (register int i = 0; i < m; i++)
                if (put(curEdge[i].u, curEdge[i].v)) ret += curEdge[i].w;
            ans[l] = ret;
        }
        return;
    }
    for (register int i = l; i <= r; i++) curEdge[real[query[i].k]].w = -INF;
    contraction(n, m, ret);
    for (register int i = l; i <= r; i++) curEdge[real[query[i].k]].w = INF;
    reduction(n, m), nodeNum[cur + 1] = n, edgeNum[cur + 1] = m;
    memcpy(levelEdge[cur + 1], curEdge, sizeof(Edge) * m);
    register int mid = (l + r) >> 1;
    cdq(l, mid, cur + 1, ret), cdq(mid + 1, r, cur + 1, ret);
}

using IO::io;

int old[MAXM + 1];

inline void solve() {
    register int n, m;
    io >> n >> m;
    for (register int i = 0; i < m; i++) {
        io >> levelEdge[0][i].u >> levelEdge[0][i].v >> levelEdge[0][i].w;
        old[i] = levelEdge[0][i].w;
        val[i] = levelEdge[0][i].w;
        levelEdge[0][i].id = i;
    }
    for (register int i = 1, t = 1; i <= m; i++, t += 2) {
        query[t].k = i - 1, query[t].w = 0;
        query[t + 1].k = i - 1, query[t + 1].w = old[i - 1];
    }
    nodeNum[0] = n, edgeNum[0] = m, cdq(1, 2 * m, 0, 0);
    for (register int t = 1, i = 1; i <= m; t += 2, i++)
        io << ans[t] + old[i - 1] << '\n';
}
}

int main() {
    // freopen("pipe.in", "r", stdin);
    // freopen("pipe.out", "w", stdout);
    solve();
    return 0;
}