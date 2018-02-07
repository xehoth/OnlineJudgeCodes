#include <bits/stdc++.h>
using namespace std;
map<int, int> M1, M2, M3, M4;
const int M = 100050;
int lastans = 0;
int get_int() {
    int x = 0;
    char c;
    for (c = getchar(); c < '0' || c > '9'; c = getchar())
        ;
    for (; c >= '0' && c <= '9'; c = getchar()) x = x * 10 + c - '0';
    return x;
}
int n, m, tou, wei;
const int P = 80 * M;
int lc[P], rc[P], sum[P], tag[P];
int root, tot;
void down(int v, int s, int t) {
    if (tag[v]) {
        if (!lc[v]) lc[v] = ++tot;
        if (!rc[v]) rc[v] = ++tot;
        tag[lc[v]] += tag[v];
        tag[rc[v]] += tag[v];
        int mid = (s + t) >> 1;
        sum[lc[v]] += tag[v] * (mid - s + 1);
        sum[rc[v]] += tag[v] * (t - mid);
        tag[v] = 0;
    }
}
void up(int v) { sum[v] = sum[lc[v]] + sum[rc[v]]; }
void add(int &v, int s, int t, int l, int r, int x) {
    if (!v) v = ++tot;
    if (s >= l && t <= r) {
        sum[v] += x * (t - s + 1);
        tag[v] += x;
        return;
    }
    down(v, s, t);
    int mid = (s + t) >> 1;
    if (mid >= l) add(lc[v], s, mid, l, r, x);
    if (mid + 1 <= r) add(rc[v], mid + 1, t, l, r, x);
    up(v);
}
int ask(int v, int s, int t, int l, int r) {
    if (!v) return 0;
    if (s >= l && t <= r) return sum[v];
    int mid = (s + t) >> 1, A = 0;
    down(v, s, t);
    if (mid >= l) A += ask(lc[v], s, mid, l, r);
    if (mid + 1 <= r) A += ask(rc[v], mid + 1, t, l, r);
    up(v);
    return A;
}
int ask(int v, int s, int t, int k) {
    if (s == t) return s;
    int mid = (s + t) >> 1, A = 0;
    down(v, s, t);
    if (sum[lc[v]] >= k)
        A = ask(lc[v], s, mid, k);
    else
        A = ask(rc[v], mid + 1, t, k - sum[lc[v]]);
    up(v);
    return A;
}
int main() {
    // freopen("onlinejudge.in","r",stdin);
    // freopen("std.out","w",stdout);
    n = get_int(), m = get_int();
    tou = 0, wei = n;
    add(root, -m, n + m, 1, n, 1);
    int mm = m;
    for (; mm--;) {
        int _ = get_int();
        if (_ == 1) {
            int x = get_int() - lastans, y = get_int() - lastans;
            int t = M1[x];
            if (!t)
                t = M1[y] = x, M2[x] = y;
            else
                M1[x] = 0, M2[t] = y, M1[y] = t;
            x = t;
            t = M3[x];
            if (!t) t = x;
            printf("%d\n", lastans = (ask(root, -m, n + m, -m, t - 1) + 1));
        }
        if (_ == 2 || _ == 3) {
            int x = get_int() - lastans;
            int t = M1[x];
            if (!t) t = x;
            x = t;
            t = M3[x];
            if (!t) t = x;
            printf("%d\n", lastans = (ask(root, -m, n + m, -m, t - 1) + 1));
            add(root, -m, n + m, t, t, -1);
            M4[t] = 0;
            if (_ == 2)
                M4[t = M3[x] = --tou] = x;
            else
                M4[t = M3[x] = ++wei] = x;
            add(root, -m, n + m, t, t, 1);
        }
        if (_ == 4) {
            int k = get_int() - lastans;
            int x = ask(root, -m, n + m, k);
            int t = M4[x];
            if (!t) t = x;
            x = t;
            t = M2[x];
            if (!t)
                printf("%d\n", lastans = x);
            else
                printf("%d\n", lastans = t);
        }
    }
}