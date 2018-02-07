/*
 * created by xehoth on 11-01-2017
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

const int MAXN = 5e4 + 3;
const int MAXM = 1e6 + 3;
const int MAXQ = 2e5 + 3;

struct Node {
    int l, r, bl, id;
    inline bool operator<(const Node &b) const {
        return bl < b.bl || bl == b.bl && r < b.r;
    }
};

int main() {
    static int n, m, S, l, r, tl, tr, tot, ans[MAXQ], a[MAXN], cnt[MAXM];
    static Node q[MAXQ];

    n = read(), S = sqrt(n);
    for (int i = 1; i <= n; i++) a[i] = read();
    m = read();
    for (int i = 1; i <= m; i++) {
        l = read(), r = read(), q[i].id = i;
        tl = (l - 1) / S + 1;
        q[i].l = l, q[i].r = r, q[i].bl = tl;
    }
    std::sort(q + 1, q + m + 1);
    tl = 0, tr = 0;
    for (int i = 1; i <= m; i++) {
        l = q[i].l, r = q[i].r;
        while (tl < l)
            if (!--cnt[a[tl++]]) --tot;
        while (tl > l)
            if (!cnt[a[--tl]]++) ++tot;
        while (tr < r)
            if (!cnt[a[++tr]]++) ++tot;
        while (tr > r)
            if (!--cnt[a[tr--]]) --tot;
        ans[q[i].id] = tot;
    }
    for (int i = 1; i <= m; i++) printf("%d\n", ans[i]);
    return 0;
}