#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
using namespace std;
typedef long long ll;
const int N = 100003;
void read(int &k) {
    k = 0;
    int fh = 1;
    char c = getchar();
    for (; c < '0' || c > '9'; c = getchar())
        if (c == '-') fh = -1;
    for (; c >= '0' && c <= '9'; c = getchar())
        k = (k << 1) + (k << 3) + c - '0';
    k = k * fh;
}

bool vis[N];
ll A[N], ans = 0;
struct TIME {
    int x, y, la;
} Time[N];
struct edge {
    int nxt, to;
} E[N << 1];
struct node {
    int l, r, lca, id, tim;
} Q[N];
int cal[N], n, m, q, V[N], W[N], f[N][17], deep[N], bel[N << 1];
int point[N], color[N], colorchange[N], pos[N << 1], L[N], R[N], cnt = 0;

void ins(int x, int y) {
    E[++cnt].nxt = point[x];
    E[cnt].to = y;
    point[x] = cnt;
}
void _(int x, int fa) {
    pos[L[x] = ++cnt] = x;
    for (int i = 1; i <= 16; ++i) {
        f[x][i] = f[f[x][i - 1]][i - 1];
        if (f[x][i] == 0) break;
    }
    for (int tmp = point[x]; tmp; tmp = E[tmp].nxt)
        if (E[tmp].to != fa) {
            deep[E[tmp].to] = deep[x] + 1;
            f[E[tmp].to][0] = x;
            _(E[tmp].to, x);
        }
    pos[R[x] = ++cnt] = x;
}
int LCA(int x, int y) {
    if (deep[x] < deep[y]) swap(x, y);
    int d = deep[x] - deep[y];
    for (int i = 0; i <= 16; ++i)
        if (d & (1 << i)) x = f[x][i];
    if (x == y) return x;
    for (int i = 16; i >= 0; --i)
        if (f[x][i] != f[y][i]) x = f[x][i], y = f[y][i];
    return f[x][0];
}
void update(int x) {
    if (vis[x]) {
        ans -= 1ll * V[color[x]] * W[cal[color[x]]];
        --cal[color[x]];
    } else {
        ++cal[color[x]];
        ans += 1ll * V[color[x]] * W[cal[color[x]]];
    }
    vis[x] = !vis[x];
}
void change(int a, int b) {
    if (vis[a]) {
        update(a);
        color[a] = b;
        update(a);
    } else
        color[a] = b;
}
void Timechange(int &a, int b) {
    while (a < b) {
        ++a;
        change(Time[a].x, Time[a].y);
    }
    while (a > b) {
        change(Time[a].x, Time[a].la);
        --a;
    }
}
bool cmp(node A, node B) {
    return bel[A.l] == bel[B.l]
               ? (bel[A.r] == bel[B.r] ? A.tim < B.tim : A.r < B.r)
               : A.l < B.l;
}

int main() {
    read(n);
    read(m);
    read(q);
    for (int i = 1; i <= m; ++i) read(V[i]);
    for (int i = 1; i <= n; ++i) read(W[i]);
    int op, u, v, timecount = 0, tmp = 0, lca;
    for (int i = 1; i < n; ++i) {
        read(u);
        read(v);
        ins(u, v);
        ins(v, u);
    }
    for (int i = 1; i <= n; ++i) read(color[i]), colorchange[i] = color[i];

    cnt = 0;
    _(1, 0);

    for (int i = 1; i <= q; ++i) {
        read(op);
        read(u);
        read(v);
        if (op == 0) {
            Time[++timecount] = (TIME){u, v, colorchange[u]};
            colorchange[u] = v;
        } else {
            if (L[u] > L[v]) swap(u, v);
            lca = LCA(u, v);
            if (lca == u)
                Q[++tmp] = (node){L[u], L[v], 0, tmp, timecount};
            else
                Q[++tmp] = (node){R[u], L[v], lca, tmp, timecount};
        }
    }

    int nn = n << 1, sq = pow(nn, 2.0 / 3) * 0.5, sign = 0;
    cnt = 1;
    for (int i = 1; i <= nn; ++i) {
        bel[i] = sign;
        ++cnt;
        if (cnt > sq) {
            cnt = 1;
            ++sign;
        }
    }

    sort(Q + 1, Q + tmp + 1, cmp);

    int l = 1, r = 0, now = 0, tol, tor;
    for (int i = 1; i <= tmp; ++i) {
        tol = Q[i].l;
        tor = Q[i].r;
        Timechange(now, Q[i].tim);
        while (l < tol) update(pos[l++]);
        while (l > tol) update(pos[--l]);
        while (r < tor) update(pos[++r]);
        while (r > tor) update(pos[r--]);
        if (Q[i].lca) update(Q[i].lca);
        A[Q[i].id] = ans;
        if (Q[i].lca) update(Q[i].lca);
    }

    for (int i = 1; i <= tmp; ++i) printf("%lld\n", A[i]);

    return 0;
}
