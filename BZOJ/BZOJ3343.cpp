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
int n, m, block;

inline void init(int x) {
    int l = (x - 1) * block + 1, r = x * block;
    if (r > n) r = n;
    for (int i = l; i <= r; i++) b[i] = a[i];
    std::sort(b + l, b + r + 1);
}

inline int binarySearch(int x, int tar) {
    int l = (x - 1) * block + 1;
    int r = x * block;
    if (r > n) r = n;
    int mid, ret = r;
    while (l <= r) {
        mid = (l + r) >> 1;
        if (b[mid] >= tar)
            r = mid - 1;
        else
            l = mid + 1;
    }
    return ret - l + 1;
}

inline void modify(int l, int r, int tar) {
    if (pos[l] == pos[r]) {
        for (int i = l; i <= r; i++) a[i] += tar;
    } else {
        for (int i = l; i <= pos[l] * block; i++) a[i] += tar;
        for (int i = (pos[r] - 1) * block + 1; i <= r; i++) a[i] += tar;
    }
    init(pos[l]), init(pos[r]);
    for (int i = pos[l] + 1; i < pos[r]; i++) add[i] += tar;
}

inline int query(int l, int r, int tar) {
    int d = 0, i;
    if (pos[l] == pos[r]) {
        for (int i = l; i <= r; i++)
            if (add[i] + a[i] >= tar) d++;
    } else {
        for (int i = l; i <= pos[l] * block; i++)
            if (a[i] + add[pos[i]] >= tar) d++;
        for (int i = (pos[r] - 1) * block + 1; i <= r; i++)
            if (a[i] + add[pos[i]] >= tar) d++;
    }
    for (int i = pos[l] + 1; i < pos[r]; i++)
        d += binarySearch(i, tar - add[i]);
    return d;
}

int main() {
    // freopen("in.in", "r", stdin);
    int x, y, z, T;
    char op;

    n = read(), T = read();
    block = (int)sqrt(n);
    for (int i = 1; i <= n; i++) {
        a[i] = read();
        b[i] = i;
        pos[i] = (i - 1) / block + 1;
    }

    if (n % block)
        m = n / block + 1;
    else
        m = n / block;
    for (int i = 1; i <= m; i++) init(i);
    while (T--) {
        scanf("\n%c", &op);
        x = read(), y = read(), z = read();
        if (op == 'M')
            modify(x, y, z);
        else
            printf("%d\n", query(x, y, z));
    }
    return 0;
}