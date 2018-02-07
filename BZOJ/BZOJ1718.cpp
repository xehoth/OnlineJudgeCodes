#include <bits/stdc++.h>
#define FAST_IO
#ifdef FAST_IO
const int IN_LEN = 100, OUT_LEN = 100;
inline int nextChar() {
    static char buf[IN_LEN], *h, *t;
    if (h == t) {
        t = (h = buf) + fread(buf, 1, IN_LEN, stdin);
        if (h == t) return -1;
    }
    return *h++;
}
template <class T>
inline bool read(T &x) {
    static bool iosig = 0;
    static char c;
    for (iosig = 0, c = nextChar(); !isdigit(c); c = nextChar()) {
        if (c == -1) return false;
        if (c == '-') iosig = 1;
    }
    for (x = 0; isdigit(c); c = nextChar()) x = (x << 1) + (x << 3) + (c ^ '0');
    if (iosig) x = -x;
    return true;
}
char obuf[OUT_LEN], *oh = obuf;
inline void writeChar(const char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}
template <class T>
inline void write(T x) {
    static int buf[30], cnt;
    if (!x)
        writeChar(48);
    else {
        if (x < 0) writeChar('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) writeChar(buf[cnt--]);
    }
}
inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
#endif
const int MAXN = 5005;
std::vector<int> edge[MAXN];
std::stack<int> st;
int dfn[MAXN], low[MAXN], idx, id[MAXN], scc;
inline void init() {
    while (!st.empty()) st.pop();
    idx = 0, scc = 1;
    memset(id, -1, sizeof(id));
    memset(edge, 0, sizeof(edge));
    memset(dfn, -1, sizeof(dfn));
    memset(low, -1, sizeof(low));
}
inline void addEdge(const int u, const int v) {
    edge[u].push_back(v);
    edge[v].push_back(u);
}
inline void tarjan(int u, int fa) {
    dfn[u] = low[u] = ++idx, st.push(u);
    bool flag = 0;
    for (register int i = 0; i < edge[u].size(); i++) {
        register int v = edge[u][i];
        if (v == fa && !flag) {
            flag = 1;
            continue;
        }
        if (dfn[v] == -1)
            tarjan(v, u), low[u] = std::min(low[u], low[v]);
        else
            low[u] = std::min(low[u], dfn[v]);
    }
    if (low[u] == dfn[u]) {
        register int v;
        do
            v = st.top(), id[v] = scc, st.pop();
        while (!st.empty() && v != u);
        scc++;
    }
}
inline void solve(const int n, int &ans) {
    register int u, v;
    static int deg[MAXN];
    memset(deg, 0, sizeof(deg));
    for (register int i = 1; i <= n; i++) {
        for (register int j = 0; j < edge[i].size(); j++) {
            u = i, v = edge[i][j];
            if (id[u] == id[v])
                continue;
            else
                deg[id[v]]++, deg[id[u]]++;
        }
    }
    register int sum = 0;
    for (register int i = 1; i < scc; i++)
        if (deg[i] == 2) sum++;
    ans = sum + 1 >> 1;
}
int main() {
#ifndef ONLINE_JUDGE
    freopen("in.in", "r", stdin);
#endif
    register int n, m;
    while (read(n), read(m)) {
        init();
        register int u, v;
        for (register int i = 0; i < m; i++) read(u), read(v), addEdge(u, v);
        tarjan(1, 1);
        register int ans;
        solve(n, ans);
        write(ans), writeChar('\n');
    }
    flush();
    return 0;
}
