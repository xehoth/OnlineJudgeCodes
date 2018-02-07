#include <bits/stdc++.h>

namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <typename T>
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

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace IO

const int MAXN = 3e5 + 10;

int n, m;
int w[MAXN];
std::vector<int> edge[MAXN];
int queryS[MAXN], queryT[MAXN];
int ans[MAXN], idx, pre[MAXN], suf[MAXN], deep[MAXN];

struct Query {
    int pos, id;
};

Query queryStart[MAXN];
std::vector<Query> queryEnd[MAXN];
std::vector<int> addTag[MAXN], subTag[MAXN];

inline void solve() {
    register int zero = MAXN * 2;
    static int a[MAXN * 4];
    for (register int k = 1; k <= n; k++) {
        for (register int i = 0; i < subTag[k].size(); i++)
            a[zero + subTag[k][i]]--;
        ans[queryStart[k].id] -= a[zero + queryStart[k].pos];
        for (register int i = 0; i < addTag[k].size(); i++)
            a[zero + addTag[k][i]]++;
        for (register int i = 0; i < queryEnd[k].size(); i++) {
            Query &q = queryEnd[k][i];
            ans[q.id] += a[zero + q.pos];
        }
    }
}

inline void dfs(int u, int father) {
    pre[u] = suf[u] = ++idx;
    deep[u] = deep[father] + 1;
    for (register int i = 0, v; i < edge[u].size(); i++) {
        v = edge[u][i];
        if (v == father) continue;
        dfs(v, u);
        suf[u] = suf[v];
    }
}

int lca[MAXN], father[MAXN];
bool vis[MAXN];
std::vector<int> pairs[MAXN];

inline int get(int i) {
    return father[i] == i ? i : father[i] = get(father[i]);
}

inline void tarjan(int u, int father) {
    vis[u] = true;
    for (register int i = 0; i < pairs[u].size(); i++) {
        register int id = pairs[u][i], t = 0;
        if (queryS[id] == u) t = queryT[id];
        if (queryT[id] == u) t = queryS[id];
        if (t && vis[t]) lca[id] = get(t);
    }
    for (register int i = 0; i < edge[u].size(); i++) {
        register int v = edge[u][i];
        if (v == father) continue;
        tarjan(v, u);
        ::father[v] = u;
    }
}

int main() {
    using namespace IO;
    read(n), read(m);
    for (register int i = 1, u, v; i < n; i++)
        read(u), read(v), edge[u].push_back(v), edge[v].push_back(u);
    for (int i = 1; i <= n; i++) read(w[i]);
    for (int i = 1; i <= m; i++)
        read(queryS[i]), read(queryT[i]), pairs[queryS[i]].push_back(i),
            pairs[queryT[i]].push_back(i);
    dfs(1, 0);
    for (int i = 1; i <= n; i++) father[i] = i;
    tarjan(1, 0);
    for (register int i = 1; i <= m; i++)
        if (w[lca[i]] == deep[queryS[i]] - deep[lca[i]]) ans[lca[i]]--;
    for (register int i = 1; i <= n; i++)
        queryStart[pre[i]] = (Query){deep[i] + w[i], i},
        queryEnd[suf[i]].push_back((Query){deep[i] + w[i], i});
    for (register int i = 1; i <= m; i++) {
        register int s = queryS[i], t = lca[i];
        addTag[pre[s]].push_back(deep[s]);
        subTag[pre[t]].push_back(deep[s]);
    }
    solve();
    memset(queryStart, 0, sizeof(queryStart));
    for (register int i = 0; i < MAXN; i++)
        queryEnd[i].clear(), addTag[i].clear(), subTag[i].clear();
    for (register int i = 1; i <= n; i++)
        queryStart[pre[i]] = (Query){w[i] - deep[i], i},
        queryEnd[suf[i]].push_back((Query){w[i] - deep[i], i});
    for (register int i = 1; i <= m; i++) {
        register int s = lca[i], t = queryT[i],
                     dis = deep[queryS[i]] + deep[queryT[i]] -
                           (deep[lca[i]] << 1);
        addTag[pre[t]].push_back(dis - deep[t]);
        subTag[pre[s]].push_back(dis - deep[t]);
    }
    solve();
    for (register int i = 1; i < n; i++) print(ans[i]), print(' ');
    print(ans[n]), flush();
    return 0;
}