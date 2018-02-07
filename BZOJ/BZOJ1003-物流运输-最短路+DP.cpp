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
 * 「ZJOI 2006」物流运输 16-05-2017
 * 最短路 + dp
 * @author xehoth
 */
namespace IO {

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
        x < 0 ? (print('-'), x = -x) : 0;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() { fwrite(obuf, 1, oh - obuf, stdout); }
}  // namespace IO

namespace Task {

const int MAXN = 25;
const int MAXM = 105;

struct Node {
    int v, w;

    Node(const int v, const int w) : v(v), w(w) {}
};

std::vector<Node> edge[MAXN];

inline void addEdge(const int u, const int v, const int w) {
    edge[u].push_back(Node(v, w));
    edge[v].push_back(Node(u, w));
}

int dis[MAXN], n, m, k, e, cost[MAXM][MAXM];

bool vis[MAXN], disable[MAXN][MAXM], flag[MAXN];

typedef std::pair<int, int> Pair;
typedef __gnu_pbds::priority_queue<Pair, std::greater<Pair> > PriorityQueue;

inline int dijkstra(const int n) {
    static PriorityQueue q;
    static PriorityQueue::point_iterator id[MAXN];
    memset(id, 0, sizeof(PriorityQueue::point_iterator) * (n + 1));
    memset(dis, 0x3f, sizeof(int) * (n + 1));
    memset(vis, 0, sizeof(bool) * (n + 1));
    dis[1] = 0, id[1] = q.push(Pair(0, 1));
    while (!q.empty()) {
        register int v = q.top().second;
        q.pop();
        if (vis[v]) continue;
        for (register int i = 0; i < edge[v].size(); i++) {
            Node *p = &edge[v][i];
            if (dis[v] + p->w < dis[p->v] && !flag[p->v]) {
                dis[p->v] = dis[v] + p->w;
                if (id[p->v] != NULL)
                    q.modify(id[p->v], Pair(dis[p->v], p->v));
                else
                    id[p->v] = q.push(Pair(dis[p->v], p->v));
            }
        }
    }
    return dis[m];
}

const int INF = 0x3f3f3f3f;

inline void solve() {
    using namespace IO;
    read(n), read(m), read(k), read(e);
    for (register int i = 0, u, v, w; i < e; i++) {
        read(u), read(v), read(w);
        addEdge(u, v, w);
    }

    register int d;
    read(d);
    for (register int i = 0, p, a, b; i < d; i++) {
        read(p), read(a), read(b);
        std::fill(disable[p] + a, disable[p] + b + 1, true);
    }

    for (register int i = 1; i <= n; i++) {
        for (register int j = i; j <= n; j++) {
            memset(flag, 0, sizeof(bool) * (m + 1));
            for (register int k = 1; k <= m; k++) {
                for (register int l = i; l <= j; l++) {
                    if (disable[k][l]) {
                        flag[k] = true;
                        break;
                    }
                }
            }
            cost[i][j] = dijkstra(m);
        }
    }
    for (register int i = 1; i <= n; i++) {
        for (register int j = i + 1; j <= n; j++) {
            if (cost[i][j] != INF) {
                cost[i][j] *= (j - i + 1);
            }
        }
    }
    static int f[MAXM];
    memcpy(f, cost[1], sizeof(int) * (n + 1));
    for (register int i = 2; i <= n; i++) {
        for (register int j = 1; j <= n; j++) {
            f[i] = std::min(f[i], f[j] + cost[j + 1][i] + k);
        }
    }
    print(f[n]);
}
}  // namespace Task

int main() {
    Task::solve();
    IO::flush();
    return 0;
}
