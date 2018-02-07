/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1066」27-04-2017
 *
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

inline void read(char &c) {
    c = read();
    while (isspace(c)) c = read();
}
}  // namespace IO

namespace MaxFlow {

const int MAXN = 1010;

int gap[MAXN], h[MAXN];

struct Node {
    int v, f, index;
    Node(int v, int f, int index) : v(v), f(f), index(index) {}
};

std::vector<Node> edge[MAXN];

inline void addEdge(int u, int v, int f) {
    edge[u].push_back(Node(v, f, edge[v].size()));
    edge[v].push_back(Node(u, 0, edge[u].size() - 1));
}

inline int sap(int v, int flow, int s, int t, int n) {
    static int iter[MAXN];
    if (v == t) return flow;
    register int rec = 0;
    for (register int i = iter[v]; i < edge[v].size(); i++) {
        Node *p = &edge[v][i];
        if (h[v] == h[p->v] + 1) {
            register int ret = sap(p->v, std::min(flow - rec, p->f), s, t, n);
            p->f -= ret, edge[p->v][p->index].f += ret, iter[v] = i;
            if ((rec += ret) == flow) return flow;
        }
    }
    iter[v] = 0;
    if (!(--gap[h[v]])) h[s] = n;
    gap[++h[v]]++;
    return rec;
}

const int INF = INT_MAX >> 1;

inline int sap(int s, int t, int n) {
    register int ret = 0;
    gap[0] = n;
    while (h[s] < n) ret += sap(s, INF, s, t, n);
    return ret;
}

inline double dis(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

inline void solve() {
    using namespace IO;
    register int r, c, d;
    read(r), read(c), read(d);
    register int s = 0, t = r * c * 2 + 1;
    static int a[25][25];
    register int ans = 0;
    for (register int i = 1; i <= r; i++) {
        for (register int j = 1; j <= c; j++) {
            register char ch;
            read(ch);
            a[i][j] = ch - '0';
            if (a[i][j] && (i - d <= 0 || i + d > r || j - d <= 0 || j + d > c))
                addEdge((i - 1) * c + j + t / 2, t, INF);
        }
    }
    for (register int x1 = 1; x1 <= r; x1++) {
        for (register int y1 = 1; y1 <= c; y1++) {
            if (!a[x1][y1]) continue;
            for (register int x2 = 1; x2 <= r; x2++) {
                for (register int y2 = 1; y2 <= c; y2++) {
                    if (x1 == x2 && y1 == y2) continue;
                    if (dis(x1, y1, x2, y2) <= d)
                        addEdge((x1 - 1) * c + y1 + t / 2, (x2 - 1) * c + y2,
                                INF);
                }
            }
        }
    }
    for (register int i = 1; i <= r; i++) {
        for (register int j = 1; j <= c; j++) {
            register char ch;
            read(ch);
            if (ch == 'L') addEdge(s, (i - 1) * c + j, 1), ans++;
        }
    }
    for (register int i = 1; i <= r; i++) {
        for (register int j = 1; j <= c; j++) {
            if (a[i][j])
                addEdge((i - 1) * c + j, (i - 1) * c + j + t / 2, a[i][j]);
        }
    }
    std::cout << ans - sap(s, t, t + 1);
}
}  // namespace MaxFlow

int main() {
    // freopen("in.in", "r", stdin);
    MaxFlow::solve();
    return 0;
}
