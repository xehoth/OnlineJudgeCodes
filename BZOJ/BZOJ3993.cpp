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
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}
}  // namespace IO

namespace Maxflow {

const int MAXN = 100000;

const double EPS = 1e-6;

inline bool equals(const double a, const double b) {
    return std::abs(a - b) < EPS;
}

struct Node {
    int v, index;
    double f;

    Node(const int v, const double f, const int index)
        : v(v), f(f), index(index) {}
};

std::vector<Node> edge[MAXN];

inline void addEdge(const int u, const int v, const double f) {
    edge[u].push_back(Node(v, f, edge[v].size()));
    edge[v].push_back(Node(u, 0, edge[u].size() - 1));
}

int h[MAXN], gap[MAXN];

inline double sap(int v, double flow, int s, int t, int n) {
    if (v == t) return flow;
    register double rec = 0;
    static int iter[MAXN];
    for (register int i = iter[v]; i < edge[v].size(); i++) {
        Node *p = &edge[v][i];
        if (p->f > 0 && h[v] == h[p->v] + 1) {
            register double ret =
                sap(p->v, std::min(flow - rec, p->f), s, t, n);
            p->f -= ret, edge[p->v][p->index].f += ret;
            if (equals(rec += ret, flow) || h[v] >= n) return rec;
        }
    }
    if (!(--gap[h[v]])) h[s] = n;
    gap[++h[v]]++, iter[v] = 0;
    return rec;
}

inline double sap(int s, int t, int n) {
    register double ret = 0;
    memset(gap, 0, sizeof(int) * (n + 1));
    memset(h, 0, sizeof(int) * (n + 1));
    gap[0] = n;
    while (h[s] < n) ret += sap(s, DBL_MAX, s, t, n);
    return ret;
}

const int MAXM = 50;

int n, m;

int robot[MAXM], sumRobot, gun[MAXM], minGun = INT_MAX, attack[MAXM][MAXM];

inline bool check(double time) {
    static const int s = 0, t = n + m + 1;

    for (register int i = 1; i <= m; i++) addEdge(s, i, gun[i - 1] * time);
    for (register int i = 1; i <= n; i++) addEdge(i + m, t, robot[i - 1]);

    for (register int i = 1; i <= m; i++) {
        for (register int j = 1; j <= n; j++) {
            if (attack[i - 1][j - 1] == 1) {
                addEdge(i, j + m, DBL_MAX);
            }
        }
    }

    register double flow = sap(s, t, t + 1);
    memset(edge, 0, sizeof(std::vector<Node>) * (t + 1));
    return equals(flow, sumRobot);
}

inline void solve() {
    using namespace IO;
    read(n), read(m);
    for (register int i = 0; i < n; i++) read(robot[i]), sumRobot += robot[i];
    for (register int i = 0; i < m; i++)
        read(gun[i]), minGun = std::min(minGun, gun[i]);

    for (register int i = 0; i < m; i++)
        for (register int j = 0; j < n; j++) read(attack[i][j]);

    register double l = 0, r = (double)sumRobot / minGun;
    while (r - l > EPS) {
        register double mid = (l + r) / 2;
        check(mid) ? r = mid : l = mid;
    }
    std::cout << std::fixed << std::setprecision(6) << r + 4e-7;
}
}  // namespace Maxflow

int main() {
    // freopen("in.in", "r", stdin);
    Maxflow::solve();
    return 0;
}
