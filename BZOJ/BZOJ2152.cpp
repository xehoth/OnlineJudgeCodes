#include <bits/stdc++.h>
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
const int MAXN = 20000 + 10;
struct Node {
    int v, w;
    Node(int v, int w) : v(v), w(w) {}
};
std::vector<Node> edge[MAXN];
inline void addEdge(int u, int v, int w) {
    edge[u].push_back(Node(v, w));
    edge[v].push_back(Node(u, w));
}
int n, size, son[MAXN], f[MAXN], root, dis[MAXN], ans;
int count[5];
bool vis[MAXN];
inline void getRoot(int u, int father) {
    son[u] = 1, f[u] = 0;
    for (register int i = 0, v; i < edge[u].size(); i++)
        if ((v = edge[u][i].v) != father && !vis[v])
            getRoot(v, u), son[u] += son[v], f[u] = std::max(f[u], son[v]);
    f[u] = std::max(f[u], size - son[u]);
    if (f[u] < f[root]) root = u;
}
inline void getDeep(int u, int father) {
    count[dis[u]]++;
    for (register int i = 0, v; i < edge[u].size(); i++)
        if ((v = edge[u][i].v) != father && !vis[v])
            dis[v] = (dis[u] + edge[u][i].w) % 3, getDeep(v, u);
}
inline int calc(int u, int init) {
    count[0] = count[1] = count[2] = 0, dis[u] = init, getDeep(u, 0);
    return count[1] * count[2] * 2 + count[0] * count[0];
}
inline void work(int u) {
    ans += calc(u, 0), vis[u] = true;
    for (register int i = 0, v; i < edge[u].size(); i++)
        if (!vis[v = edge[u][i].v])
            ans -= calc(v, edge[u][i].w), f[0] = size = son[v],
                                          getRoot(v, root = 0), work(root);
}
inline int gcd(int a, int b) { return !b ? a : gcd(b, a % b); }
int main() {
    read(n);
    for (register int i = 1, u, v, w; i < n; i++)
        read(u), read(v), read(w), addEdge(u, v, w % 3);
    f[0] = size = n;
    getRoot(1, 0);
    work(root);
    register int t = gcd(ans, n * n);
    std::cout << ans / t << "/" << n * n / t;
    return 0;
}
