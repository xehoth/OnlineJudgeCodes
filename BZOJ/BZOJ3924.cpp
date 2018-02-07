#include <bits/stdc++.h>
const int MAXN = 100100;
inline char read() {
    static const int IO_LEN = 1024 * 1024;
    static char buf[IO_LEN], *ioh, *iot;
    if (ioh == iot) {
        iot = (ioh = buf) + fread(buf, 1, IO_LEN, stdin);
        if (ioh == iot) return -1;
    }
    return *ioh++;
}
template <class T>
inline void read(T &x) {
    static char ioc;
    static bool iosig = 0;
    for (iosig = 0, ioc = read(); !isdigit(ioc); ioc = read())
        if (ioc == '-') iosig = 1;
    for (x = 0; isdigit(ioc); ioc = read())
        x = (x << 1) + (x << 3) + (ioc ^ '0');
    if (iosig) x = -x;
}
typedef long long ll;
struct Node {
    int v, w;
    Node(int v, int w) : v(v), w(w) {}
};
std::vector<Node> edge[MAXN];
inline void addEdge(int u, int v, int w) {
    edge[u].push_back(Node(v, w));
    edge[v].push_back(Node(u, w));
}
bool vis[MAXN];
int s[MAXN], f[MAXN], root, size;
inline void dfs(int u, int fa) {
    s[u] = 1;
    f[u] = 0;
    int v = 0;
    for (register int i = 0; i < edge[u].size(); i++) {
        if (vis[v = edge[u][i].v] || v == fa) continue;
        dfs(v, u);
        s[u] += s[v];
        f[u] = std::max(f[u], s[v]);
    }
    f[u] = std::max(f[u], size - s[u]);
    if (f[u] < f[root]) root = u;
}
std::vector<std::pair<ll, ll> > pre[MAXN];
inline void getdist(int u, int fa, int tar, int dist) {
    pre[u].push_back(std::make_pair(tar, dist));
    s[u] = 1;
    int v = 0;
    for (register int i = 0; i < edge[u].size(); i++) {
        if (vis[v = edge[u][i].v] || v == fa) continue;
        getdist(v, u, tar, dist + edge[u][i].w);
        s[u] += s[v];
    }
}
std::vector<std::pair<ll, std::pair<ll, ll> > > ch[MAXN];
inline void work(int u) {
    vis[u] = 1;
    int v = 0;
    pre[u].push_back(std::make_pair(u, 0));
    for (int i = 0; i < edge[u].size(); i++) {
        if (vis[v = edge[u][i].v]) continue;
        getdist(v, 0, u, edge[u][i].w);
        f[0] = size = s[v];
        dfs(v, root = 0);
        ch[u].push_back(std::make_pair(root, std::make_pair(v, edge[u][i].w)));
        work(root);
    }
}
ll cnt[MAXN], sum[MAXN];
std::vector<ll> sumdist[MAXN];
inline void update(int x, ll y, ll z) {
    for (int i = 0; i < pre[x].size(); i++) {
        int u = pre[x][i].first;
        cnt[u] += y;
        sum[u] += z + y * pre[x][i].second;
        if (i != pre[x].size() - 1) {
            int j = 0;
            for (; j < ch[u].size(); j++)
                if (ch[u][j].first == pre[x][i + 1].first)
                    sumdist[u][j] += z + y * pre[x][i].second;
        }
    }
}
int realroot;
std::vector<std::pair<ll, std::pair<ll, ll> > > record;
inline ll query() {
    int x = realroot;
    int mx = 0;
    record.clear();
    while (x) {
        mx = 0;
        for (int i = 1; i < ch[x].size(); i++)
            if (cnt[ch[x][mx].first] < cnt[ch[x][i].first]) mx = i;
        if (ch[x].size() == 0 || cnt[ch[x][mx].first] * 2 <= cnt[x]) {
            ll ans = sum[x];
            for (int i = 0; i < record.size(); i++)
                update(record[i].first, record[i].second.first,
                       record[i].second.second);
            return ans;
        }
        int v = ch[x][mx].first;
        record.push_back(std::make_pair(
            ch[x][mx].second.first,
            std::make_pair(-(cnt[x] - cnt[v]),
                           -(sum[x] - sumdist[x][mx] +
                             (cnt[x] - cnt[v]) * ch[x][mx].second.second))));
        update(ch[x][mx].second.first, cnt[x] - cnt[v],
               sum[x] - sumdist[x][mx] +
                   (cnt[x] - cnt[v]) * ch[x][mx].second.second);
        x = v;
    }
}
int main() {
    int n, Q;
    read(n), read(Q);
    for (register int i = 1, u, v, w; i < n; i++)
        read(u), read(v), read(w), addEdge(u, v, w);
    f[0] = size = n;
    dfs(1, root = 0);
    realroot = root;
    work(root);
    for (register int i = 1; i <= n; i++)
        sumdist[i] = std::vector<ll>(ch[i].size(), 0);
    while (Q--) {
        register int x, y;
        read(x), read(y);
        update(x, y, 0);
        std::cout << query() << "\n";
    }
    return 0;
}