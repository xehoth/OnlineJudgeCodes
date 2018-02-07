#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

inline char read() {
    static const int IN_LEN = 1024 * 1024;
    static char buf[IN_LEN], *s, *t;
    if (s == t) {
        t = (s = buf) + fread(buf, 1, IN_LEN, stdin);
        if (s == t) return -1;
    }
    return *s++;
}

///*
template <class T>
inline void R(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        if (c == '-') iosig = true;
    }
    for (x = 0; isdigit(c); c = read()) x = ((x << 2) + x << 1) + (c ^ '0');
    if (iosig) x = -x;
}
//*/

const int OUT_LEN = 1024 * 1024;
char obuf[OUT_LEN], *oh = obuf;
inline void write_char(char c) {
    if (oh == obuf + OUT_LEN) fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf;
    *oh++ = c;
}

template <class T>
inline void W(T x) {
    static int buf[30], cnt;
    if (x == 0)
        write_char('0');
    else {
        if (x < 0) write_char('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) write_char(buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }

const int MAXN = 1000 + 10;

int n, m, k, x, y;
std::vector<int> edge[MAXN];
int exist[MAXN][MAXN], vis[MAXN];
int c[MAXN], P;

inline void add_edge(int x, int y) {
    edge[x].push_back(y);
    edge[y].push_back(x);
}

inline void read_in() {
    R(n), R(m), R(k);
    for (int i = 1; i <= n; ++i) edge[i].clear();
    for (int i = 1; i <= m; ++i) {
        R(x), R(y), add_edge(x, y);
    }
}

inline bool dfs(int cur) {
    vis[cur] = true;
    for (int i = 1; i <= k; ++i) {
        if (exist[cur][i] == 0) {
            c[cur] = i;
            for (int p = 0; p < edge[cur].size(); ++p)
                exist[edge[cur][p]][i] = cur;
            break;
        }
    }
    if (c[cur] == 0) return P = cur, false;
    for (int p = 0; p < edge[cur].size(); ++p)
        if (!vis[edge[cur][p]])
            if (!dfs(edge[cur][p])) return false;
    return true;
}

inline void solve() {
    bool finish = true;
    memset(vis, false, sizeof(vis));
    for (int i = 1; i <= n; ++i) memset(exist[i], 0, sizeof(int) * (k + 1));
    memset(c, 0, sizeof(c));
    for (int i = 1; i <= n; ++i)
        if (!vis[i])
            if (!dfs(i)) {
                finish = false;
                break;
            }
    if (finish) {
        printf("color ");
        for (int i = 1; i <= n; ++i) printf("%d ", c[i]);
    } else {
        printf("path "), printf("%d ", P);
        for (int i = k; i >= 1; --i)
            printf("%d ", exist[P][i]), P = exist[P][i];
    }
    printf("\n");
}

int main() {
    int t;
    R(t);
    while (t--) {
        read_in();
        solve();
    }
    return 0;
}