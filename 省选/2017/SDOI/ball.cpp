/*******************************************************************************
 * Copyright (c) 2016-2017, xehoth
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name xehoth, nor the names of its contributors may be used
 *       to endorse or promote products derived from this software without 
 *       specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY XEHOTH AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEHOTH AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/
#include <bits/stdc++.h>
#include <ext/pb_ds/priority_queue.hpp>
/**
 * 「SDOI2017」新生舞会 10-05-2017
 * @author xehoth 
 */
namespace IO {

inline char read() {
    static const int IN_LEN = 1000000;
    static char buf[IN_LEN], *s, *t;
    s == t ? t = (s = buf) + fread(buf, 1, IN_LEN, stdin) : 0;
    return s == t ? -1 : *s++;
}

template<class T>
inline void read(T &x) {
    static char c;
    static bool iosig;
    for (c = read(), iosig = false; !isdigit(c); c = read()) {
        if (c == -1) return;
        c == '-' ? iosig = true : 0;
    }
    for (x = 0; isdigit(c); c = read())
        x = (x + (x << 2) << 1) + (c ^ '0');
    iosig ? x = -x : 0;
}

const int OUT_LEN = 1000000;

char obuf[OUT_LEN], *oh = obuf;

inline void print(char c) {
    oh == obuf + OUT_LEN ? (fwrite(obuf, 1, OUT_LEN, stdout), oh = obuf) : 0;
    *oh++ = c;
}

template<class T>
inline void print(T x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[cnt++] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() {
    fwrite(obuf, 1, oh - obuf, stdout);
}
}

namespace PrimalDual {

const int MAXM = 103;
const int MAXN = MAXM * 2 + 2;

struct Node {
    int v, f, index;
    double w;
    Node(const int v, const int f, const double w, const int index) :
        v(v), f(f), w(w), index(index) {}
};

std::vector<Node> edge[MAXN];

inline void addEdge(const int u, const int v, const int f, const double w) {
    edge[u].push_back(Node(v, f, w, edge[v].size()));
    edge[v].push_back(Node(u, 0, -w, edge[u].size() - 1));
}

typedef std::pair<double, int> Pair;
typedef __gnu_pbds::priority_queue<Pair, std::greater<Pair> > PriorityQueue;

double h[MAXN], d[MAXN];
bool vis[MAXN];
int prev[MAXN], pree[MAXN];

const double INF = 1e60;

inline void spfa(const int s, const int n) {
    static std::queue<int> q;
    std::fill(h, h + n + 1, INF);
    memset(vis, 0, sizeof(bool) * (n + 1));
    q.push(s), h[s] = 0;
    while (!q.empty()) {
        register int u = q.front();
        q.pop();
        vis[u] = false;
        for (register int i = 0; i < edge[u].size(); i++) {
            Node *e = &edge[u][i];
            if (e->f > 0 && h[u] + e->w < h[e->v]) {
                h[e->v] = h[u] + e->w;
                if (!vis[e->v]) q.push(e->v), vis[e->v] = true;
            }
        }
    }
}

inline void dijkstra(const int s, const int n) {
    static PriorityQueue::point_iterator id[MAXN];
    memset(vis, 0, n + 1);
    std::fill(d, d + n + 1, INF);
    memset(id, 0, sizeof(PriorityQueue::point_iterator) * (n + 1));
    static PriorityQueue q;
    d[s] = 0, id[s] = q.push(Pair(0, s));
    while (!q.empty()) {
        Pair now = q.top();
        q.pop();
        register int v = now.second;
        if (vis[v] || d[v] < now.first) continue;
        vis[v] = true;
        for (register int i = 0; i < edge[v].size(); i++) {
            Node *p = &edge[v][i];
            register double w = d[v] + p->w + h[v] - h[p->v];
            if (p->f > 0 && d[p->v] > w) {
                d[p->v] = w, prev[p->v] = v, pree[p->v] = i;
                if (id[p->v] != NULL) q.modify(id[p->v], Pair(d[p->v], p->v));
                else id[p->v] = q.push(Pair(d[p->v], p->v));
            }
        }
    }
}

inline double minCostFlow(const int s, const int t, const int n) {
    register double ans = 0;
    for (spfa(s, n); ;) {
        dijkstra(s, n);
        if (d[t] == INF) break;
        for (register int i = 0; i <= n; i++) h[i] += d[i];
        ans += h[t];
        for (register int i = t; i != s; i = prev[i]) {
            Node *p = &edge[prev[i]][pree[i]];
            p->f--, edge[p->v][p->index].f++;
        }
    }
    return ans;
}

int n, a[MAXM][MAXM], b[MAXM][MAXM];

inline bool check(double x) {
    const int s = n * 2, t = s + 1;
    memset(edge, 0, sizeof(std::vector<Node>) * (t + 1));
    for (register int i = 0; i < n; i++) addEdge(s, i, 1, 0);
    for (register int i = 0; i < n; i++) addEdge(i + n, t, 1, 0);
    for (register int i = 0; i < n; i++)
        for (register int j = 0; j < n; j++) 
            addEdge(i, j + n, 1, -(a[i][j] - x * b[i][j]));
    return minCostFlow(s, t, t + 1) < 1e-7;
}

inline void solve() {
    using namespace IO;
    read(n);
    for (register int i = 0; i < n; i++)
        for (register int j = 0; j < n; j++)
            read(a[i][j]);
    for (register int i = 0; i < n; i++)
        for (register int j = 0; j < n; j++)
            read(b[i][j]);
    
    register double l = 0, r = 10000;
    while (r - l > 1e-7) {
        double mid = (l + r) / 2;
        if (check(mid)) l = mid;
        else r = mid;
    }
    
    printf("%.6f\n", l);
}
}

int main() {
    freopen("ball.in", "r", stdin);
    freopen("ball.out", "w", stdout);
    PrimalDual::solve();
    return 0;
}