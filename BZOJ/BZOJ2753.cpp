/*
 * created by xehoth on 01-03-2017
 */
#include <bits/stdc++.h>

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    if (s == t) {
        t = (s = buf) + fread(buf, 1, IN_LEN, stdin);
        if (s == t) return -1;
    }
    return *s++;
}

template <class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        if (c == '-') iosig = true;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    if (iosig) x = -x;
}

const int OUT_LEN = 100000;
char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}

template <class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        if (x < 0) print('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

const int MAXN = 1e5 + 10;

#define long long long

int fa[MAXN], h[MAXN];

inline int get(int x) { return fa[x] == x ? x : fa[x] = get(fa[x]); }

struct Edge {
    int u, v, w;

    Edge(int u, int v, int w) : u(u), v(v), w(w) {}

    inline bool operator<(const Edge &x) const {
        return h[v] > h[x.v] || h[v] == h[x.v] && w < x.w;
    }
};

std::vector<int> g[MAXN];
std::vector<Edge> edge, dfsEdge;

inline void addEdge(int u, int v, int w) {
    edge.push_back(Edge(u, v, w));
    g[u].push_back(edge.size() - 1);
}

bool vis[MAXN];

inline void dfs(int u) {
    static std::vector<int> st;
    st.push_back(u);
    while (!st.empty()) {
        u = st.back(), st.pop_back();
        for (register int i = 0; i < g[u].size(); i++) {
            Edge *e = &edge[g[u][i]];
            dfsEdge.push_back(*e);
            if (!vis[e->v]) vis[e->v] = true, st.push_back(e->v);
        }
    }
}

int main() {
    // freopen("in.in", "r", stdin);
    register int n, m;
    read(n), read(m);

    for (register int i = 1; i <= n; i++) read(h[i]);
    for (register int i = 0, u, v, w; i < m; i++) {
        read(u), read(v), read(w);
        if (h[u] < h[v]) std::swap(u, v);
        addEdge(u, v, w);
        if (h[u] == h[v]) addEdge(v, u, w);
    }

    vis[1] = true, dfs(1);
    std::sort(dfsEdge.begin(), dfsEdge.end());
    for (register int i = 1; i <= n; i++) fa[i] = i;

    long ans = 0;
    register int cnt = 0;
    for (register int i = 0; i < dfsEdge.size(); i++) {
        Edge *e = &dfsEdge[i];
        register int u = get(e->u), v = get(e->v);
        if (u != v) ans += e->w, cnt++, fa[v] = u;
    }
    print(cnt + 1), print(' '), print(ans);
    flush();
    return 0;
}