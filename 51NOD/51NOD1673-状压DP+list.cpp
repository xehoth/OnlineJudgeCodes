/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「51 NOD 1673」树有几多愁 23-10-2017
 * 状压 DP + std::list
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
typedef std::list<State> List;
List list[MAX_STATE + 1];

void dfs(const int u) {
    vis[u] = true, dep[u] = dep[fa[u]] + 1;
    register int cnt = 0;
    for (register Iterator v = edge[u].begin(); v != edge[u].end(); v++) {
        if (!vis[*v]) {
            fa[*v] = u, dfs(*v), cnt++;
            if (cnt == 1) {
                list[u].splice(list[u].end(), list[*v]);
            } else {
                State now = 0;
                for (List::iterator itFa = list[u].begin();
                     itFa != list[u].end(); itFa++) {
                    for (List::iterator it = list[*v].begin();
                         it != list[*v].end(); it++) {
                        g[now = *itFa ^ *it] = g[*itFa] + g[*it] - dep[u];
                        list[0].push_back(now);
                    }
                }
                list[u].splice(list[u].end(), list[*v]);
                list[u].splice(list[u].end(), list[0]);
            }
        }
    }
    if (cnt == 0)
        g[1 << leafNum] = dep[u], list[u].push_back(1 << leafNum), leafNum++;
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