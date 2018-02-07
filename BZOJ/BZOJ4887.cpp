/*
 * created by xehoth 04-05-2017
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}
}  // namespace IO

namespace Task {

int n, m;
const int MAXN = 1005;

std::vector<int> edge[MAXN];

const int MOD = 2017;

int in[MAXN];
int dp[35][35][101];
int ret[35][10001];
int boom[35][10001];
int f[10001];
int top;
int ans;

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v), edge[v].push_back(u);
    in[u]++, in[v]++;
}

inline void solve() {
    using namespace IO;
    read(n), read(m);
    for (register int i = 1, x, y; i <= m; i++) read(x), read(y), addEdge(x, y);

    register int t;
    read(t);
    register int tmp = 1;
    for (; tmp << 1 <= t;) tmp <<= 1, top++;
    for (register int j = 1; j <= n; j++) {
        for (register int i = 0; i < edge[j].size(); i++)
            dp[j][edge[j][i]][0]++;
        dp[j][j][0]++;
    }

    for (register int i = 1; i <= n; i++) boom[i][0] = 1;

    for (register int i = 1; i <= top; i++) {
        for (register int j = 1; j <= n; j++) {
            boom[j][i] = boom[j][i - 1];
            for (register int k = 1; k <= n; k++) {
                boom[j][i] =
                    (boom[j][i] + dp[j][k][i - 1] * boom[k][i - 1]) % MOD;
                for (register int l = 1; l <= n; l++) {
                    dp[j][l][i] =
                        (dp[j][l][i] + dp[j][k][i - 1] * dp[k][l][i - 1]) % MOD;
                }
            }
        }
    }

    register int now = 1 << top;
    for (register int i = 1; i <= n; i++) ret[i][1] = dp[1][i][top];
    f[1] = boom[1][top];

    register int cnt = 1;
    while (now < t) {
        for (register int i = top; i >= 0; i--) {
            if ((1 << i) + now <= t) {
                f[++cnt] = f[cnt - 1];
                for (register int j = 1; j <= n; j++) {
                    f[cnt] = (f[cnt] + ret[j][cnt - 1] * boom[j][i]) % MOD;
                    for (register int k = 1; k <= n; k++)
                        ret[k][cnt] =
                            (ret[k][cnt] + ret[j][cnt - 1] * dp[j][k][i]) % MOD;
                }
                now += (1 << i);
                break;
            }
        }
    }
    for (register int i = 1; i <= n; i++) ans = (ans + ret[i][cnt]) % MOD;
    std::cout << (ans + f[cnt]) % MOD;
}

}  // namespace Task

int main() {
    // freopen("cola.in", "r", stdin);
    // freopen("cola.out", "w", stdout);
    Task::solve();
    return 0;
}
