/*
 * created by xehoth on 02-05-2017
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
    for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
    iosig ? x = -x : 0;
}
}  // namespace IO

namespace Maxflow {

const int MAXN = 2500;

const int INF = 0x3f3f3f3f;

struct Node {
    int v, f, index;

    Node(int v, int f, int index) : v(v), f(f), index(index) {}
};

std::vector<Node> edge[MAXN];
std::vector<int> list[MAXN], height, count, que, excess;
typedef std::list<int> List;
std::vector<List::iterator> iter;
List dlist[MAXN];
int highest, highestActive;
typedef std::vector<Node>::iterator Iterator;

inline void addEdge(const int u, const int v, const int f) {
    edge[u].push_back(Node(v, f, edge[v].size()));
    edge[v].push_back(Node(u, 0, edge[u].size() - 1));
}

inline void globalRelabel(int n, int t) {
    height.assign(n, n);
    height[t] = 0;
    count.assign(n, 0);
    que.clear();
    que.resize(n + 1);
    int qh = 0, qt = 0;
    for (que[qt++] = t; qh < qt;) {
        int u = que[qh++], h = height[u] + 1;
        for (Iterator p = edge[u].begin(); p != edge[u].end(); ++p) {
            if (height[p->v] == n && edge[p->v][p->index].f > 0) {
                count[height[p->v] = h]++;
                que[qt++] = p->v;
            }
        }
    }
    for (int i = 0; i <= n; i++) {
        list[i].clear();
        dlist[i].clear();
    }
    for (int u = 0; u < n; ++u) {
        if (height[u] < n) {
            iter[u] = dlist[height[u]].insert(dlist[height[u]].begin(), u);
            if (excess[u] > 0) list[height[u]].push_back(u);
        }
    }
    highest = (highestActive = height[que[qt - 1]]);
}

inline void push(int u, Node &e) {
    int v = e.v;
    int df = std::min(excess[u], e.f);
    e.f -= df;
    edge[v][e.index].f += df;
    excess[u] -= df;
    excess[v] += df;
    if (0 < excess[v] && excess[v] <= df) list[height[v]].push_back(v);
}

inline void discharge(int n, int u) {
    static int cur[MAXN];
    for (int &i = cur[u]; i < (int)edge[u].size(); i++) {
        Node *p = &edge[u][i];
        if (p->f > 0 && height[u] == height[p->v] + 1) {
            push(u, *p);
            if (excess[u] == 0) return;
        }
    }
    int h = height[u];
    cur[u] = 0;
    if (count[h] == 1) {
        for (int i = h; i <= highest; i++) {
            for (List::iterator it = dlist[i].begin(); it != dlist[i].end();
                 ++it) {
                count[height[*it]]--;
                height[*it] = n;
            }
            dlist[i].clear();
        }
        highest = h - 1;
    } else {
        int nh = n;
        for (Iterator p = edge[u].begin(); p != edge[u].end(); ++p)
            if (p->f > 0 && height[u] != height[p->v] + 1)
                nh = std::min(nh, height[p->v] + 1);
        count[h]--;
        iter[u] = dlist[h].erase(iter[u]);
        height[u] = nh;
        if (nh == n) return;
        count[nh]++;
        iter[u] = dlist[nh].insert(dlist[nh].begin(), u);
        highest = std::max(highest, highestActive = nh);
        list[nh].push_back(u);
    }
}

inline int hlpp(int s, int t, int n) {
    if (s == t) return 0;
    highestActive = 0;
    highest = 0;
    height.assign(n, 0);
    height[s] = n;
    iter.resize(n);
    for (int i = 0; i < n; i++)
        if (i != s)
            iter[i] = dlist[height[i]].insert(dlist[height[i]].begin(), i);
    count.assign(n, 0);
    count[0] = n - 1;
    excess.assign(n, 0);
    excess[s] = INF;
    excess[t] = -INF;
    for (int i = 0; i < (int)edge[s].size(); i++) push(s, edge[s][i]);
    globalRelabel(n, t);
    for (int u, res = n; highestActive >= 0;) {
        if (list[highestActive].empty()) {
            highestActive--;
            continue;
        }
        u = list[highestActive].back();
        list[highestActive].pop_back();
        discharge(n, u);
        // if (--res == 0) globalRelabel(res = n, t);
    }
    return excess[t] + INF;
}

inline void solve() {
    using namespace IO;
    register int n, m, sum = 0;
    read(n), read(m);

    const int s = 0, t = n + m + 1;
    for (register int i = 1, w, k; i <= n; i++) {
        read(w), read(k), sum += w, addEdge(s, i, w);

        for (register int j = 0, x, c; j < k; j++)
            read(x), read(c), addEdge(i, n + x, c);
    }

    for (register int i = 1, x; i <= m; i++) read(x), addEdge(n + i, t, x);

    std::cout << sum - hlpp(s, t, t + 1);
}
}  // namespace Maxflow

int main() {
    // freopen("in.in", "r", stdin);
    Maxflow::solve();
    return 0;
}
