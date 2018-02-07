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

inline int read(char *buf) {
    register int s = 0;
    register char c;
    while (c = read(), isspace(c) && c != -1)
        ;
    if (c == -1) {
        *buf = 0;
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = 0;
    return s;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

inline void print(const char *s) {
    for (; *s; s++) print(*s);
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

namespace SuffixArray {

#define long long long

inline bool islms(const int i, const bool *t) {
    return i > 0 && t[i] && !t[i - 1];
}

template <typename T>
inline void sort(T s, int *sa, const int len, const int sz, const int sigma,
                 bool *t, int *b, int *cb, int *p) {
    memset(b, 0, sizeof(int) * sigma);
    memset(sa, -1, sizeof(int) * len);
    for (register int i = 0; i < len; i++) b[s[i]]++;
    cb[0] = b[0];
    for (register int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i];
    for (register int i = sz - 1; i >= 0; i--) sa[--cb[s[p[i]]]] = p[i];
    for (register int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i - 1];
    for (register int i = 0; i < len; i++)
        if (sa[i] > 0 && !t[sa[i] - 1]) sa[cb[s[sa[i] - 1]]++] = sa[i] - 1;
    cb[0] = b[0];
    for (register int i = 1; i < sigma; i++) cb[i] = cb[i - 1] + b[i];
    for (register int i = len - 1; i >= 0; i--)
        if (sa[i] > 0 && t[sa[i] - 1]) sa[--cb[s[sa[i] - 1]]] = sa[i] - 1;
}

template <typename T>
inline void sais(T s, int *sa, const int len, bool *t, int *b, int *b1,
                 const int sigma) {
    register int i, j, x, p = -1, sz = 0, cnt = 0, *cb = b + sigma;
    for (t[len - 1] = 1, i = len - 2; i >= 0; i--)
        t[i] = s[i] < s[i + 1] || (s[i] == s[i + 1] && t[i + 1]);
    for (i = 1; i < len; i++)
        if (t[i] && !t[i - 1]) b1[sz++] = i;
    sort(s, sa, len, sz, sigma, t, b, cb, b1);
    for (i = sz = 0; i < len; i++)
        if (islms(sa[i], t)) sa[sz++] = sa[i];
    for (i = sz; i < len; i++) sa[i] = -1;
    for (i = 0; i < sz; i++) {
        for (x = sa[i], j = 0; j < len; j++) {
            if (p == -1 || s[x + j] != s[p + j] || t[x + j] != t[p + j]) {
                p = x, cnt++;
                break;
            } else if (j > 0 && (islms(x + j, t) || islms(p + j, t))) {
                break;
            }
        }
        sa[sz + (x >>= 1)] = cnt - 1;
    }
    for (i = j = len - 1; i >= sz; i--)
        if (sa[i] >= 0) sa[j--] = sa[i];
    register int *s1 = sa + len - sz, *b2 = b1 + sz;
    if (cnt < sz)
        sais(s1, sa, sz, t + len, b, b1 + sz, cnt);
    else
        for (i = 0; i < sz; i++) sa[s1[i]] = i;
    for (i = 0; i < sz; i++) b2[i] = b1[sa[i]];
    sort(s, sa, len, sz, sigma, t, b, cb, b2);
}

template <typename T>
inline void getHeight(T s, const int n, int *sa, int *rk, int *ht) {
    for (register int i = 1; i <= n; i++) rk[sa[i]] = i;
    for (register int i = 0, j = 0, k = 0; i < n; ht[rk[i++]] = k)
        for (k ? k-- : 0, j = sa[rk[i] - 1]; s[i + k] == s[j + k]; k++)
            ;
}

const int MAXN = 230000;

int logs[MAXN];

struct SuffixArray {
    int sa[MAXN], rk[MAXN], min[20][MAXN], n;
    bool t[MAXN << 1];
    int s[MAXN];

    inline void build(const int sigma) {
        s[n] = 0, sais(s, sa, n + 1, t, rk, min[0], sigma);
        rk[0] = min[0][0] = 0, getHeight(s, n, sa, rk, min[0]);
        for (register int w = 1; w <= logs[n]; w++)
            for (register int i = 1; i + (1 << w) < n; i++)
                min[w][i] =
                    std::min(min[w - 1][i], min[w - 1][i + (1 << w - 1)]);
    }

    inline int query(int l, int r) {
        l = rk[l], r = rk[r];
        l > r ? std::swap(l, r) : (void)0;
        static int bit;
        bit = logs[r - l];
        return std::min(min[bit][l + 1], min[bit][r - (1 << bit) + 1]);
    }

} suffixArray;

inline void init(const int n) {
    logs[0] = -1;
    for (register int i = 1; i <= n; i++) logs[i] = logs[i >> 1] + 1;
}

int belong[MAXN], split = 27, len[MAXN];
int sumLen[MAXN], lBound[MAXN], rBound[MAXN];

inline int lcp(int lb, int lLen, int rb, int rLen) {
    return suffixArray.query(
        lLen > 0 ? lBound[lb] + lLen - 1 : rBound[lb] + len[lb] + lLen,
        rLen > 0 ? lBound[rb] + rLen - 1 : rBound[rb] + len[rb] + rLen);
}

struct Node {
    int v, w;

    Node(int v, int w) : v(v), w(w) {}
};

std::vector<Node> edge[MAXN];

inline void addEdge(const int u, const int v, const int w) {
    edge[u].push_back(Node(v, w));
}

int S, T;

inline int getId(int x, int y) {
    if (x == 0) return S;
    if (x == -1) return T;
    if (y == 0 || y > len[x]) return T + 1;
    if (y > 0) return sumLen[x - 1] * 2 + y;
    return sumLen[x - 1] * 2 + len[x] - y;
}

inline void addEdge(int a, int x, int b, int y, int w) {
    addEdge(getId(a, x), getId(b, y), w);
}

bool vis[MAXN], in[MAXN];
int deg[MAXN];

inline bool stackSpfa(int s) {
    vis[s] = in[s] = true;
    for (register int i = 0, v; i < edge[s].size(); i++) {
        deg[v = edge[s][i].v]++;
        if (in[v] || (!vis[v] && stackSpfa(v))) return true;
    }
    return in[s] = false;
}

inline void topoSort(int n) {}

inline long buildGraph(int n) {
    register long ans = 0;
    for (register int i = 1; i <= n; i++) {
        for (register int j = 1, l, r; j <= len[i]; j++) {
            r = lcp(i, j, i, -j), l = std::min(j, len[i] - j + 1),
            ans = std::max(ans, (long)r * 2 - 1);
            if (r == l) {
                (j > len[i] - j + 1) ? addEdge(0, 0, i, -(j - r), r * 2 - 1)
                                     : addEdge(0, 0, i, j + r, r * 2 - 1);
            }
        }

        for (register int j = 2, l, r; j <= len[i]; j++) {
            r = lcp(i, j, i, -(j - 1)), l = std::min(j - 1, len[i] - j + 1),
            ans = std::max(ans, (long)r * 2);
            if (r == l) {
                (j - 1 > len[i] - j + 1) ? addEdge(0, 0, i, -(j - 1 - r), r * 2)
                                         : addEdge(0, 0, i, j + r, r * 2);
            }
        }
        addEdge(0, 0, i, 1, 0), addEdge(0, 0, i, -len[i], 0);
        for (register int j = 1, L, R; j <= len[i]; j++) {
            L = 0, R = 0;
            for (register int k = 1, r; k <= n; k++) {
                r = lcp(i, j, k, -len[k]);
                if (r == len[i] - j + 1)
                    addEdge(i, j, k, -(len[k] - r), r * 2);
                else if (r == len[k])
                    addEdge(i, j, i, j + r, r * 2);
                else
                    L = std::max(L, r * 2);
                r = lcp(i, -j, k, 1);
                if (r == j)
                    addEdge(i, -j, k, r + 1, r * 2);
                else if (r == len[k])
                    addEdge(i, -j, i, -(j - r), r * 2);
                else
                    R = std::max(R, r * 2);
            }
            if (L > 0) addEdge(i, j, -1, 0, L);
            if (R > 0) addEdge(i, -j, -1, 0, R);
        }
    }
    if (stackSpfa(S) || vis[T + 1]) return -1;
    return ans;
}

inline void topoSort(long &ans, const int n) {
    static std::vector<int> st;
    st.reserve(n), st.push_back(S);
    static long dis[MAXN];
    while (!st.empty()) {
        register int u = st.back();
        st.pop_back();
        for (register int i = 0, v; i < edge[u].size(); i++) {
            dis[v = edge[u][i].v] = std::max(dis[v], dis[u] + edge[u][i].w);
            if (!--deg[v]) st.push_back(v);
        }
    }
    ans = std::max(ans, *std::max_element(dis + S, dis + T + 2));
}

inline void solve(int n) {
    using namespace IO;
    register bool isCyclePossible = true;
    for (register int i = 1; i <= n; i++) {
        if (lcp(i, 0, i, -(len[i] + 1)) == len[i]) {
            isCyclePossible = false;
            break;
        }
    }
    S = 0, T = sumLen[n] * 2 + 10;
    register long ans = 0;
    if (isCyclePossible) ans = buildGraph(n);
    if (~ans)
        topoSort(ans, n), print(ans), print('\n');
    else
        print("Infinity\n");
}

inline void solve() {
    using namespace IO;
    register int n;
    read(n);
    init(200000);
    static char s[MAXN];
    for (register int i = 1; i <= n; i++) {
        len[i] = read(s), sumLen[i] = sumLen[i - 1] + len[i];
        suffixArray.s[suffixArray.n++] = split++;
        lBound[i] = suffixArray.n;
        for (register int j = 0; j < len[i]; j++)
            suffixArray.s[suffixArray.n++] = s[j] - 'a' + 1;
        /*------- rev -------*/
        suffixArray.s[suffixArray.n++] = split++;
        rBound[i] = suffixArray.n;
        for (register int j = 0; j < len[i]; j++)
            suffixArray.s[suffixArray.n++] = s[len[i] - j - 1] - 'a' + 1;
    }

    suffixArray.build(split + 1);
    solve(n);
}

#undef long
}

int main() {
#ifdef DBG
    freopen("in.in", "r", stdin);
#endif
    SuffixArray::solve();
    IO::flush();
    return 0;
}