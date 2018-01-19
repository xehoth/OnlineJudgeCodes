/**
 * Copyright (c) 2017-2018, xehoth
 * All rights reserved.
 * 「BZOJ 2566」xmastree 18-01-2018
 * 点分树
 * @author xehoth
 */
#include <bits/stdc++.h>

namespace {

inline char read() {
    static const int IN_LEN = 1 << 18 | 1;
    static char buf[IN_LEN], *s, *t;
    return (s == t) && (t = (s = buf) + fread(buf, 1, IN_LEN, stdin)),
           s == t ? -1 : *s++;
}

const int OUT_LEN = 1 << 18 | 1;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    (oh == obuf + OUT_LEN) && (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf);
    *oh++ = c;
}

template <typename T>
inline void print(T x) {
    static int buf[21], cnt;
    if (x != 0) {
        (x < 0) && (print('-'), x = -x);
        for (cnt = 0; x != 0; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt != 0) print((char)buf[cnt--]);
    } else {
        print('0');
    }
}

struct InputOutputStream {
    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        iosig && (x = -x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 12000 + 1;
const int MAX_LOG = 15;

struct Node {
    int v, w;

    Node(int v, int w) : v(v), w(w) {}
};

std::vector<Node> edge[MAXN];
typedef std::vector<Node>::iterator Iterator;

inline void addEdge(const int u, const int v, const int w) {
    edge[u].push_back(Node(v, w));
    edge[v].push_back(Node(u, w));
}

bool vis[MAXN];
int color[MAXN], sz[MAXN], center[MAXN][MAX_LOG], dis[MAXN][MAX_LOG];

typedef std::pair<int, int> Pair;
std::multiset<Pair> info[MAXN];
std::multiset<int> distance;

void dfsSize(int u, int pre) {
    sz[u] = 1;
    for (Iterator p = edge[u].begin(); p != edge[u].end(); ++p) {
        if (!vis[p->v] && p->v != pre) {
            dfsSize(p->v, u);
            sz[u] += sz[p->v];
        }
    }
}

int get(int u, int pre, int size) {
    for (Iterator p = edge[u].begin(); p != edge[u].end(); ++p)
        if (!vis[p->v] && p->v != pre && sz[p->v] > size)
            return get(p->v, u, size);
    return u;
}

void dfs(int u, int pre, int level, int root) {
    info[root].insert(Pair(color[u], dis[u][level]));
    center[u][level] = root;
    for (Iterator p = edge[u].begin(); p != edge[u].end(); ++p) {
        if (!vis[p->v] && p->v != pre) {
            dis[p->v][level] = dis[u][level] + p->w;
            dfs(p->v, u, level, root);
        }
    }
}

void build(int u, int level) {
    dfsSize(u, 0);
    u = get(u, 0, sz[u] / 2);
    dfs(u, 0, level, u);
    vis[u] = true;
    for (Iterator p = edge[u].begin(); p != edge[u].end(); ++p)
        if (!vis[p->v]) build(p->v, level + 1);
}

inline int findMinDis(int root, int color) {
    std::multiset<Pair>::iterator begin, next;
    begin = info[root].lower_bound(Pair(color, 0));
    next = begin;
    if (begin == info[root].end() || begin->first != color) return -1;
    if (++next == info[root].end()) return -1;
    if (begin->first != next->first) return -1;
    return begin->second + next->second;
}

inline void erase(int root, int color, int dist) {
    std::multiset<Pair>::iterator it =
        info[root].lower_bound(Pair(color, dist));
    info[root].erase(it);
}

inline void insert(int root, int color, int dist) {
    info[root].insert(Pair(color, dist));
}

inline void solve() {
    int n;
    io >> n;
    for (int i = 1; i <= n; i++) io >> color[i];
    for (int i = 1, u, v, w; i < n; i++) {
        io >> u >> v >> w;
        addEdge(u, v, w);
    }
    build(1, 0);
    for (int i = 1, preColor; i <= n; i++) {
        preColor = -1;
        typedef std::multiset<Pair>::iterator It;
        for (It begin = info[i].begin(), end = info[i].end(); begin != end;
             ++begin) {
            if (begin->first != preColor) {
                preColor = begin->first;
                It next = begin;
                ++next;
                if (next != end && next->first == begin->first) {
                    distance.insert(next->second + begin->second);
                    begin = next;
                }
            }
        }
    }
    io << (distance.empty() ? -1 : *distance.begin()) << '\n';
    int q;
    io >> q;
    for (int u, w; q--;) {
        io >> u >> w;
        for (int j = 0; center[u][j]; ++j) {
            int v = findMinDis(center[u][j], color[u]);
            if (v != -1) distance.erase(distance.find(v));
            erase(center[u][j], color[u], dis[u][j]);

            v = findMinDis(center[u][j], color[u]);
            if (v != -1) distance.insert(v);

            v = findMinDis(center[u][j], w);
            if (v != -1) distance.erase(distance.find(v));
            insert(center[u][j], w, dis[u][j]);

            v = findMinDis(center[u][j], w);
            if (v != -1) distance.insert(v);
        }
        color[u] = w;
        io << (distance.empty() ? -1 : *distance.begin()) << '\n';
    }
}
}  // namespace

int main() {
    // freopen("sample/1.in", "r", stdin);
    solve();
    return 0;
}