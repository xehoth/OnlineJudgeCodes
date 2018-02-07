#include <bits/stdc++.h>
#define FAST_IO
#ifdef FAST_IO
const int IN_LEN = 1000010;
inline int nextChar() {
    static char buf[IN_LEN], *h, *t;
    if (h == t) {
        t = (h = buf) + fread(buf, 1, IN_LEN, stdin);
        if (h == t) return -1;
    }
    return *h++;
}
template <class T>
inline bool read(T &x) {
    static bool iosig = 0;
    static char c;
    for (iosig = 0, c = nextChar(); !isdigit(c); c = nextChar()) {
        if (c == -1) return false;
        if (c == '-') iosig = 1;
    }
    for (x = 0; isdigit(c); c = nextChar()) x = (x << 1) + (x << 3) + (c ^ '0');
    if (iosig) x = -x;
    return true;
}
#endif
template <size_t MAXN, size_t MAXM>
struct HLPP {
    struct Edge {
        int u, v, c, f;
        Edge *next, *rev;
        inline Edge *init(int u, int v, int c, int f, Edge *next) {
            return this->u = u, this->v = v, this->c = c, this->f = f,
                   this->next = next, this;
        }
    } edge[MAXM], *cur, *head[MAXN];
    HLPP() : cur(edge) {}
    inline void addEdge(int u, int v, int c) {
        head[u] = (++cur)->init(u, v, c, 0, head[u]),
        head[v] = (++cur)->init(v, u, 0, 0, head[v]), head[u]->rev = head[v],
        head[v]->rev = head[u];
    }
    std::vector<int> q[MAXN];
    int dis[MAXN], exc[MAXN], cnt[MAXN + 1], act[MAXN], pos, n;
    inline void enqueue(int v) {
        if (!act[v] && exc[v] > 0 && dis[v] < n)
            act[v] = true, q[dis[v]].push_back(v), pos = std::max(pos, dis[v]);
    }
    inline void push(Edge *e) {
        register int amt = std::min(exc[e->u], e->c - e->f);
        if (dis[e->u] == dis[e->v] + 1 && amt > 0)
            e->f += amt, e->rev->f -= amt, exc[e->v] += amt, exc[e->u] -= amt,
                enqueue(e->v);
    }
    inline void gap(int k) {
        for (register int v = 0; v < n; v++)
            if (dis[v] >= k)
                cnt[dis[v]]--, dis[v] = std::max(dis[v], n), cnt[dis[v]]++,
                               enqueue(v);
    }
    inline void relabel(int v) {
        cnt[dis[v]]--, dis[v] = n;
        for (Edge *e = head[v]; e; e = e->next)
            if (e->c - e->f > 0) dis[v] = std::min(dis[v], dis[e->v] + 1);
        cnt[dis[v]]++, enqueue(v);
    }
    inline void discharge(int v) {
        for (Edge *e = head[v]; e; e = e->next) {
            if (exc[v] > 0)
                push(e);
            else
                break;
        }
        if (exc[v] > 0) {
            if (cnt[dis[v]] == 1)
                gap(dis[v]);
            else
                relabel(v);
        }
    }
    inline int hlpp(int s, int t) {
        pos = 0;
        for (Edge *e = head[s]; e; e = e->next) exc[s] += e->c;
        cnt[0] = n, enqueue(s), act[t] = true;
        while (pos >= 0) {
            if (!q[pos].empty()) {
                register int v = q[pos].back();
                q[pos].pop_back(), act[v] = false, discharge(v);
            } else {
                pos--;
            }
        }
        return exc[t];
    }
};
const int MAXN = 5005;
int Q[MAXN * 3], N, L[MAXN], R[MAXN], P[MAXN], Ans, tot, sz, rt[MAXN],
    d[MAXN * 20], S, T;
const int INF = 999999999;
struct SegmentTree {
    int lc, rc;
} t[MAXN * 20];
inline int binarySearch(int p) {
    register int l = 1, r = N, mid, ans = 0;
    while (l <= r) {
        mid = l + r >> 1;
        if (Q[mid] >= p) {
            ans = mid;
            r = mid - 1;
        } else
            l = mid + 1;
    }
    return ans;
}
int n, a[MAXN], b[MAXN], w[MAXN];
HLPP<100000, 1000000> hlpp;
inline void Link(int rt, int l, int r, int i) {
    if (L[i] > r || l > R[i]) return;
    if (L[i] <= l && r <= R[i]) {
        hlpp.addEdge(n + i, tot + rt, INF);
        return;
    }
    int mid = (l + r) >> 1;
    if (t[rt].lc) Link(t[rt].lc, l, mid, i);
    if (t[rt].rc) Link(t[rt].rc, mid + 1, r, i);
}
inline void Insert(int &rt, int p, int l, int r, int i) {
    rt = ++sz;
    if (l == r) {
        hlpp.addEdge(tot + rt, i, INF);
        if (p) hlpp.addEdge(tot + rt, tot + p, INF);
        return;
    }
    int mid = (l + r) >> 1;
    if (a[i] <= mid)
        t[rt].rc = t[p].rc, Insert(t[rt].lc, t[p].lc, l, mid, i);
    else
        t[rt].lc = t[p].lc, Insert(t[rt].rc, t[p].rc, mid + 1, r, i);
    if (t[rt].lc) hlpp.addEdge(tot + rt, tot + t[rt].lc, INF);
    if (t[rt].rc) hlpp.addEdge(tot + rt, tot + t[rt].rc, INF);
}
int main() {
    // freopen("in.in", "r", stdin);
    read(n);
    for (register int i = 1; i <= n; ++i) {
        read(a[i]), read(b[i]), read(w[i]);
        read(L[i]);
        read(R[i]);
        read(P[i]);
        Q[++N] = a[i];
        Q[++N] = L[i];
        Q[++N] = R[i];
        Ans += b[i] + w[i];
    }
    std::sort(Q + 1, Q + N + 1), N = std::unique(Q + 1, Q + N + 1) - (Q + 1);
    S = n << 1 | 1, T = S + 1;
    tot = T;
    for (register int i = 1; i <= n; i++) {
        a[i] = binarySearch(a[i]), L[i] = binarySearch(L[i]),
        R[i] = binarySearch(R[i]);
        hlpp.addEdge(S, i, b[i]), hlpp.addEdge(i, T, w[i]),
            hlpp.addEdge(i, i + n, P[i]);
    }
    for (int i = 1; i <= n; ++i) {
        if (i > 1) Link(rt[i - 1], 1, N, i);
        Insert(rt[i], rt[i - 1], 1, N, i);
    }
    tot = tot + sz;
    hlpp.n = tot;
    std::cout << Ans - hlpp.hlpp(S, T);
    return 0;
}
