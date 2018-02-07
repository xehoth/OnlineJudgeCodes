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

inline void read(char &c) {
    while (c = read(), isspace(c))
        ;
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

inline void print(const char *s) {
    for (; *s; s++) print(*s);
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace IO

namespace ShrinkedGraph {

const int MAXN = 100005;

std::vector<int> edge[MAXN];
int in[MAXN];

typedef std::bitset<2005> BitSet;

BitSet map[2004], comMap[2005];

inline void addEdge(const int u, const int v) { in[v]++, edge[u].push_back(v); }

inline void topoSort(int n) {
    static std::vector<int> st;
    st.reserve(n * 4 + 4);
    for (register int i = 2, r = n << 1 | 1; i <= r; i++)
        if (!in[i]) st.push_back(i);
    while (!st.empty()) {
        register int u = st.back();
        st.pop_back();
        for (register int i = 0, v; i < edge[u].size(); i++) {
            map[v = edge[u][i]] |= map[u];
            if (!(--in[v])) st.push_back(v);
        }
    }
}
}  // namespace ShrinkedGraph

namespace TwoSat {

const int MAXN = 100005;

std::vector<int> edge[MAXN];

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v);
    edge[v ^ 1].push_back(u ^ 1);
}

std::vector<int> st;
int low[MAXN], dfn[MAXN], idx, cnt, inComponent[MAXN];
bool vis[MAXN];

inline void tarjan(const int u) {
    vis[u] = true, st.push_back(u), low[u] = dfn[u] = ++idx;
    register int v;
    for (register int i = 0; i < edge[u].size(); i++) {
        if (!dfn[v = edge[u][i]])
            tarjan(v), low[u] = std::min(low[u], low[v]);
        else if (vis[v])
            low[u] = std::min(low[u], dfn[v]);
    }
    if (dfn[u] == low[u]) {
        cnt++;
        do
            vis[v = st.back()] = false, st.pop_back(), inComponent[v] = cnt;
        while (u != v);
    }
}

inline void solve() {
    using namespace IO;
    register int n, m;
    read(n), read(m);
    for (register int i = 0; i < m; i++) {
        register int u, v;
        register char a, b;
        read(u), read(a), read(v), read(b);
        addEdge(u << 1 | a == 'Y' ^ 1, v << 1 | b == 'Y');
    }
    for (register int i = 2, r = n << 1 | 1; i <= r; i++)
        if (!dfn[i]) tarjan(i);
    for (register int i = 1; i <= n; i++) {
        if (inComponent[i << 1] == inComponent[i << 1 | 1]) {
            print("IMPOSSIBLE\n");
            return;
        }
    }
    for (register int i = 2, r = n << 1 | 1, u, v; i <= r; i++) {
        for (register int j = 0; j < edge[i].size(); j++) {
            if (inComponent[i] != inComponent[edge[i][j]]) {
                ShrinkedGraph::addEdge(inComponent[i], inComponent[edge[i][j]]);
            }
        }
    }
    for (register int i = 2, r = n << 1 | 1; i <= r; i++)
        ShrinkedGraph::map[inComponent[i]].set(i);
    for (register int i = 1; i <= cnt; i++)
        ShrinkedGraph::comMap[i] = ShrinkedGraph::map[i];
    ShrinkedGraph::topoSort(n);
    static int vis[MAXN];
    memset(vis, -1, sizeof(vis));
    for (register int i = 1; i <= cnt; i++) {
        using namespace ShrinkedGraph;
        for (register int j = 1; j <= n; j++) {
            if (comMap[i][j << 1] && map[i][j << 1 | 1])
                vis[j] = 0;
            else if (comMap[i][j << 1 | 1] && map[i][j << 1])
                vis[j] = 1;
        }
    }
    for (register int i = 1; i <= n; i++)
        if (vis[i] == 1)
            print('Y');
        else if (vis[i] == 0)
            print('N');
        else
            print('?');
}
}  // namespace TwoSat

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    TwoSat::solve();
    IO::flush();
    return 0;
}