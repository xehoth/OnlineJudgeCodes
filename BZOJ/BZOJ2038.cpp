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

const int MAXN = 50001;

typedef long long ll;

int n, m, block, l, r, c[MAXN], p[MAXN], left[MAXN], right[MAXN], len[MAXN];
ll tmp1, ans[MAXN], cnt[MAXN], tmp2;

inline bool cmp(int a, int b) {
    if (len[a] == len[b]) return right[a] < right[b];
    return len[a] < len[b];
}

inline ll gcd(ll a, ll b) { return !b ? a : gcd(b, a % b); }

int main() {
    // freopen("in.in", "r", stdin);
    n = read(), m = read();
    block = sqrt(n);

    for (int i = 1; i <= n; i++) c[i] = read();
    for (int i = 0; i < m; i++) {
        left[i] = read(), right[i] = read();
        p[i] = i;
        len[i] = (left[i] - 1) / block + 1;
    }

    std::sort(p, p + m, cmp);
    l = 1;

    for (int i = 0; i < m; i++) {
        while (l < left[p[i]]) cnt[c[l]]--, tmp1 -= cnt[c[l]], l++;
        while (l > left[p[i]]) tmp1 += cnt[c[--l]], cnt[c[l]]++;
        while (r < right[p[i]]) tmp1 += cnt[c[++r]], cnt[c[r]]++;
        while (r > right[p[i]]) cnt[c[r]]--, tmp1 -= cnt[c[r]], r--;
        ans[p[i]] = tmp1;
    }

    for (int i = 0; i < m; i++) {
        if (!ans[i]) {
            printf("0/1\n");
        } else {
            tmp2 =
                ((ll)(right[i] - left[i] + 1) * (ll)(right[i] - left[i])) / 2;
            tmp1 = gcd(ans[i], tmp2);
            printf("%lld/%lld\n", ans[i] / tmp1, tmp2 / tmp1);
        }
    }
    return 0;
}