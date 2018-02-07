/**
 * Copyright (c) 2017, xehoth
 * All rights reserved.
 * 「BZOJ 1179」25-04-2017
 *
 * @author xehoth
 */
#include <cstdio>
#include <cstdlib>
#include <iostream>
#define ri IO::freadInt
using namespace std;
namespace IO {
typedef long long Long;
using namespace std;
#define IO_L 1048576
char _buf[IO_L], *S, *T, _buf1[IO_L + 1], *S1 = _buf1, c;
inline char freadChar() {
    if (S == T) {
        T = (S = _buf) + fread(_buf, 1, IO_L, stdin);
        if (S == T) return 0;
    }
    return *S++;
}
inline void freadLong(Long &x) {
    for (c = freadChar(); c < '0' || c > '9'; c = freadChar())
        ;
    for (x = c ^ '0', c = freadChar(); c >= '0' && c <= '9'; c = freadChar())
        x = (x << 3) + (x << 1) + (c ^ '0');
}
void freadInt(int &x) {
    for (c = freadChar(); c < '0' || c > '9'; c = freadChar())
        ;
    for (x = c ^ '0', c = freadChar(); c >= '0' && c <= '9'; c = freadChar())
        x = (x << 3) + (x << 1) + (c ^ '0');
}
inline void fwriteChar(char c) {
    if (S1 == _buf1 + IO_L) {
        fwrite(_buf1, 1, IO_L, stdout);
        S1 = _buf1;
    }
    *S1++ = c;
}
inline void fwriteString(const string &str) {
    for (register int i = 0, range = str.length(); i < range; i++) {
        if (S1 == _buf1 + IO_L) {
            fwrite(_buf1, 1, IO_L, stdout);
            S1 = _buf1;
        }
        *S1++ = str[i];
    }
}
inline void flushIO() { fwrite(_buf1, 1, S1 - _buf1, stdout); }
void fwriteInt(int x) {
    if (x > 9) fwriteInt(x / 10);
    fwriteChar(x % 10 ^ '0');
}
}  // namespace IO
const int N = 500010;
int n, m, s, q, size, tot, top, ans;
int dfn[N], low[N], id[N];
int stack[N];
bool f[N];
int nxt[N], last[N], to[N], v[N];
bool b[N];
int first[N], dis[N], w[N];
bool exist[N], bar[N];
int p[N];
struct node {
    int next;
    int to;
    int len;
} edge[N];
inline void insert(int x, int y, int w) {
    size++;
    edge[size].next = first[x];
    first[x] = size;
    edge[size].to = y;
    edge[size].len = w;
}
inline void init() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int x, y;
    ri(n), ri(m);
    for (register int i = 1; i <= m; i++) {
        ri(x), ri(y);
        nxt[i] = last[x];
        last[x] = i;
        to[i] = y;
    }
    for (register int i = 1; i <= n; i++) {
        ri(x);
        v[i] = x;
    }
    ri(s), ri(q);
    for (register int i = 1; i <= q; i++) {
        ri(x);
        b[x] = true;
    }
}

void tarjan(int x) {
    tot++;
    while (stack[top] != x) {
        f[stack[top]] = false;
        id[stack[top]] = tot;
        top--;
    }
    f[stack[top]] = false;
    id[stack[top]] = tot;
    top--;
}

void dfs(int x) {
    size++;
    dfn[x] = size;
    low[x] = size;
    f[x] = true;
    stack[++top] = x;
    for (register int y = last[x]; y; y = nxt[y])
        if (!dfn[to[y]]) {
            dfs(to[y]);
            low[x] = min(low[x], low[to[y]]);
        } else if (f[to[y]])
            low[x] = min(low[x], low[to[y]]);
    if (dfn[x] == low[x]) tarjan(x);
}
inline void build() {
    for (register int i = 1; i <= n; i++) {
        w[id[i]] += v[i];
        if (b[i]) bar[id[i]] = true;
    }
    for (register int i = 1; i <= n; i++)
        for (register int j = last[i]; j; j = nxt[j])
            if (id[i] != id[to[j]]) insert(id[i], id[to[j]], w[id[to[j]]]);
}

inline void spfa() {
    register int head = 0, tail = 1;
    p[1] = id[s];
    exist[id[s]] = true;
    while (head < tail) {
        register int x = p[++head];
        exist[x] = false;
        for (register int y = first[x]; y; y = edge[y].next) {
            register int t = edge[y].to;
            if (dis[t] < dis[x] + edge[y].len) {
                dis[t] = dis[x] + edge[y].len;
                if (!exist[t]) {
                    exist[t] = true;
                    p[++tail] = t;
                }
            }
        }
    }
    for (register int i = 1; i <= tot; i++)
        if (bar[i]) ans = max(ans, dis[i]);
    ans += w[id[s]];
}

int main() {
    // freopen("in.in","r",stdin);
    init();
    for (register int i = 1; i <= n; i++)
        if (!dfn[i]) dfs(i);
    size = 0;
    build();
    spfa();
    cout << ans;
    return 0;
}
