/*
 * created by xehoth on 22-04-2017
 */
// #define DBG
// #define DBG_DFS
// #define DBG_UPDATE
// #define DBG_GETPOS
// #define DBG_GETNUM
// #define DBG_ADD_TMP
// #define DBG_ADD_COLOR
// #define DBG_UPDATE_TMP

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
        *buf = '\0';
        return -1;
    }
    do
        buf[s++] = c;
    while (c = read(), !isspace(c) && c != -1);
    buf[s] = '\0';
    return s;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
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

namespace SharedData {

const int MAXN = 200005;
const int INF = INT_MIN;

int n, m, l, r;

struct Node {
    int v, w;

    Node(int v, int w) : v(v), w(w) {}
};

std::vector<Node> edge[MAXN];

int w[MAXN], len;

inline void addEdge(const int u, const int v, const int w) {
    edge[u].push_back(Node(v, w));
    edge[v].push_back(Node(u, w));
}

}  // namespace SharedData

namespace SegmentTree {

using namespace SharedData;

int d[MAXN << 2];

/*
int M;

inline void build(int n) {
        for (M = 1; M < n + 2; M <<= 1);
}

inline void modify(std::vector<int> &v, int l, int r) {
        for (register int i = l; i <= r; i++) d[i + M] = v[i - 1];
        for (register int i = M - 1; i; i--) d[i] = std::max(d[i << 1], d[i << 1
| !]);
}

inline void modify(std::vector<int> &v) {
        modify(v, 1, len = v.size());
}

inline void modify(int pos, int v) {
        d[pos += M] = std::max(d[pos], v);
        while (pos) d[pos >>= 1] = std::max(d[pos << 1], d[pos << 1 | 1]);
}

inline int query(int s, int t) {
        s = std::max(s, 1), v = std::min(v, len);
        if (s > v) return INF;
        register int ret = INF;
        for (s = s + M - 1, t = t + M + 1; s ^ t ^ 1; s >>= 1, t >>= 1) {
                if (~s & 1) ret = std::max(ret, d[s ^ 1]);
                if (t & 1) ret = std::max(ret, d[t ^ 1]);
        }
        return ret;
}
*/

inline void modify(std::vector<int> &v, int l, int r, int k) {
    if (l == r) {
        d[k] = v[l - 1];

#ifdef DBG
        fprintf(stderr, "modify d[%d] = v[%d]\n", k, l - 1);
#endif

    } else {
        modify(v, l, l + r >> 1, k << 1),
            modify(v, (l + r >> 1) + 1, r, k << 1 | 1);
        d[k] = std::max(d[k << 1], d[k << 1 | 1]);
    }
}

inline void modify(std::vector<int> &v) {
#ifdef DBG
    fprintf(stderr, "modify vector begin now len is %d\n", len);
#endif

    modify(v, 1, len = v.size(), 1);
}

inline void modify(int pos, int v, int l, int r, int k) {
    if (l == r) {
        d[k] = std::max(d[k], v);

#ifdef DBG
        fprintf(stderr, "pos modify d[%d] = %d\n", k, d[k]);
#endif

    } else {
        pos < (l + r >> 1) + 1
            ? modify(pos, v, l, l + r >> 1, k << 1)
            : modify(pos, v, (l + r >> 1) + 1, r, k << 1 | 1);
        d[k] = std::max(d[k << 1], d[k << 1 | 1]);
    }
}

inline void modify(int pos, int v) {
#ifdef DBG
    fprintf(stderr, "pos modify begin pos = %d\n", pos);
#endif

    modify(pos, v, 1, len, 1);
}

/**
inline int query(int s, int t, int l, int r, int k) {
        if (s == l && t == r) {
                return d[k];
        } else if (t < (l + r >> 1) + 1) {
                return query(s, t, l, l + r >> 1, k << 1);
        } else if (s > (l + r >> 1)) {
                return query(s, t, (l + r) >> 1 + 1, r, k << 1 | 1);
        } else {
                return std::max(query(s, l + r >> 1, l, l + r >> 1, k << 1),
                                                query((l + r >> 1) + 1, t, (l +
r >> 1) + 1, r, k << 1 | 1));
        }
}
*/

inline int query(int s, int t, int l, int r, int k) {
    return s == l && t == r
               ? d[k]
               : t < ((l + r >> 1) + 1)
                     ? query(s, t, l, l + r >> 1, k << 1)
                     : s > (l + r >> 1)
                           ? query(s, t, (l + r >> 1) + 1, r, k << 1 | 1)
                           : std::max(
                                 query(s, l + r >> 1, l, l + r >> 1, k << 1),
                                 query((l + r >> 1) + 1, t, (l + r >> 1) + 1, r,
                                       k << 1 | 1));
}

inline int query(int u, int v) {
    u = std::max(u, 1), v = std::min(v, len);

#ifdef DBG
    fprintf(stderr, "query(u = %d, v = %d)\n", u, v);
#endif

    return u > v ? INF : query(u, v, 1, len, 1);
}
}  // namespace SegmentTree

