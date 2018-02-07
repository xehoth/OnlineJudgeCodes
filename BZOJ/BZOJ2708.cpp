#include <bits/stdc++.h>

namespace {

const int MAXN = 50;

int f[MAXN + 1], a[MAXN + 1], n;

inline int calc(int x, int y) {
    for (register int k = 1; k <= y - x + 1; k++) {
        for (register int j = x; j <= y - k; j++)
            if (abs(a[j] - a[j + k]) > 1) return k - 1;
        if (abs(a[x + k - 1] - a[y - k + 1]) <= 1) return k - 1;
    }
    return y - x + 1;
}

inline void solve() {
    while (std::cin >> n) {
        memset(f, 0, sizeof(f));
        for (register int i = 1; i <= n; i++) std::cin >> a[i];
        std::sort(a + 1, a + n + 1);
        for (register int i = 1; i <= n; i++)
            for (register int j = 0; j < i; j++)
                f[i] = std::max(f[i], f[j] + calc(j + 1, i));
        std::cout << f[n] << '\n';
    }
}
}  // namespace

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    solve();
    return 0;
}
