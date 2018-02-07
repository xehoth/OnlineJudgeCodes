#include <bits/stdc++.h>
inline char read() {
    static const int IO_LEN = 1024 * 1024;
    static char buf[IO_LEN], *ioh, *iot;
    if (ioh == iot) {
        iot = (ioh = buf) + fread(buf, 1, IO_LEN, stdin);
        if (ioh == iot) return -1;
    }
    return *ioh++;
}
template <class T>
inline void read(T &x) {
    static char ioc;
    static bool iosig = 0;
    for (iosig = 0, ioc = read(); !isdigit(ioc); ioc = read())
        if (ioc == '-') iosig = 1;
    for (x = 0; isdigit(ioc); ioc = read())
        x = (x << 1) + (x << 3) + (ioc ^ '0');
    if (iosig) x = -x;
}
const int MAXN = 100005;
int n, m, root[MAXN], ls[100 * MAXN], rs[100 * MAXN], sum[100 * MAXN], cnt, ans,
    get;
inline void build(int x, int &y, int l, int r, int v) {
    y = ++cnt;
    sum[y] = sum[x] + v;
    if (l == r) return;
    ls[y] = ls[x];
    rs[y] = rs[x];
    register int mid = (l + r) >> 1;
    if (v > mid)
        build(rs[x], rs[y], mid + 1, r, v);
    else
        build(ls[x], ls[y], l, mid, v);
}
inline int query(int x, int y, int l, int r, int lim) {
    register int mid = (l + r) >> 1;
    if (r <= lim)
        return sum[y] - sum[x];
    else if (lim <= mid)
        return query(ls[x], ls[y], l, mid, lim);
    else
        return sum[ls[y]] - sum[ls[x]] + query(rs[x], rs[y], mid + 1, r, lim);
}
int main() {
    read(n);
    for (register int i = 1, v; i <= n; i++)
        read(v), build(root[i - 1], root[i], 1, 1e9, v);
    read(m);
    for (int l, r, i = 1; i <= m; i++) {
        read(l), read(r);
        ans = 1;
        while (true) {
            get = query(root[l - 1], root[r], 1, 1e9, ans);
            if (get < ans) break;
            ans = get + 1;
        }
        std::cout << ans << "\n";
    }
    return 0;
}