#include <algorithm>
#include <cstdio>

using namespace std;

const int maxn = 100005;

int n, dp[maxn], size[maxn], belong[maxn], next[maxn], to[maxn], dfn[maxn],
    low[maxn], clo, tot;
bool ins[maxn];

inline int iread() {
    int f = 1, x = 0;
    char ch = getchar();
    for (; ch < '0' || ch > '9'; ch = getchar()) f = ch == '-' ? -1 : 1;
    for (; ch >= '0' && ch <= '9'; ch = getchar()) x = x * 10 + ch - '0';
    return f * x;
}

int sta[maxn], top;

inline void tarjan(int x) {
    dfn[x] = low[x] = ++clo;
    ins[sta[++top] = x] = 1;
    if (!dfn[next[x]])
        tarjan(next[x]), low[x] = min(low[x], low[next[x]]);
    else if (ins[next[x]])
        low[x] = min(low[x], dfn[next[x]]);
    if (low[x] == dfn[x]) {
        tot++;
        while (1) {
            int u = sta[top--];
            belong[u] = tot;
            size[tot]++;
            ins[u] = 0;
            if (u == x) break;
        }
    }
}

inline int dfs(int x) {
    if (dp[x]) return dp[x];
    dp[x] = size[x];
    if (to[x]) dp[x] += dfs(to[x]);
    return dp[x];
}

int main() {
    n = iread();
    for (int i = 1; i <= n; i++) next[i] = iread();
    for (int i = 1; i <= n; i++)
        if (!dfn[i]) tarjan(i);
    for (int i = 1; i <= n; i++)
        if (belong[i] != belong[next[i]]) to[belong[i]] = belong[next[i]];
    for (int i = 1; i <= n; i++) printf("%d\n", dfs(belong[i]));
    return 0;
}