namespace Task {

using namespace SharedData;

int sz[MAXN], ans = INT_MIN;

bool vis[MAXN];

inline void dfs(int u, int pre) {
#ifdef DBG_DFS
    fprintf(stderr, "now dfs at(%d, %d)\n", u, pre);
    for (register int i = 1; i <= n; i++)
        fprintf(stderr, "sz[%d] = %d ", i, sz[i]);
    fprintf(stderr, "\n");
#endif

    sz[u] = 1;
    for (register int i = 0, v; i < edge[u].size(); i++)
        if ((v = edge[u][i].v) != pre && !vis[v]) dfs(v, u), sz[u] += sz[v];
}

inline int getPos(int u, int pre, int size) {
    register int rem = size - sz[u];

#ifdef DBG_GETPOS
    fprintf(stderr, "now getPos at(%d, %d, %d), rem = %d\n", u, pre, size, rem);
#endif

    for (register int i = 0, v; i < edge[u].size(); i++) {
        if ((v = edge[u][i].v) != pre && !vis[v]) {
            if (int ret = getPos(v, u, size)) return ret;
            rem = std::max(rem, sz[v]);
        }
    }
    return rem * 2 <= size ? u : 0;
}

inline int getNum(int u, int pre, int dep) {
    register int ret = dep;

#ifdef DBG_GETNUM
    fprintf(stderr, "now getNum at (%d, %d, %d)\n", u, pre, dep);
#endif

    if (dep < r) {
        for (register int i = 0, v; i < edge[u].size(); i++)
            if ((v = edge[u][i].v) != pre && !vis[v])
                ret = std::max(ret, getNum(v, u, dep + 1));
    }
    return ret;
}

typedef std::vector<std::vector<int> > Vector;
typedef std::map<int, Vector> Map;

std::vector<int> cur;

inline void update(int u, int pre, int c, int dep, int val) {
#ifdef DBG_UPDATE
    fprintf(stderr, "vector cur is:\n");
    for (register int i = 0; i < cur.size(); i++)
        fprintf(stderr, "cur[%d] = %d ", i, cur[i]);
    fprintf(stderr, "\nnow update cur[%d]\n", dep);
#endif

    cur[dep] = std::max(cur[dep], val);
    if (dep < r) {
        for (register int i = 0; i < edge[u].size(); i++) {
            Node *p = &edge[u][i];
            if (p->v != pre && !vis[p->v])
                update(p->v, u, p->w, dep + 1, c != p->w ? val + w[p->w] : val);
        }
    }
}

inline void addColor(Map &color, const int u) {
    for (register int i = 0; i < edge[u].size(); i++) {
        Node *p = &edge[u][i];
        if (!vis[p->v]) {
            register int n = getNum(p->v, 0, 1);
            cur.assign(n, INF);

#ifdef DBG_ADD_COLOR
            fprintf(stderr, "cur.assign(%d, INT_MIN)\nvector cur is:\n", n);
            for (register int i = 0; i < cur.size(); i++)
                fprintf(stderr, "cur[%d] = %d ", i, cur[i]);
            fprintf(stderr, "\n");
#endif

            update(p->v, 0, p->w, 0, w[p->w]);

#ifdef DBG_ADD_COLOR
            fprintf(stderr, "add color at %d\n", p->w);
#endif
            color[p->w].push_back(cur);
        }
    }
}

inline bool cmp(const std::vector<int> &a, const std::vector<int> &b) {
    return a.size() > b.size();
}

inline void addTmp(Vector &tmp, Map &color) {
#ifdef DBG_ADD_TMP
    fprintf(stderr, "addTmp begin\n");
#endif

    for (Map::iterator it = color.begin(); it != color.end(); it++) {
#ifdef DBG_ADD_TMP
        fprintf(stderr, "sort %d Vector\n", it->first);
#endif

        std::sort(it->second.begin(), it->second.end(), cmp);
        std::vector<int> &s = it->second.front();
        SegmentTree::modify(s);
        for (Vector::iterator j = it->second.begin() + 1; j != it->second.end();
             j++) {
            std::vector<int> &v = *j;

#ifdef DBG_ADD_TMP
            fprintf(stderr, "v.size() + len >= l is %d\n",
                    v.size() + len >= l ? "true" : "false");
#endif

            if (v.size() + len >= l) {
                for (register int k = 0; k < v.size(); k++) {
                    register int a = SegmentTree::query(l - k - 1, r - k - 1);

#ifdef DBG_ADD_TMP
                    if (a != INF) fprintf(stderr, "update ans\n");
#endif

                    if (a != INF) ans = std::max(ans, a + v[k] - w[it->first]);

#ifdef DBG_ADD_TMP
                    fprintf(stderr, "ans = %d\n", ans);
#endif
                }
            }

            for (register int k = 0; k < v.size(); k++)
                SegmentTree::modify(k + 1, v[k]), s[k] = std::max(s[k], v[k]);
        }

#ifdef DBG_ADD_TMP
        fprintf(stderr, "query(%d, %d) = %d\n", l, r, SegmentTree::query(l, r));
#endif

        ans = std::max(ans, SegmentTree::query(l, r)), tmp.push_back(s);
    }
}

inline void updateTmp(Vector &tmp) {
#ifdef DBG_UPDATE_TMP
    fprintf(stderr, "sort tmp begin\n");
#endif

    std::sort(tmp.begin(), tmp.end(), cmp);
    SegmentTree::modify(tmp.front());
    for (Vector::iterator it = tmp.begin() + 1; it != tmp.end(); it++) {
        std::vector<int> &v = *it;

#ifdef DBG_UPDATE_TMP
        fprintf(stderr, "v.size() + len >= l is %d\n",
                v.size() + len >= l ? "true" : "false");
#endif

        if (v.size() + len >= l) {
            for (register int j = 0; j < v.size(); j++) {
                register int a = SegmentTree::query(l - j - 1, r - j - 1);
                if (a != INF) ans = std::max(ans, a + v[j]);
            }
        }
        for (register int j = 0; j < v.size(); j++)
            SegmentTree::modify(j + 1, v[j]);
    }
}

inline void solve(int u) {
#ifdef DBG
    fprintf(stderr, "\nnow solve at %d\n", u);
    fprintf(stderr, "dfs(%d, 0) begin\n", u);
#endif

    dfs(u, 0);
    if (sz[u] > l) {
#ifdef DBG
        fprintf(stderr, "sz[%d] > l\n", u);
#endif

        vis[u = getPos(u, 0, sz[u])] = true;

#ifdef DBG
        fprintf(stderr, "now u after getPos is %d", u);
#endif

        Map color;

#ifdef DBG
        fprintf(stderr, "add color into Map tmp color\n");
#endif
        addColor(color, u);

        std::vector<std::vector<int> > tmp;

#ifdef DBG
        fprintf(stderr, "select valid into tmp\n");
#endif

        addTmp(tmp, color);

#ifdef DBG
        fprintf(stderr, "tmp.size() = %d\n", tmp.size());
#endif

        if (tmp.size() > 1) updateTmp(tmp);
        for (register int i = 0, v; i < edge[u].size(); i++)
            if (!vis[v = edge[u][i].v]) solve(v);
    }
}

inline void solve() {
    read(n), read(m), read(l), read(r);

#ifdef DBG
    fprintf(stderr, "Input Data\nn = %d, m = %d, l = %d, r = %d\n", n, m, l, r);
#endif

    for (register int i = 1; i <= m; i++) read(w[i]);

#ifdef DBG
    for (register int i = 1; i <= m; i++)
        fprintf(stderr, "w[%d] = %d ", i, w[i]);
    fprintf(stderr, "\n");
#endif

    for (register int i = 1, u, v, w; i < n; i++)
        read(u), read(v), read(w), addEdge(u, v, w);

#ifdef DBG
    fprintf(stderr, "Data Input Done Runtime Error cannot be here\n");
#endif

    solve(1);
    print(ans);
}
}  // namespace Task

int main() {
    // freopen("journey.in", "r", stdin);
    // freopen("journey.out", "w", stdout);
    Task::solve();
    flush();
    return 0;
}
