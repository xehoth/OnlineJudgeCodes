/*
 * created by xehoth on 04-03-2017
 */
#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    if (s == t) {
        t = (s = buf) + fread(buf, 1, IN_LEN, stdin);
        if (s == t) return -1;
    }
    return *s++;
}

template <class T>
inline void read(T &x) {
    static bool iosig;
    static char c;
    for (iosig = false, c = read(); !isdigit(c); c = read()) {
        if (c == '-') iosig = true;
        if (c == -1) return;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    if (iosig) x = -x;
}

const int OUT_LEN = 1000000;
char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}

template <class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        if (x < 0) print('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

#define long long long

const int MAXN = 10010;

struct Node {
    int v, f, w, index;
    Node(int v, int f, int w, int index) : v(v), f(f), w(w), index(index) {}
};

std::vector<Node> edge[MAXN];
std::pair<int, int> amt[MAXN];
int tot;

inline void addEdge(int u, int v, int f, int w) {
    edge[u].push_back(Node(v, f, w, edge[v].size()));
    amt[++tot] = std::make_pair(u, edge[u].size() - 1);
    edge[v].push_back(Node(u, 0, -w, edge[u].size() - 1));
}

typedef std::pair<int, long> Pair;

const long INF = 9187201950435737471;

typedef __gnu_pbds::priority_queue<Pair, std::greater<Pair> > PriorityQueue;
int pree[MAXN], prev[MAXN];
PriorityQueue::point_iterator id[MAXN];
inline Pair minCostFlow(int s, int t, int n) {
    Pair ans(0, 0);
    while (true) {
        static bool vis[MAXN];
        static long dis[MAXN];
        static int prev[MAXN], pree[MAXN];
        std::queue<int> q;
        memset(dis, 127, sizeof(long) * (n + 1));
        memset(vis, 0, sizeof(bool) * (n + 1));
        q.push(s), dis[s] = 0;
        while (!q.empty()) {
            register int u = q.front();
            q.pop();
            vis[u] = false;
            for (register int i = 0; i < edge[u].size(); i++) {
                Node *e = &edge[u][i];
                if (e->f && dis[u] + e->w < dis[e->v]) {
                    dis[e->v] = dis[u] + e->w;
                    prev[e->v] = u, pree[e->v] = i;
                    if (!vis[e->v]) q.push(e->v), vis[e->v] = true;
                }
            }
        }
        if (dis[t] == INF) break;
        register int flow = INT_MAX;
        for (register int i = t; i != s; i = prev[i])
            flow = std::min(flow, edge[prev[i]][pree[i]].f);
        ans.first += flow, ans.second += flow * dis[t];
        for (register int i = t; i != s; i = prev[i]) {
            Node *e = &edge[prev[i]][pree[i]];
            e->f -= flow, edge[e->v][e->index].f += flow;
        }
    }
    return ans;
}

int main() {
    // freopen("delight.in", "r", stdin);
    // freopen("delight.out", "w", stdout);
    register int n, k, ms, me, maxe;
    read(n), read(k), read(ms), read(me);
    static int a[MAXN];
    register int S = n + 1, SS = n + 2, T = n + 3;
    maxe = k - ms;
    register int x;
    register long ans = 0;
    for (register int i = 1; i <= n; i++) read(x), a[i] -= x, ans += x;
    for (register int i = 1; i <= n; i++) read(x), a[i] += x;
    for (register int i = 1; i < n; i++) addEdge(i, i + 1, maxe - me, 0);
    addEdge(n, T, maxe - me, 0);
    tot = 0;
    for (register int i = 1; i <= n; i++) {
        if (i + k <= n)
            addEdge(i, i + k, 1, -a[i]);
        else
            addEdge(i, T, 1, -a[i]);
    }
    for (register int i = 1; i <= k; i++) addEdge(SS, i, INT_MAX >> 1, 0);
    addEdge(S, SS, maxe, 0);
    print(ans - minCostFlow(S, T, T + 1).second), print('\n');
    for (register int i = 1; i <= n; i++)
        edge[amt[i].first][amt[i].second].f ? print('S') : print('E');
    flush();
    return 0;
}