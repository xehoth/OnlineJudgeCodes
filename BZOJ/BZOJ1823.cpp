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

namespace TwoSat {

const int MAXN = 250;

std::vector<int> edge[MAXN];

inline void debug(int u) {
    if (u & 1)
        std::cerr << 'h';
    else
        std::cerr << 'm';
}

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v);
    edge[v ^ 1].push_back(u ^ 1);
}

bool vis[MAXN];
int inComponent[MAXN], dfn[MAXN], low[MAXN], idx, cnt;

std::vector<int> st;

inline void tarjan(const int u) {
    register int v;
    vis[u] = true, st.push_back(u), low[u] = dfn[u] = ++idx;
    for (register int i = 0; i < edge[u].size(); i++) {
        if (!dfn[v = edge[u][i]])
            tarjan(v), low[u] = std::min(low[u], low[v]);
        else if (vis[v])
            low[u] = std::min(low[u], dfn[v]);
    }
    if (low[u] == dfn[u]) {
        cnt++;
        do
            vis[v = st.back()] = false, st.pop_back(), inComponent[v] = cnt;
        while (u != v);
    }
}

inline void init(int n) {
    n = (n << 1 | 1) + 1;
    memset(edge, 0, sizeof(std::vector<int>) * n);
    memset(dfn, 0, sizeof(int) * n);
    memset(low, 0, sizeof(int) * n);
    st.clear();
    idx = cnt = 0;
}

inline void solve() {
    using namespace IO;
    register int k, n, m;
    for (read(k); read(n), read(m), k--;) {
        init(n);
        register char a, b;
        register int u, v;
        for (register int i = 0; i < m; i++) {
            read(a), read(u), read(b), read(v);
            addEdge(u << 1 | a == 'h' ^ 1, v << 1 | b == 'h');
        }
        for (register int i = 2, r = n << 1 | 1; i <= r; i++)
            if (!dfn[i]) tarjan(i);
        for (register int i = 1; i <= n; i++) {
            if (inComponent[i << 1] == inComponent[i << 1 | 1]) {
                print("BAD\n");
                break;
            }
            i == n ? print("GOOD\n") : (void)0;
        }
    }
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