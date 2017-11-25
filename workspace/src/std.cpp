#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#define ll long long
using namespace std;
const int maxn = 500010, inf = 1000000000;
struct poi {
    int too, pre, sum;
} e[maxn];
struct tjm {
    int sum, tag;
} a[maxn];
struct zs {
    int x, y, len;
} edge[maxn];
int n, m, x, y, z, flag, tot, tot2, cnt;
int last[maxn], size[maxn], fa[maxn], dep[maxn], son[maxn], w[maxn], top[maxn];
void read(int &k) {
    int f = 1;
    k = 0;
    char c = getchar();
    while (c < '0' || c > '9') c == '-' && (f = -1), c = getchar();
    while (c <= '9' && c >= '0') k = k * 10 + c - '0', c = getchar();
    k *= f;
}
void add(int x, int y, int z) {
    e[++tot].too = y;
    e[tot].sum = z;
    e[tot].pre = last[x];
    last[x] = tot;
}
void dfs1(int x) {
    size[x] = 1;
    for (int i = last[x]; i; i = e[i].pre) {
        int too = e[i].too;
        if (too != fa[x]) {
            fa[too] = x;
            dep[too] = dep[x] + e[i].sum;
            dfs1(too);
            if (size[too] > size[son[x]]) son[x] = too;
            size[x] += size[too];
        }
    }
}
void dfs2(int x, int tp) {
    w[x] = ++cnt;
    top[x] = tp;
    if (son[x]) dfs2(son[x], tp);
    for (int i = last[x]; i; i = e[i].pre)
        if (e[i].too != son[x] && e[i].too != fa[x]) dfs2(e[i].too, e[i].too);
}
void pushdown(int x) {
    if (a[x].tag == inf) return;
    int tag = a[x].tag;
    a[x].tag = inf;
    a[x << 1].tag = min(tag, a[x << 1].tag);
    a[x << 1 | 1].tag = min(tag, a[x << 1 | 1].tag);
}
void update(int x, int nl, int nr, int l, int r, int delta) {
    /// if(nl!=nr)pushdown(x);
    if (l <= nl && nr <= r)
        a[x].tag = min(a[x].tag, delta);
    else {
        // printf("%d %d\n",nl,nr);
        int mid = (nl + nr) >> 1;
        if (l <= mid) update(x << 1, nl, mid, l, r, delta);
        if (r > mid) update(x << 1 | 1, mid + 1, nr, l, r, delta);
    }
}
ll query(int x, int nl, int nr, int num) {
    if (nl != nr) pushdown(x);
    if (nl == num && nr == num)
        return a[x].tag;
    else {
        int mid = (nl + nr) >> 1;
        if (nl <= num && num <= mid) return query(x << 1, nl, mid, num);
        if (mid < num && num <= nr) return query(x << 1 | 1, mid + 1, nr, num);
    }
}
void work(int x, int y, int len) {
    std::cerr << x << ' ' << y << ' ' << len << std::endl;
    int f1 = top[x], f2 = top[y];
    while (f1 != f2) {
        if (dep[f1] < dep[f2]) swap(x, y), swap(f1, f2);
        std::cerr << w[f1] << ' ' << w[x] << std::endl;
        update(1, 1, cnt, w[f1], w[x], len);
        x = fa[f1];
        f1 = top[x];
    }
    if (x == y) return;
    if (dep[x] < dep[y]) swap(x, y);
    update(1, 1, cnt, w[son[y]], w[x], len);
}
void build(int x, int l, int r) {
    a[x].tag = inf;
    int mid = (l + r) >> 1;
    if (l != r) build(x << 1, l, mid), build(x << 1 | 1, mid + 1, r);
}
int main() {
    read(n);
    read(m);
    for (int i = 1; i <= m; i++) {
        read(x);
        read(y);
        read(z);
        read(flag);
        if (!flag)
            edge[++tot2].x = x, edge[tot2].y = y, edge[tot2].len = z;
        else
            add(x, y, z), add(y, x, z);
    }
    dfs1(1);
    dfs2(1, 1);
    build(1, 1, n);
    for (int i = 1; i <= tot2; i++)
        work(edge[i].x, edge[i].y,
             edge[i].len + dep[edge[i].x] + dep[edge[i].y]);
    for (int i = 2; i <= n; i++) {
        int ans = query(1, 1, cnt, w[i]);
        if (ans != inf)
            printf("%d ", ans - dep[i]);
        else
            printf("-1 ");
    }
    return 0;
}