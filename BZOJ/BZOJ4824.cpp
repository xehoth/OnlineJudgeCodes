/*
 * created by xehoth on 29-04-2017
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

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = '\0';
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = '\0';
    return s;
}
}  // namespace IO

namespace Dp {

const int MAXN = 110;
const int MOD = 1000000007;

#define long long long

long c[MAXN][MAXN];

std::vector<int> edge[MAXN << 1];

char s[MAXN];

inline void addEdge(const int u, const int v) { edge[u].push_back(v); }

inline void init(const int n) {
    for (register int i = 0; i <= n; i++) c[i][0] = 1;
    for (register int i = 1; i <= n; i++) {
        for (register int j = 1; j <= i; j++) {
            (c[i][j] = c[i - 1][j] + c[i - 1][j - 1]) >= MOD ? c[i][j] -= MOD
                                                             : 0;
        }
    }
}

long size[MAXN], tmp[MAXN];

long dp[MAXN][MAXN], g[MAXN][MAXN];

inline void dfs(int x) {
    size[x] = dp[x][1] = g[x][1] = 1;
    register int v;
    for (register int i = 0; i < edge[x].size(); i++) {
        v = edge[x][i];
        dfs(v);
        memset(tmp, 0, sizeof(long) * (size[x] + size[v] + 1));
        for (register int i = 1; i <= size[x]; i++) {
            for (register int j = 0; j <= size[v]; j++) {
                if (s[v] == '<') {
                    tmp[i + j] += dp[x][i] * (g[v][size[v]] - g[v][j] + MOD) %
                                  MOD * c[i + j - 1][i - 1] % MOD *
                                  c[size[x] + size[v] - i - j][size[x] - i] %
                                  MOD;
                } else {
                    tmp[i + j] +=
                        dp[x][i] * g[v][j] % MOD * c[i + j - 1][i - 1] % MOD *
                        c[size[x] + size[v] - i - j][size[x] - i] % MOD;
                }
            }
        }
        size[x] += size[v];
        for (register int i = 1; i <= size[x]; i++)
            dp[x][i] = tmp[i], g[x][i] = (g[x][i - 1] + tmp[i]) % MOD;
    }
}

inline void solve() {
    using namespace IO;
    register int n;
    read(n), init(n);
    read(s + 2);
    for (register int i = 1; i <= n; i++) {
        (i << 1) <= n ? (addEdge(i, i << 1), 0) : 0;
        (i << 1 | 1) <= n ? (addEdge(i, i << 1 | 1), 0) : 0;
    }
    dfs(1);
    std::cout << g[1][size[1]];
}

}  // namespace Dp

int main() {
    // freopen("in.in", "r", stdin);
    Dp::solve();
    return 0;
}
