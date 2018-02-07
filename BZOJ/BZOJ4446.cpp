#include <algorithm>
#include <cstdio>
#include <cstdlib>

const int MAXN = 200000;
const int MAXLOGN = 18;

int n, l[MAXN + 1];
long long a[MAXN + 1], b[MAXN + 1], d[MAXN + 1], f[MAXN + 1][MAXLOGN + 1],
    g[MAXN + 1][MAXLOGN + 1];

inline void dp() {
    for (int x = n; x; x--) {
        for (int i = l[x] - 1; i >= 0; i--) {
            int lchild = x << 1, rchild = lchild + 1,
                target = x >> (l[x] - i - 1) ^ 1;
            if (lchild > n) {
                f[x][i] =
                    a[target] * (d[x] + d[target] - (d[target >> 1] << 1));
            } else if (rchild > n) {
                f[x][i] = f[lchild][i] + a[lchild] * b[lchild];
            } else {
                f[x][i] = std::min(
                    a[lchild] * b[lchild] + f[lchild][l[x]] + f[rchild][i],
                    a[rchild] * b[rchild] + f[rchild][l[x]] + f[lchild][i]);
            }
        }
    }

    for (int x = n; x; x--) {
        for (int i = l[x]; i >= 0; i--) {
            int lchild = x << 1, rchild = lchild + 1, target = x >> (l[x] - i);
            if (lchild > n) {
                g[x][i] = a[target] * (d[x] - d[target]);
            } else if (rchild > n) {
                g[x][i] = g[lchild][i] + a[lchild] * b[lchild];
            } else {
                g[x][i] = std::min(
                    a[lchild] * b[lchild] + f[lchild][l[x]] + g[rchild][i],
                    a[rchild] * b[rchild] + f[rchild][l[x]] + g[lchild][i]);
            }
        }
    }
}

inline long long calc(int x) {
    long long result = g[x][l[x] - 1];
    for (; x != 1; x >>= 1) {
        int brother = x ^ 1, parent = x >> 1;
        if (brother > n) {
            result += a[parent >> 1] * b[parent];
        } else {
            result += a[brother] * b[brother] + g[brother][l[parent] - 1];
        }
    }

    return result;
}

inline long long solve() {
    long long ans = g[1][0];
    for (int i = 2; i <= n; i++) ans = std::min(ans, calc(i));
    return ans;
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%lld", &a[i]);
    }

    for (int i = 2; i <= n; i++) {
        scanf("%lld", &b[i]);
    }

    l[1] = 1, d[1] = 0;
    for (int i = 2; i <= n; i++) {
        l[i] = l[i >> 1] + 1;
        d[i] = d[i >> 1] + b[i];
    }

    dp();
    printf("%lld\n", solve());

    return 0;
}