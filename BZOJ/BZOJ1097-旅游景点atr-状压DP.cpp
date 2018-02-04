/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 1097」旅游景点atr 04-02-2018
 * 状压 DP
 * @author xehoth
 */
#include <algorithm>
#include <cstdio>
#include <ext/pb_ds/priority_queue.hpp>
#include <iostream>
#include <vector>

const int POOL_SIZE = 1024 * 1024 * 256;

inline void *operator new(size_t size) {
    static char pool[POOL_SIZE], *s = pool;
    char *t = s;
    s += size;
    return t;
}

inline void operator delete(void *) {}

struct InputOutputStream {
    static const int SIZE = 1 << 18 | 1;
    char ibuf[SIZE], *s, *t;

    inline char read() {
        return (s == t) && (t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin)),
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

    InputOutputStream() : s(), t() {}

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }
} io;

struct Node {
    int v, w;

    Node(int v, int w) : v(v), w(w) {}
};

typedef std::pair<int, int> Pair;
typedef __gnu_pbds::priority_queue<Pair, std::greater<Pair> > PriorityQueue;
typedef std::vector<Node>::iterator Iterator;
const int INF = 0x3f3f3f3f;

std::vector<std::vector<Node> > g;

inline void addEdge(const int u, const int v, const int w) {
    g[u].push_back(Node(v, w));
    g[v].push_back(Node(u, w));
}

std::vector<int> dis, st;
std::vector<std::vector<int> > f, d;
int n, m, K;

inline bool tense(int &x, const int v) { return x > v ? (x = v, true) : false; }

inline const std::vector<int> &dijkstra(const int s) {
    static std::vector<PriorityQueue::point_iterator> it;
    static PriorityQueue q;
    static std::vector<bool> vis;
    q.clear();
    it.clear();
    it.resize(n);
    vis.assign(n, 0);
    dis.assign(n, INF);
    it[s] = q.push(Pair(dis[s] = 0, s));
    for (int u; !q.empty();) {
        u = q.top().second;
        q.pop();
        if (vis[u]) continue;
        vis[u] = true;
        for (Iterator p = g[u].begin(); p != g[u].end(); ++p) {
            if (tense(dis[p->v], dis[u] + p->w)) {
                if (it[p->v] != NULL)
                    q.modify(it[p->v], Pair(dis[p->v], p->v));
                else
                    it[p->v] = q.push(Pair(dis[p->v], p->v));
            }
        }
    }
    return dis;
}

int main() {
#ifdef DBG
    freopen("sample/1.in", "r", stdin);
#endif
    io >> n >> m >> K;
    g.resize(n);
    st.resize(n);
    d.resize(K + 1);
    for (int i = 0, u, v, w; i < m; i++) {
        io >> u >> v >> w;
        addEdge(u - 1, v - 1, w);
    }
    for (int i = 0; i <= K; i++) d[i] = dijkstra(i);
    io >> m;
    for (int i = 0, u, v; i < m; i++) {
        io >> u >> v;
        st[v - 1] |= 1 << (u - 2);
    }
    f.assign(1 << K, std::vector<int>(K + 1, INF));
    f[0][0] = 0;
    const int MAX = 1 << K;
    for (int s = 0; s < MAX; s++) {
        for (int i = 0; i <= K; i++) {
            if (f[s][i] != INF) {
                for (int j = 1; j <= K; j++) {
                    ((s & st[j]) == st[j]) &&
                        tense(f[s | (1 << (j - 1))][j], f[s][i] + d[i][j]);
                }
            }
        }
    }
    int ans = INF;
    for (int i = 0; i <= K; i++) tense(ans, f[MAX - 1][i] + d[i][n - 1]);
    std::cout << ans;
    return 0;
}