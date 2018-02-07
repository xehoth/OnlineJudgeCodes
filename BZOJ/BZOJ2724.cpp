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

int n, m, block;
int v[50005], bl[50005];
int f[505][505];
int val[50005], cnt[50005];
std::vector<int> data[50005];

inline void init(int x) {
    memset(cnt, 0, sizeof(cnt));
    int max = 0, ans = 0;
    for (int i = (x - 1) * block + 1; i <= n; i++) {
        cnt[v[i]]++;
        int t = bl[i];
        if (cnt[v[i]] > max || (cnt[v[i]] == max && val[v[i]] < val[ans]))
            ans = v[i], max = cnt[v[i]];
        f[x][t] = ans;
    }
}

int query(int l, int r, int x) {
    int t = std::upper_bound(data[x].begin(), data[x].end(), r) -
            std::lower_bound(data[x].begin(), data[x].end(), l);
    return t;
}

int query(int a, int b) {
    int ans = f[bl[a] + 1][bl[b] - 1], max = query(a, b, ans);

    for (int i = a; i <= std::min(bl[a] * block, b); i++) {
        int t = query(a, b, v[i]);
        if (t > max || (t == max && val[v[i]] < val[ans])) ans = v[i], max = t;
    }
    if (bl[a] != bl[b]) {
        for (int i = (bl[b] - 1) * block + 1; i <= b; i++) {
            int t = query(a, b, v[i]);
            if (t > max || (t == max && val[v[i]] < val[ans]))
                ans = v[i], max = t;
        }
    }
    return ans;
}

int main() {
    // freopen("in.in", "r", stdin);
    static std::map<int, int> mp;

    n = read(), m = read();
    block = 200;
    int ans = 0, id = 0;
    for (int i = 1; i <= n; i++) {
        v[i] = read();
        if (!mp[v[i]]) {
            mp[v[i]] = ++id;
            val[id] = v[i];
        }
        v[i] = mp[v[i]];
        data[v[i]].push_back(i);
    }
    for (int i = 1; i <= n; i++) bl[i] = (i - 1) / block + 1;

    for (int i = 1; i <= bl[n]; i++) init(i);
    for (int i = 1; i <= m; i++) {
        int a = read(), b = read();
        a = (a + ans - 1) % n + 1, b = (b + ans - 1) % n + 1;
        if (a > b) std::swap(a, b);
        ans = val[query(a, b)];
        printf("%d\n", ans);
    }
    return 0;
}