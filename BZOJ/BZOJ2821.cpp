/*
 * created by xehoth on 11-01-2017
 * an implement of BlockList
 */
#include <bits/stdc++.h>

inline int read() {
    int x = 0;
    bool iosig = false;
    char c = getchar();
    for (; !isdigit(c); c = getchar())
        if (c == '-') iosig = 1;
    for (; isdigit(c); c = getchar()) x = (x << 1) + (x << 3) + (c ^ '0');
    return iosig ? -x : x;
}

const int MAXN = 100010;
const int MAXB = 320;

int n;
int sum[MAXB][MAXN], f[MAXB][MAXB], a[MAXN], pos[MAXN], l[MAXN], r[MAXN];
int tmpSum[MAXN];
inline void solve() {
    static int ans, c, m;
    int x, y, L, R;

    x = read(), y = read();
    x = (x + ans) % n + 1, y = (y + ans) % n + 1, ans = 0;
    if (x > y) std::swap(x, y);
    L = pos[x], R = pos[y];
    if (L == R) {
        for (int i = x; i <= y; i++) tmpSum[a[i]] = 0;
        for (int i = x; i <= y; i++)
            if ((++tmpSum[a[i]]) != 1) ans += (tmpSum[a[i]] & 1) ? -1 : 1;
    } else {
        ans = f[L + 1][R - 1];
        for (int i = x; i <= r[L]; i++)
            tmpSum[a[i]] = sum[R - 1][a[i]] - sum[L][a[i]];
        for (int i = l[R]; i <= y; i++)
            tmpSum[a[i]] = sum[R - 1][a[i]] - sum[L][a[i]];
        for (int i = x; i <= r[L]; i++)
            if (++tmpSum[a[i]] != 1) ans += (tmpSum[a[i]] & 1) ? -1 : 1;
        for (int i = l[R]; i <= y; i++)
            if (++tmpSum[a[i]] != 1) ans += (tmpSum[a[i]] & 1) ? -1 : 1;
    }
    printf("%d\n", ans);
}

int main() {
    // freopen("in.in", "r", stdin);
    static int sz, cnt, c, m;

    n = read(), c = read(), m = read(), sz = sqrt(n);
    cnt = n / sz + (n % sz != 0);
    for (int i = 1; i <= n; i++) a[i] = read();
    for (int i = 1; i <= n; i++) pos[i] = (i - 1) / sz + 1;

    for (int i = 1; i <= n; i++) {
        r[pos[i]] = i;
        if (!l[pos[i]]) {
            l[pos[i]] = i;
        }
    }

    for (int i = 1; i <= cnt; i++) {
        for (int j = 1; j <= c; j++) sum[i][j] = sum[i - 1][j];
        for (int j = l[i]; j <= r[i]; j++) sum[i][a[j]]++;
    }
    for (int i = 1; i <= cnt; i++) {
        for (int j = 1; j <= c; j++) tmpSum[j] = 0;
        int tmp = 0;
        for (int j = i; j <= cnt; j++) {
            for (int k = l[j]; k <= r[j]; k++)
                if (tmpSum[a[k]]++) tmp += (tmpSum[a[k]] & 1) ? -1 : 1;
            f[i][j] = tmp;
        }
    }
    for (int i = 1; i <= m; i++) solve();
    return 0;
}
