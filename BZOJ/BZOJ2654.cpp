#include <bits/stdc++.h>

using namespace std;

const int iol = 1024 * 1024;

char buf[iol], *s1, *t1, ioc;
bool iosig;

inline char read() {
    if (s1 == t1) {
        t1 = (s1 = buf) + fread(buf, 1, iol, stdin);
        if (s1 == t1) return -1;
    }
    return *s1++;
}

template <class T>
inline bool read(T &x) {
    iosig = false;
    for (ioc = read(); !isdigit(ioc); ioc = read()) {
        if (ioc == -1) return false;
        if (ioc == '-') iosig = true;
    }
    x = 0;
    while (ioc == '0') ioc = read();
    for (; isdigit(ioc); ioc = read()) x = (x << 1) + (x << 3) + (ioc ^ '0');
    s1--;
    if (iosig) x = -x;
    return true;
}

const int MAXN = 50010;
const int MAXM = 100010;

struct DisjointSet {
    int *rank, *father, size;
    inline int get(int x) {
        register int p = x, i;
        while (p != father[p]) p = father[p];
        while (x != p) i = father[x], father[x] = p, x = i;
        return p;
    }

    inline void put(int x, int y) {
        x = get(x), y = get(y);
        if (rank[x] > rank[y])
            father[y] = x;
        else {
            father[x] = y;
            if (rank[x] == rank[y]) rank[y]++;
        }
    }

    inline void init(int x) { rank[x] = 0, father[x] = x; }

    inline void clear(int n) {
        for (register int i = 1; i <= n; i++) father[i] = i;
    }

    DisjointSet(int n)
        : rank(new int[n + 1]), father(new int[n + 1]), size(n + 1) {
        for (register int i = 1; i <= n; i++) father[i] = i;
    }
} * s;

int n, m, k, ans;

struct Node {
    int u, v, w, c;
    Node(int u, int v, int w, int c) : u(u), v(v), w(w), c(c) {}
    inline bool operator<(const Node &b) const {
        return w == b.w ? c < b.c : w < b.w;
    }
    Node() {}
} edge[MAXM], tmp[MAXM];

inline int kruskal(int x) {
    for (register int i = 1; i <= m; i++) {
        tmp[i] = edge[i];
        if (!tmp[i].c) tmp[i].w += x;
    }
    stable_sort(tmp + 1, tmp + m + 1);
    s->clear(n);
    register int cost = 0, cnt = 1, cw = 0;
    for (register int i = 1; i <= m; i++) {
        register int u = tmp[i].u, v = tmp[i].v, w = tmp[i].w, c = tmp[i].c;
        if (s->get(u) != s->get(v)) s->put(u, v), cost += w, cw += 1 - c, cnt++;
        if (cnt == n) break;
    }
    return ans = cost - x * k, cw;
}

int main() {
    // freopen("tree.in", "r", stdin);
    // freopen("tree.out", "w", stdout);
    read(n), read(m), read(k);
    s = new DisjointSet(n);
    for (register int i = 1, u, v, w, c; i <= m; i++)
        read(u), read(v), read(w), read(c), edge[i] = Node(u + 1, v + 1, w, c);
    register int l = -100, r = 100 + 1;
    while (l < r - 1) {
        register int mid = l + r >> 1;
        if (kruskal(mid) >= k)
            l = mid;
        else
            r = mid;
    }
    kruskal(l);
    cout << ans;
    return 0;
}
