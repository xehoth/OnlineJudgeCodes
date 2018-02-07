/*
 * created by xehoth on 29-04-2017
 */
#include <bits/stdc++.h>

// #define DBG

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
}  // namespace IO

namespace Maxflow {

const int MAXN = 100010;

struct Node {
    int v, f, index;

    Node(const int v, const int f, const int index)
        : v(v), f(f), index(index) {}
};

std::vector<Node> edge[MAXN];

inline void addEdge(const int u, const int v, const int f) {
    // fprintf(stderr, "addEdge(%d, %d, %d)\n", u, v, f);

    edge[u].push_back(Node(v, f, edge[v].size()));
    edge[v].push_back(Node(u, 0, edge[u].size() - 1));
}

int h[MAXN], gap[MAXN];

inline int sap(int v, int flow, int s, int t, int n) {
    if (v == t) return flow;
    register int rec = 0;
    static int iter[MAXN];
    for (register int i = iter[v]; i < edge[v].size(); i++) {
        Node *p = &edge[v][i];
        if (p->f > 0 && h[v] == h[p->v] + 1) {
            register int ret = sap(p->v, std::min(flow - rec, p->f), s, t, n);
            p->f -= ret, edge[p->v][p->index].f += ret, iter[v] = i;
            if ((rec += ret) == flow || h[v] >= n) return rec;
        }
    }
    if (!(--gap[h[v]])) h[s] = n;
    gap[++h[v]]++, iter[v] = 0;
    return rec;
}

const int INF = INT_MAX >> 1;

inline int sap(int s, int t, int n) {
    register int ret = 0;
    gap[0] = n;
    while (h[s] < n) ret += sap(s, INF, s, t, n);
    return ret;
}

// s = 0, t = num + 1
int s, t, n, m, num;
// n 行 m 列

typedef std::pair<int, int> Pair;

typedef std::map<Pair, Pair> Map;

Map map;

std::vector<int> v[MAXN];

// special -> addEdge(i, j, min(w_i, w_j))

const int dir[2][3][2] = {-1, 0, 1, 0, 0, 1, -1, 0, 1, 0, 0, -1};

inline void addSpecialEdge(const int x, const int y, const int d) {
    Map::iterator p1 = map.find(Pair(x, y)), p2 = map.find(Pair(x, y + d));

#ifdef DBG
    fprintf(stderr, "addSpecialEdge: ");
#endif

    addEdge(p1->second.first, p2->second.first,
            std::min(p1->second.second, p2->second.second));
}

inline Map::iterator addINFEdge(const int x, const int y, const int d,
                                bool rev = false) {
    Map::iterator p, o = map.find(Pair(x, y));
    for (register int k = 0; k < 3; k++) {
        if ((p = map.find(Pair(x + dir[d][k][0], y + dir[d][k][1]))) !=
            map.end()) {
#ifdef DBG
            fprintf(stderr, "addINFEdge: ");
#endif
            rev ? addEdge(p->second.first, o->second.first, INF)
                : addEdge(o->second.first, p->second.first, INF);
        }
    }
    return o;
}

inline void build() {
    s = 0, t = num + 1;

    for (register int i = 1; i <= n; i++) {
        for (register int j = 0; j < v[i].size(); j++) {
            register int x = i, y = v[i][j];

            if ((x & 1) && (y & 3) == 1) {  // special 1
                if (j < v[i].size() - 1 && v[i][j + 1] == y + 1)
                    addSpecialEdge(x, y, 1);
            } else if ((~x & 1) && (y & 3) == 0) {  // special 2
                if (j > 0 && v[i][j - 1] == y - 1) addSpecialEdge(x, y, -1);
            } else if ((x & 1) && (y & 3) == 2) {  // no _*__|
                addINFEdge(x, y, 0);
            } else if ((~x & 1) && (y & 3) == 3) {  // no --|*-
                addINFEdge(x, y, 1);
            } else if (((x + y) & 1) && (x & 1)) {  // cost ___|
                Map::iterator o = addINFEdge(x, y, 0);
                addEdge(s, o->second.first, o->second.second);
            } else if ((x & 1) && ((x + y) & 1) == 0) {  // cost --|-
                Map::iterator o = addINFEdge(x, y, 1, true);
                addEdge(o->second.first, t, o->second.second);
            } else if (((x + y) & 1) && (~x & 1)) {
                Map::iterator o = addINFEdge(x, y, 1);
                addEdge(s, o->second.first, o->second.second);
            } else {
                Map::iterator o = addINFEdge(x, y, 0, true);
                addEdge(o->second.first, t, o->second.second);
            }
        }
    }
}

inline void solve() {
    using namespace IO;
    // n 行 m 列
    read(m), read(n), read(num);
    // x -> 行, y -> 列, (x, y) -> (i, w), i -> [1, num]
    for (register int i = 1, x, y, w; i <= num; i++)
        read(y), read(x), read(w), map[Pair(x, y)] = Pair(i, w),
                                               v[x].push_back(y);
    for (register int i = 1; i <= n; i++) std::sort(v[i].begin(), v[i].end());

    build();
    std::cout << sap(s, t, t + 1);
}
}  // namespace Maxflow

int main() {
    // freopen("block.in", "r", stdin);
    // freopen("block.out", "w", stdout);
    Maxflow::solve();
    return 0;
}
