#include <bits/stdc++.h>
#define LL long long
using namespace std;

const int L = 100 + 9;
const int N = 50000 + 9;
const int M = 1000000;
const int INF = 1000000000;

int head[N], nxt[M], to[M], dis[N], cur[N], flow[M];
int n, m, X[L], Y[L], cx[L], cy[L], k, S, T, mat[L][L], vout;
queue<int> que;

inline int read() {
    char c = getchar();
    int ret = 0, f = 1;
    while (c < '0' || c > '9') {
        if (c == '-') f = -1;
        c = getchar();
    }
    while (c <= '9' && c >= '0') {
        ret = ret * 10 + c - '0';
        c = getchar();
    }
    return ret * f;
}

#define id(x, y) (x + (y - 1) * n)
inline void addEdge(int u, int v, int f) {
    static int TT = 1;
    to[++TT] = v;
    nxt[TT] = head[u];
    head[u] = TT;
    flow[TT] = f;
    to[++TT] = u;
    nxt[TT] = head[v];
    head[v] = TT;
    flow[TT] = 0;
}

inline bool bfs() {
    memset(dis, -1, sizeof(dis));
    dis[S] = 0;
    que.push(S);
    while (!que.empty()) {
        int w = que.front();
        que.pop();
        for (int i = head[w]; i; i = nxt[i])
            if (flow[i] && !~dis[to[i]])
                dis[to[i]] = dis[w] + 1, que.push(to[i]);
    }
    return ~dis[T];
}

int dfs(int w, int f) {
    if (w == T)
        return f;
    else {
        int ret = 0;
        for (int &i = cur[w]; i; i = nxt[i])
            if (flow[i] && dis[to[i]] == dis[w] + 1) {
                int tmp = dfs(to[i], min(f, flow[i]));
                ret += tmp;
                f -= tmp;
                flow[i] -= tmp;
                flow[i ^ 1] += tmp;
                if (!f) break;
            }
        return ret;
    }
}

inline int dinic() {
    int ret = 0;
    while (bfs()) {
        memcpy(cur, head, sizeof(head));
        ret += dfs(S, INF);
    }
    return ret;
}

int main() {
    m = read();
    n = read();
    k = read();
    S = 0, T = N - 1;
    vout = n * m;
    for (int i = 1; i <= m; i++) Y[i] = read(), cy[i] = n;
    for (int i = 1; i <= n; i++) X[i] = read(), cx[i] = m;
    for (int i = 1, x, y; i <= k; i++)
        x = read(), y = read(), mat[x][y] = 1, cx[x]--, cy[y]--, vout--;
    for (int i = 1; i <= m; i++)
        if (cy[i] < Y[i]) puts("JIONG!"), exit(0);
    for (int i = 1; i <= n; i++)
        if (cx[i] < X[i]) puts("JIONG!"), exit(0);

    for (int i = 1; i <= n; i++) addEdge(S, i, cx[i] - X[i]);
    for (int i = 1; i <= m; i++) addEdge(n + i, T, cy[i] - Y[i]);
    for (int j = 1; j <= m; j++)
        for (int i = 1; i <= n; i++)
            if (!mat[i][j])
                addEdge(m + n + id(i, j), m + n + m + n + id(i, j), 1),
                    addEdge(i, m + n + id(i, j), 1),
                    addEdge(m + n + m + n + id(i, j), j + n, 1);
    printf("%d\n", vout - dinic());
    return 0;
}