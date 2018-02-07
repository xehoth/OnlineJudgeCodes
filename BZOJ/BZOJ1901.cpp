/*
 * created by xehoth on 10-01-2017
 */
#include <bits/stdc++.h>
const int MAXN = 60000 + 1000;
const int MAXM = 2400000;
int n, q, m, tot;
int a[MAXN], t[MAXN];
int T[MAXN], lson[MAXM], rson[MAXM], c[MAXM];
int S[MAXN];
struct Query {
    int kind;
    int l, r, k;
} que[10010];
inline void init(const int k) {
    std::sort(t, t + k), m = std::unique(t, t + k) - t;
}
inline int getPos(int x) { return std::lower_bound(t, t + m, x) - t; }
inline int build(int l, int r) {
    int root = tot++;
    c[root] = 0;
    if (l != r) {
        int mid = (l + r) >> 1;
        lson[root] = build(l, mid);
        rson[root] = build(mid + 1, r);
    }
    return root;
}
inline int insert(int root, int pos, int val) {
    int newroot = tot++, tmp = newroot;
    int l = 0, r = m - 1;
    c[newroot] = c[root] + val;
    while (l < r) {
        int mid = (l + r) >> 1;
        if (pos <= mid) {
            lson[newroot] = tot++;
            rson[newroot] = rson[root];
            newroot = lson[newroot];
            root = lson[root];
            r = mid;
        } else {
            rson[newroot] = tot++;
            lson[newroot] = lson[root];
            newroot = rson[newroot];
            root = rson[root];
            l = mid + 1;
        }
        c[newroot] = c[root] + val;
    }
    return tmp;
}
inline int lowbit(const int x) { return x & (-x); }
int use[MAXN];
inline void modify(int x, int pos, int d) {
    while (x <= n) {
        S[x] = insert(S[x], pos, d);
        x += lowbit(x);
    }
}
inline int sum(int x) {
    int ret = 0;
    while (x > 0) {
        ret += c[lson[use[x]]];
        x -= lowbit(x);
    }
    return ret;
}
inline int query(int left, int right, int k) {
    int left_root = T[left - 1], right_root = T[right];
    int l = 0, r = m - 1;
    for (int i = left - 1; i; i -= lowbit(i)) use[i] = S[i];
    for (int i = right; i; i -= lowbit(i)) use[i] = S[i];
    while (l < r) {
        int mid = (l + r) >> 1;
        int tmp = sum(right) - sum(left - 1) + c[lson[right_root]] -
                  c[lson[left_root]];
        if (tmp >= k) {
            r = mid;
            for (int i = left - 1; i; i -= lowbit(i)) use[i] = lson[use[i]];
            for (int i = right; i; i -= lowbit(i)) use[i] = lson[use[i]];
            left_root = lson[left_root];
            right_root = lson[right_root];
        } else {
            l = mid + 1;
            k -= tmp;
            for (int i = left - 1; i; i -= lowbit(i)) use[i] = rson[use[i]];
            for (int i = right; i; i -= lowbit(i)) use[i] = rson[use[i]];
            left_root = rson[left_root];
            right_root = rson[right_root];
        }
    }
    return l;
}

int main() {
    // freopen("input.txt", "r", stdin);
    scanf("%d%d", &n, &q);
    tot = 0;
    m = 0;
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        t[m++] = a[i];
    }
    char op[10];
    for (int i = 0; i < q; i++) {
        scanf("%s", op);
        if (op[0] == 'Q') {
            que[i].kind = 0;
            scanf("%d%d%d", &que[i].l, &que[i].r, &que[i].k);
        } else {
            que[i].kind = 1;
            scanf("%d%d", &que[i].l, &que[i].r);
            t[m++] = que[i].r;
        }
    }
    init(m);
    T[0] = build(0, m - 1);
    for (int i = 1; i <= n; i++) T[i] = insert(T[i - 1], getPos(a[i]), 1);
    for (int i = 1; i <= n; i++) S[i] = T[0];
    for (int i = 0; i < q; i++) {
        if (que[i].kind == 0)
            printf("%d\n", t[query(que[i].l, que[i].r, que[i].k)]);
        else {
            modify(que[i].l, getPos(a[que[i].l]), -1);
            modify(que[i].l, getPos(que[i].r), 1);
            a[que[i].l] = que[i].r;
        }
    }
    return 0;
}