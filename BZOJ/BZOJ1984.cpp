#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#define M 100100
using namespace std;
struct Segtree {
    Segtree *ls, *rs;
    int change_mark, add_mark, max_num;
    void tAdd(int x);
    void tChange(int x);
    void* operator new(size_t size);
} * tree, mempool[M << 1], *C = mempool;
struct Edges {
    int to, f, next;
} table[M << 1];
int head[M], tot = 1;
int n;
int fa[M], son[M], dpt[M], size[M];
int pos[M], top[M], f[M], posf[M], cnt;
void Segtree ::tAdd(int x) {
    add_mark += x;
    max_num += x;
}
void Segtree ::tChange(int x) {
    add_mark = 0;
    change_mark = x;
    max_num = x;
}
void* Segtree ::operator new(size_t size) {
    C->change_mark = -1;
    return C++;
}
void buildTree(Segtree*& p, int x, int y) {
    int mid = x + y >> 1;
    p = new Segtree;
    if (x == y) {
        p->max_num = posf[mid];
        return;
    }
    buildTree(p->ls, x, mid);
    buildTree(p->rs, mid + 1, y);
    p->max_num = max(p->ls->max_num, p->rs->max_num);
}
void tChange(Segtree* p, int x, int y, int z, int v) {
    int mid = x + y >> 1;
    if (x == y) {
        p->max_num = v;
        return;
    }
    if (~p->change_mark) {
        p->ls->tChange(p->change_mark);
        p->rs->tChange(p->change_mark);
        p->change_mark = -1;
    }
    if (p->add_mark) {
        p->ls->tAdd(p->add_mark);
        p->rs->tAdd(p->add_mark);
        p->add_mark = 0;
    }
    if (z <= mid)
        tChange(p->ls, x, mid, z, v);
    else
        tChange(p->rs, mid + 1, y, z, v);
    p->max_num = max(p->ls->max_num, p->rs->max_num);
}
void tChange(Segtree* p, int x, int y, int l, int r, int v) {
    int mid = x + y >> 1;
    if (x == l && y == r) {
        p->tChange(v);
        return;
    }
    if (~p->change_mark) {
        p->ls->tChange(p->change_mark);
        p->rs->tChange(p->change_mark);
        p->change_mark = -1;
    }
    if (p->add_mark) {
        p->ls->tAdd(p->add_mark);
        p->rs->tAdd(p->add_mark);
        p->add_mark = 0;
    }
    if (r <= mid)
        tChange(p->ls, x, mid, l, r, v);
    else if (l > mid)
        tChange(p->rs, mid + 1, y, l, r, v);
    else
        tChange(p->ls, x, mid, l, mid, v),
            tChange(p->rs, mid + 1, y, mid + 1, r, v);
    p->max_num = max(p->ls->max_num, p->rs->max_num);
}
void tAdd(Segtree* p, int x, int y, int l, int r, int v) {
    int mid = x + y >> 1;
    if (x == l && y == r) {
        p->tAdd(v);
        return;
    }
    if (~p->change_mark) {
        p->ls->tChange(p->change_mark);
        p->rs->tChange(p->change_mark);
        p->change_mark = -1;
    }
    if (p->add_mark) {
        p->ls->tAdd(p->add_mark);
        p->rs->tAdd(p->add_mark);
        p->add_mark = 0;
    }
    if (r <= mid)
        tAdd(p->ls, x, mid, l, r, v);
    else if (l > mid)
        tAdd(p->rs, mid + 1, y, l, r, v);
    else
        tAdd(p->ls, x, mid, l, mid, v), tAdd(p->rs, mid + 1, y, mid + 1, r, v);
    p->max_num = max(p->ls->max_num, p->rs->max_num);
}
int Get_Ans(Segtree* p, int x, int y, int l, int r) {
    int mid = x + y >> 1;
    if (x == l && y == r) return p->max_num;
    if (~p->change_mark) {
        p->ls->tChange(p->change_mark);
        p->rs->tChange(p->change_mark);
        p->change_mark = -1;
    }
    if (p->add_mark) {
        p->ls->tAdd(p->add_mark);
        p->rs->tAdd(p->add_mark);
        p->add_mark = 0;
    }
    if (r <= mid) return Get_Ans(p->ls, x, mid, l, r);
    if (l > mid) return Get_Ans(p->rs, mid + 1, y, l, r);
    return max(Get_Ans(p->ls, x, mid, l, mid),
               Get_Ans(p->rs, mid + 1, y, mid + 1, r));
}
void tAdd(int x, int y, int z) {
    table[++tot].to = y;
    table[tot].f = z;
    table[tot].next = head[x];
    head[x] = tot;
}
void DFS1(int x) {
    int i;
    dpt[x] = dpt[fa[x]] + 1;
    size[x] = 1;
    for (i = head[x]; i; i = table[i].next) {
        if (table[i].to == fa[x]) continue;
        fa[table[i].to] = x;
        f[table[i].to] = table[i].f;
        DFS1(table[i].to);
        size[x] += size[table[i].to];
        if (size[table[i].to] > size[son[x]]) son[x] = table[i].to;
    }
}
void DFS2(int x) {
    int i;
    if (son[fa[x]] == x)
        top[x] = top[fa[x]];
    else
        top[x] = x;
    posf[pos[x] = ++cnt] = f[x];
    if (son[x]) DFS2(son[x]);
    for (i = head[x]; i; i = table[i].next) {
        if (table[i].to == fa[x] || table[i].to == son[x]) continue;
        DFS2(table[i].to);
    }
}
int Query(int x, int y) {
    int re = -2147483647, fx = top[x], fy = top[y];
    while (fx != fy) {
        if (dpt[fx] < dpt[fy]) swap(x, y), swap(fx, fy);
        re = max(re, Get_Ans(tree, 1, n, pos[fx], pos[x]));
        fx = top[x = fa[fx]];
    }
    if (x == y) return re;
    if (dpt[x] < dpt[y]) swap(x, y);
    re = max(re, Get_Ans(tree, 1, n, pos[y] + 1, pos[x]));
    return re;
}
void tChange(int x, int y, int z) {
    int fx = top[x], fy = top[y];
    while (fx != fy) {
        if (dpt[fx] < dpt[fy]) swap(x, y), swap(fx, fy);
        tChange(tree, 1, n, pos[fx], pos[x], z);
        fx = top[x = fa[fx]];
    }
    if (x == y) return;
    if (dpt[x] < dpt[y]) swap(x, y);
    tChange(tree, 1, n, pos[y] + 1, pos[x], z);
}
void _Add(int x, int y, int z) {
    int fx = top[x], fy = top[y];
    while (fx != fy) {
        if (dpt[fx] < dpt[fy]) swap(x, y), swap(fx, fy);
        tAdd(tree, 1, n, pos[fx], pos[x], z);
        fx = top[x = fa[fx]];
    }
    if (x == y) return;
    if (dpt[x] < dpt[y]) swap(x, y);
    tAdd(tree, 1, n, pos[y] + 1, pos[x], z);
}
int main() {
    int i, x, y, z;
    char p[20];
    cin >> n;
    for (i = 1; i < n; i++) {
        scanf("%d%d%d", &x, &y, &z);
        tAdd(x, y, z);
        tAdd(y, x, z);
    }
    DFS1(1);
    DFS2(1);
    buildTree(tree, 1, n);
    while (1) {
        scanf("%s", p);
        switch (p[1]) {
            case 'a':
                scanf("%d%d", &x, &y);
                printf("%d\n", Query(x, y));
                break;
            case 'h':
                int temp;
                scanf("%d%d", &temp, &z);
                x = table[temp << 1].to;
                y = table[temp << 1 | 1].to;
                if (dpt[x] < dpt[y]) swap(x, y);
                tChange(tree, 1, n, pos[x], z);
                break;
            case 'o':
                scanf("%d%d%d", &x, &y, &z);
                tChange(x, y, z);
                break;
            case 'd':
                scanf("%d%d%d", &x, &y, &z);
                _Add(x, y, z);
                break;
            case 't':
                return 0;
        }
    }
}
