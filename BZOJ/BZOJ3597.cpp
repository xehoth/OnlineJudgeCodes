#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
#define D 5990
const int N = 6000;
const int M = 10000;
double dis[N];
bool f[N], flag;
struct S {
    int st, en;
    double va;
} aa[M];
int n, m, tot, point[N], next[M], l[N], cnt[N];
struct Line {
    int st, en, a, b, c, d;
} e[N];
#define inf 1e9
#define eps 1e-6
#define mid (l + r) / 2
inline void add(int x, int y, double z) {
    next[++tot] = point[x];
    point[x] = tot;
    aa[tot].st = x;
    aa[tot].en = y;
    aa[tot].va = z;
}
inline void SPFA(int x) {
    int h = 0, t = 1, u, i;
    for (i = 1; i <= n; ++i) dis[i] = inf, cnt[i] = 0, f[i] = true;
    dis[x] = 0.;
    l[t] = x;
    ++cnt[x];
    while (h != t) {
        h = h % D + 1;
        u = l[h];
        f[u] = true;
        for (i = point[u]; i; i = next[i])
            if (dis[aa[i].en] > dis[u] + aa[i].va + eps) {
                dis[aa[i].en] = dis[u] + aa[i].va;
                if (f[aa[i].en]) {
                    if (++cnt[aa[i].en] > n) {
                        flag = false;
                        return;
                    }
                    t = t % D + 1;
                    l[t] = aa[i].en;
                    f[aa[i].en] = false;
                }
            }
    }
}
inline bool check(double x) {
    int i;
    tot = 0;
    memset(point, 0, sizeof(point));
    for (i = 1; i <= m; ++i) {
        if (e[i].st == n - 1) continue;
        add(e[i].st, e[i].en, (double)e[i].b + (double)e[i].d + x);
        if (e[i].c != 0)
            add(e[i].en, e[i].st, (double)e[i].a - (double)e[i].d + x);
    }
    flag = true;
    SPFA(n);
    return flag == false;
}

int main() {
    int i;
    scanf("%d%d", &n, &m);
    for (n += 2, i = 1; i <= m; ++i)
        scanf("%d%d%d%d%d%d", &e[i].st, &e[i].en, &e[i].a, &e[i].b, &e[i].c,
              &e[i].d);
    double l = 0, r = inf, ans = 0;
    while (l + eps < r) {
        if (check(mid))
            ans = max(ans, mid), l = mid;
        else
            r = mid;
    }
    printf("%.2f\n", ans);
}
