#include <bits/stdc++.h>

const int MAXN = 100000;
const int MAXK = 100;
int f[MAXN + 1][MAXK + 1];
long long n, k, c;
int T;

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(NULL), std::cout.tie(NULL);
    std::cin >> n >> k;
    for (register int i = 2;; i++) {
        f[i][0] = 0;
        for (register int j = 2; j <= k; j++) {
            f[i][j] = std::max(f[i - 1][j] + f[i - 2][j - 2],
                               f[i - 2][j] + f[i - 1][j - 1]) +
                      1;
        }
        if (f[i][k] >= n) {
            std::cout << i << '\n';
            break;
        }
    }
    return 0;
}