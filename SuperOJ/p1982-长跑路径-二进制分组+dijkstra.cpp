/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 1982」长跑路径 15-09-2017
 * 二进制分组 + dijkstra
 * @author xehoth
 */
#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>

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

using IO::io;

const int MAXN = 100000;
const int INF = 0x3f3f3f3f;

int n, m, k;

struct Node {
    int v, w;

    Node(int v, int w) : v(v), w(w) {}
};

std::vector<Node> edge[MAXN + 1];

typedef std::vector<Node>::iterator Iterator;

inline void addEdge(const int u, const int v, const int w) {
    edge[u].push_back(Node(v, w)), edge[v].push_back(Node(u, w));
}

int dis[MAXN + 1], pos[MAXN + 1];
std::vector<int> key;

typedef std::pair<int, int> Pair;
typedef __gnu_pbds::priority_queue<Pair, std::greater<Pair> > PriorityQueue;

PriorityQueue::point_iterator id[MAXN + 1];
PriorityQueue q;

inline void solveCase() {
    io >> n >> m;
    for (register int i = 0; i <= n; i++) edge[i].clear();
    key.clear();
    memset(pos, 0, sizeof(int) * (n + 1));
    for (register int i = 1, u, v, w; i <= m; i++)
        io >> u >> v >> w, addEdge(u, v, w);
    io >> k;
    for (register int i = 1, x; i <= k; i++)
        io >> x, key.push_back(x), pos[x] = i;
    if (k == n) {
        register int min = INT_MAX;
        for (register int i = 1; i <= n; i++)
            for (register Iterator p = edge[i].begin(); p != edge[i].end(); p++)
                min = std::min(min, p->w);
        io << min << '\n';
    } else {
        register int ret = INF;
        for (register int i = 0; (1 << i) <= k; i++) {
            q.clear();
            memset(dis, 0x3f, sizeof(int) * (n + 1));
            memset(id, 0, sizeof(PriorityQueue::point_iterator) * (n + 1));
            for (register int j = 0; j < key.size(); j++)
                if ((1 << i) & pos[key[j]])
                    dis[key[j]] = 0, id[key[j]] = q.push(Pair(0, key[j]));
            while (!q.empty()) {
                register int u = q.top().second, w = q.top().first;
                if (pos[u] && !((1 << i) & pos[u])) {
                    ret = std::min(ret, w);
                    break;
                }
                q.pop();
                for (register Iterator p = edge[u].begin(); p != edge[u].end();
                     p++) {
                    if (p->w + w < dis[p->v]) {
                        dis[p->v] = p->w + w;
                        if (id[p->v] != NULL)
                            q.modify(id[p->v], Pair(dis[p->v], p->v));
                        else
                            id[p->v] = q.push(Pair(dis[p->v], p->v));
                    }
                }
            }
        }
        io << ret << '\n';
    }
}

inline void solve() {
    register int T;
    io >> T;
    while (T--) solveCase();
}
}

int main() {
    solve();
    return 0;
}