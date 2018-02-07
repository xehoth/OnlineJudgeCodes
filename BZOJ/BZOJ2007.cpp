#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>

const int MAXN = 510 * 510;
const int INF = INT_MAX >> 1;

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

struct Node {
    int v, w;

    Node(const int v, const int w) : v(v), w(w) {}

    inline bool operator<(const Node &p) const { return w > p.w; }
};

std::vector<Node> edge[MAXN];

inline void addEdge(const int u, const int v, const int w) {
    edge[u].push_back(Node(v, w));
}

int dis[MAXN];

typedef std::pair<int, int> Pair;

typedef __gnu_pbds::priority_queue<Node> PriorityQueue;

inline int dijkstra(int s, int n) {
    static PriorityQueue q;
    static PriorityQueue::point_iterator id[MAXN];
    memset(dis, 127, sizeof(int) * (n + 1));
    static bool vis[MAXN];
    id[s] = q.push(Node(s, dis[s] = 0));
    while (!q.empty()) {
        register int v = q.top().v;
        q.pop();
        if (vis[v]) continue;
        vis[v] = true;
        for (register int i = 0; i < edge[v].size(); i++) {
            Node *p = &edge[v][i];
            if (dis[v] + p->w < dis[p->v]) {
                dis[p->v] = dis[v] + p->w;
                if (id[p->v] != NULL)
                    q.modify(id[p->v], Node(p->v, dis[p->v]));
                else
                    id[p->v] = q.push(Node(p->v, dis[p->v]));
            }
        }
    }
}

int main() {
    // freopen("in.in", "r", stdin);
    register int n;
    read(n);
    register int s = 0, t = n * n + 1;

    static int num[510][510];
    for (register int i = 1; i <= n; i++)
        num[0][i] = num[i][n + 1] = s, num[i][0] = num[n + 1][i] = t;

    for (register int i = 1; i <= n; i++)
        for (register int j = 1; j <= n; j++) num[i][j] = n * (i - 1) + j;

    for (register int i = 0, x; i <= n; i++)
        for (register int j = 1; j <= n; j++)
            read(x), addEdge(num[i][j], num[i + 1][j], x);

    for (register int i = 1, x; i <= n; i++)
        for (register int j = 0; j <= n; j++)
            read(x), addEdge(num[i][j + 1], num[i][j], x);

    for (register int i = 0, x; i <= n; i++)
        for (register int j = 1; j <= n; j++)
            read(x), addEdge(num[i + 1][j], num[i][j], x);

    for (register int i = 1, x; i <= n; i++)
        for (register int j = 0; j <= n; j++)
            read(x), addEdge(num[i][j], num[i][j + 1], x);

    dijkstra(s, t + 1);
    printf("%d", dis[t]);
    return 0;
}