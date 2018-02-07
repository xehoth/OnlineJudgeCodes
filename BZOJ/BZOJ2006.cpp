#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <queue>

#define LL long long
#define hash _hash_

using namespace std;

const int MAXN = 500000 + 3;

int h_num[MAXN], hash[MAXN], tot, top;

void hash_init() {
    sort(h_num + 1, h_num + top + 1);
    tot = 0;
    hash[++tot] = h_num[1];
    for (int i = 2; i <= top; i++) {
        if (h_num[i] != h_num[i - 1]) hash[++tot] = h_num[i];
    }
}

int hash_find(int x) {
    int l = 1, r = tot, mid;
    while (l <= r) {
        mid = (l + r) >> 1;
        if (hash[mid] < x)
            l = mid + 1;
        else
            r = mid - 1;
    }
    return l;
}

struct T {
    int v, r, k;
    bool operator<(const T &a) const { return v < a.v; }
};
priority_queue<T> que;
struct Node {
    int ls, rs, w;
    Node() { ls = rs = w = 0; }
} node[MAXN * 20];
int a[MAXN], sum[MAXN], root[MAXN], sz, N, K, L, R;

void update(int &i, int l, int r, int w) {
    node[++sz] = node[i];
    i = sz;
    node[i].w++;
    if (l == r) return;
    int mid = (l + r) >> 1;
    if (w <= mid)
        update(node[i].ls, l, mid, w);
    else
        update(node[i].rs, mid + 1, r, w);
}

int query(int i, int j, int l, int r, int k) {
    if (l == r) return l;
    int t = node[node[j].ls].w - node[node[i].ls].w;
    int mid = (l + r) >> 1;
    if (t >= k)
        return query(node[i].ls, node[j].ls, l, mid, k);
    else
        return query(node[i].rs, node[j].rs, mid + 1, r, k - t);
}

void get(int i, int &l, int &r) {
    l = i - R, r = i - L;
    if (l < 0) l = 0;
    if (r < 0) r = -1;
}

LL solve() {
    sum[0] = 0;
    top = 0;
    h_num[++top] = 0;
    for (int i = 1; i <= N; i++) {
        sum[i] = sum[i - 1] + a[i];
        h_num[++top] = sum[i];
    }
    hash_init();
    for (int i = 0; i <= N; i++) sum[i] = hash_find(sum[i]);
    sz = 0;
    root[1] = root[0] = 0;
    update(root[1], 1, tot, sum[0]);
    for (int i = 1; i <= N; i++) {
        root[i + 1] = root[i];
        update(root[i + 1], 1, tot, sum[i]);
    }
    while (!que.empty()) que.pop();
    LL ans = 0;
    int now, l, r;
    for (int i = L; i <= N; i++) {
        get(i, l, r);
        if (r < 0) continue;
        // printf("i = %d l = %d r = %d\n",i,l,r);
        now = hash[sum[i]] - hash[query(root[l], root[r + 1], 1, tot, 1)];
        // printf("q = %d now = %d\n",query(root[l],root[r+1],1,tot,1),now);
        que.push((T){now, i, 1});
    }
    T u;
    while (true) {
        u = que.top();
        que.pop();
        // printf("u.v = %d\n",u.v);
        ans += u.v;
        K--;
        if (!K) break;
        get(u.r, l, r);
        if (r - l + 1 <= u.k) continue;
        now =
            hash[sum[u.r]] - hash[query(root[l], root[r + 1], 1, tot, u.k + 1)];
        que.push((T){now, u.r, u.k + 1});
    }
    return ans;
}

int main() {
    while (~scanf("%d %d %d %d", &N, &K, &L, &R)) {
        for (int i = 1; i <= N; i++) scanf("%d", &a[i]);
        // cout << solve() << endl;
        printf("%lld\n", solve());
    }
    return 0;
}