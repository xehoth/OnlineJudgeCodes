#include <bits/stdc++.h>

#define long long long

const int MAXN = 2000;

int l, r, arr[MAXN], tmp[MAXN], ans, cnt, tot;

inline void dfs(long w) {
    if (w > r) return;
    tmp[++cnt] = w;
    dfs(w * 10 + 2);
    dfs(w * 10 + 9);
}

inline void init() {
    dfs(2), dfs(9);
    std::sort(tmp + 1, tmp + cnt);
    for (register int i = 1, tag = 0; i <= cnt; i++, tag = 0) {
        for (register int j = i - 1; j; j--) {
            if (tmp[i] % tmp[j] == 0) {
                tag = 1;
                break;
            }
        }
        (!tag) ? arr[++tot] = tmp[i] : 0;
    }
    std::reverse(arr + 1, arr + tot + 1);
}

inline int gcd(int a, int b) { return b ? gcd(b, a % b) : a; }

inline void solve(int step, int sz, int w) {
    if (step == tot + 1) {
        if (sz & 1)
            ans += r / w - (l - 1) / w;
        else if (sz)
            ans -= r / w - (l - 1) / w;
    } else {
        solve(step + 1, sz, w);
        register long buf = (long)w / gcd(w, arr[step]) * arr[step];
        if (buf <= r) solve(step + 1, sz + 1, buf);
    }
}

int main() {
    scanf("%d%d", &l, &r);
    init();
    solve(1, 0, 1);
    printf("%d\n", ans);
    return 0;
}
