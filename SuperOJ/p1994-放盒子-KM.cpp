/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 1994」放盒子 19-10-2017
 * KM
 * @author xehoth
 */
#include <bits/stdc++.h>

const int MAXN = 210;
const int MAX_NL = 210;
const int MAX_NR = 210;
const int INF = 0x3f3f3f3f;

template <class T>
inline bool relax(T &a, const T &b) {
    return b > a ? (a = b, true) : false;
}

template <class T>
inline bool tense(T &a, const T &b) {
    return b < a ? (a = b, true) : false;
}

struct KuhnMunkres {
    int map[MAX_NL][MAX_NR], n, labL[MAX_NL], labR[MAX_NR], slackR[MAX_NR];
    int mateL[MAX_NL], mateR[MAX_NR], faR[MAX_NR], qSize, q[MAX_NL];
    bool bookL[MAX_NL], bookR[MAX_NR];

    inline void augment(int v) {
        for (register int u = faR[v]; v > 0;)
            mateR[v] = (u = faR[v]), std::swap(v, mateL[u]);
    }

    inline bool isOnFoundEdge(int v) {
        if (mateR[v]) {
            q[++qSize] = mateR[v], bookR[v] = true, bookL[mateR[v]] = true;
            return false;
        } else {
            augment(v);
            return true;
        }
    }

    inline void match(int sv) {
        memset(bookL, 0, sizeof(bool) * (n + 1));
        memset(bookR, 0, sizeof(bool) * (n + 1));
        memset(slackR, 0x3f, sizeof(int) * (n + 1));
        memset(faR, 0, sizeof(int) * (n + 1));
        bookL[q[qSize = 1] = sv] = true;
        for (;;) {
            for (register int i = 1; i <= qSize; ++i) {
                register int u = q[i];
                for (register int v = 1; v <= n; v++) {
                    register int d = labL[u] + labR[v] - map[u][v];
                    if (bookR[v] || d > slackR[v]) continue;
                    faR[v] = u;
                    if (d > 0)
                        slackR[v] = d;
                    else if (isOnFoundEdge(v))
                        return;
                }
            }
            register int nv = 0, delta = INF;
            for (register int v = 1; v <= n; v++)
                if (!bookR[v] && tense(delta, slackR[v])) nv = v;
            for (register int u = 1; u <= n; u++)
                if (bookL[u]) labL[u] -= delta;
            for (register int v = 1; v <= n; v++) {
                if (bookR[v])
                    labR[v] += delta;
                else
                    slackR[v] -= delta;
            }
            qSize = 0;
            if (isOnFoundEdge(nv)) return;
        }
    }

    inline void addEdge(const int u, const int v, const int w) {
        map[u][v] = w, relax(labL[u], w);
    }

    inline int km(const int nL, const int nR) {
        this->n = std::max(nL, nR);
        for (register int u = 1; u <= nL; u++) match(u);
        register int ret = 0;
        for (register int u = 1; u <= nL; u++) ret += labL[u];
        for (register int v = 1; v <= nR; v++) ret += labR[v];
        return ret;
    }
} km;

struct Node {
    int l, w, s;

    inline bool operator<(const Node &p) const {
        return l < p.l || (l == p.l && w < p.w);
    }

    inline bool operator==(const Node &p) const { return l == p.l && w == p.w; }
} d[MAXN + 1];

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    register int n;
    std::cin >> n;
    register int ans = 0;
    for (register int i = 1; i <= n; i++)
        std::cin >> d[i].l >> d[i].w, d[i].s = d[i].l * d[i].w;
    std::sort(d + 1, d + n + 1), n = std::unique(d + 1, d + n + 1) - d - 1;
    for (register int i = 1; i <= n; i++) ans += d[i].s;
    for (register int i = 1; i <= n; i++)
        for (register int j = i + 1; j <= n; j++)
            if (d[i].l <= d[j].l && d[i].w <= d[j].w) km.addEdge(i, j, d[i].s);
    std::cout << ans - km.km(n, n);
    return 0;
}