/*
 * created by xehoth on 25-04-2017
 */
#include <bits/stdc++.h>

namespace {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template <class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (iosig = false, c = read(); !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read()) x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), 0) : 0;
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
}  // namespace

const int MOD = 1e9 + 7;

const int MAXN = 200005;

std::vector<int> edge[MAXN];

int edges, n, m, x, y;

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v);
    edges++;
}

std::vector<int> topo;
int in[MAXN], b[MAXN];
long long ans = 1, inv[MAXN], f[MAXN];

inline bool topoSort(int n) {
    std::queue<int> q;
    for (register int i = 1; i <= n; i++)
        if (!in[i]) q.push(i);
    f[y] = ans;
    while (!q.empty()) {
        register int v = q.front();
        q.pop(), topo.push_back(v);
        f[v] = f[v] * inv[b[v]] % MOD;
        for (register int i = 0; i < edge[v].size(); i++) {
            f[edge[v][i]] = (f[edge[v][i]] + f[v]) % MOD;
            edges--;
            if (--in[edge[v][i]] == 0) q.push(edge[v][i]);
        }
    }
    return edges == 0;
}

inline void solve() {
    read(n), read(m), read(x), read(y);
    inv[1] = 1;
    for (register int i = 2; i <= m + 1; i++)
        inv[i] = (-(MOD / i) * inv[MOD % i] % MOD + MOD) % MOD;
    for (register int i = 1, u, v; i <= m; i++)
        read(u), read(v), addEdge(u, v), in[v]++;
    in[y]++;
    memcpy(b, in, sizeof(int) * (n + 1));
    for (register int i = 2; i <= n; i++) ans = ans * in[i] % MOD;
    in[y]--;
    if (y == 1) std::cout << ans, exit(0);
    topoSort(n);
    std::cout << (ans - f[x] + MOD) % MOD;
}

int main() {
    //    freopen("in.in", "r", stdin);
    solve();
    return 0;
}
