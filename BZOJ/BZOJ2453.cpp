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

const int MAXN = 1000001;

int pos[MAXN], a[MAXN], b[MAXN], add[MAXN];
int last[MAXN], pre[MAXN];
int n, m, block;

inline void init(int x) {
    int l = (x - 1) * block + 1, r = x * block;

    if (r > n) r = n;
    for (int i = l; i <= r; i++) pre[i] = b[i];
    std::sort(pre + l, pre + r + 1);
}

inline int binarySearch(int x, int tar) {
    int l = (x - 1) * block + 1, r = x * block;

    if (r > n) r = n;
    int mid, ret = l;
    while (l <= r) {
        mid = (l + r) >> 1;
        if (pre[mid] >= tar)
            r = mid - 1;
        else
            l = mid + 1;
    }
    return l - ret;
}

inline void modify(int x, int tar) {
    int tmp;
    for (int i = 1; i <= n; i++) last[a[i]] = 0;
    a[x] = tar;
    for (int i = 1; i <= n; i++) {
        tmp = b[i];
        b[i] = last[a[i]];
        if (tmp != b[i]) init(pos[i]);
        last[a[i]] = i;
    }
}

int query(int l, int r) {
    int d = 0;
    if (pos[l] == pos[r]) {
        for (int i = l; i <= r; i++)
            if (b[i] < l) d++;
    } else {
        for (int i = l; i <= pos[l] * block; i++)
            if (b[i] < l) d++;
        for (int i = (pos[r] - 1) * block + 1; i <= r; i++)
            if (b[i] < l) d++;
    }
    for (int i = pos[l] + 1; i < pos[r]; i++) d += binarySearch(i, l);
    return d;
}

int main() {
    // freopen("in.in", "r", stdin);
    int x, y, T;
    char op;

    n = read(), T = read();
    block = int(sqrt(n) + log(2 * n) / log(2));

    for (int i = 1; i <= n; i++) {
        a[i] = read();
        b[i] = last[a[i]];
        last[a[i]] = i;
        pos[i] = (i - 1) / block + 1;
    }

    if (n % block)
        m = n / block + 1;
    else
        m = n / block;

    for (int i = 1; i <= m; i++) init(i);
    while (T--) {
        scanf("\n%c", &op), x = read(), y = read();
        if (op == 'R')
            modify(x, y);
        else
            printf("%d\n", query(x, y));
    }
    return 0;
}