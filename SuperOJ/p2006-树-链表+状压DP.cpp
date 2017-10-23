/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「SuperOJ 2006」树 23-10-2017
 * 链表 + 状压 DP
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

struct InputOutputStream {
    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            c == '-' ? iosig = true : 0;
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        return iosig ? x = -x : 0, *this;
    }
} io;
}

namespace {
using IO::io;

const int MAXN = 100000;
const int MAX_STATE = 1 << 20;
const int MOD = 1000000007;

std::vector<int> edge[MAXN + 1];
bool vis[MAXN + 1];
int fa[MAXN + 1], dep[MAXN + 1], g[MAX_STATE + 1], leafNum;
typedef std::vector<int>::iterator Iterator;
typedef int State;

struct Node {
    int begin, end;
} list[MAX_STATE + 1];

int next[MAX_STATE + 1];

void dfs(const int u) {
    vis[u] = true, dep[u] = dep[fa[u]] + 1;
    register int cnt = 0;
    for (register Iterator v = edge[u].begin(); v != edge[u].end(); v++) {
        if (!vis[*v]) {
            fa[*v] = u, dfs(*v), cnt++;
            if (cnt == 1) {
                list[u].begin = list[*v].begin, list[u].end = list[*v].end;
            } else {
                State now = 0;
                for (int itFa = list[u].begin; itFa; itFa = next[itFa]) {
                    for (int it = list[*v].begin; it; it = next[it]) {
                        g[itFa ^ it] = g[itFa] + g[it] - dep[u];
                        next[now] = itFa ^ it, now = itFa ^ it;
                    }
                }
                next[list[u].end] = list[*v].begin;
                next[list[*v].end] = next[0], list[u].end = now;
            }
        }
    }
    if (cnt == 0) {
        g[1 << leafNum] = dep[u];
        list[u].begin = list[u].end = 1 << leafNum, leafNum++;
    }
}

typedef std::pair<double, long long> Pair;
Pair f[MAX_STATE + 1];

inline void solve() {
    register int n;
    io >> n;
    for (register int i = 1, u, v; i < n; i++)
        io >> u >> v, edge[u].push_back(v), edge[v].push_back(u);
    dfs(1);
    register State maxState = 1 << leafNum;
    register double tmp;
    for (register State s = 0; s < maxState; s++) {
        f[s].second = 1, f[s].first = 1;
        for (register State i = 0; i < leafNum; i++) {
            if (((s >> i) & 1) &&
                f[s].first < (tmp = f[s ^ (1 << i)].first * (n - g[s] + 1))) {
                f[s].first = tmp;
                f[s].second = f[s ^ (1 << i)].second * (n - g[s] + 1) % MOD;
            }
        }
    }
    std::cout << f[maxState - 1].second;
}
}

int main() {
    solve();
    return 0;
}