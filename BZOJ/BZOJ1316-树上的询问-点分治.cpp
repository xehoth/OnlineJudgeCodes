/**
 * Copyright (c) 2016-2018, xehoth
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * 「BZOJ 1316」树上的询问 05-03-2018
 * 点分治
 * @author xehoth
 */
#include <algorithm>
#include <bitset>
#include <cstdio>
#include <iostream>
#include <list>
#include <vector>

struct InputOutputStream {
    enum { SIZE = 1 << 18 | 1 };

    char ibuf[SIZE], *s, *t, obuf[SIZE], *oh;

    InputOutputStream() : s(), t(), oh(obuf) {}

    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    inline char read() {
        return (s == t) && (t = (s = ibuf) + fread(ibuf, 1, SIZE, stdin)),
               s == t ? -1 : *s++;
    }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        static char c;
        static bool iosig;
        for (c = read(), iosig = false; !isdigit(c); c = read()) {
            if (c == -1) return *this;
            iosig |= c == '-';
        }
        for (x = 0; isdigit(c); c = read()) x = x * 10 + (c ^ '0');
        iosig && (x = -x);
        return *this;
    }

    inline InputOutputStream &operator<<(const char *s) {
        for (; *s; s++) *oh++ = *s;
        return *this;
    }
} io;

const int MAXN = 10000 + 1;
const int MAXM = 1000000 + 1;
const int MAXQ = 100 + 1;

std::bitset<MAXM> l;

int n, q;

struct Node {
    int v, w;

    Node(int v, int w) : v(v), w(w) {}
};

struct Query {
    int w;
    bool *ans;
};

typedef std::vector<Node>::iterator Iterator;
typedef std::list<Query> List;

std::vector<Node> g[MAXN];
std::vector<int> pos, dis;
List que;

int sz[MAXN];
bool vis[MAXN], ans[MAXQ];

void dfsSize(int u, int pre) {
    sz[u] = 1;
    for (Iterator p = g[u].begin(); p != g[u].end(); ++p) {
        if (!vis[p->v] && p->v != pre) {
            dfsSize(p->v, u);
            sz[u] += sz[p->v];
        }
    }
}

int get(int u, int pre, int n) {
    for (Iterator p = g[u].begin(); p != g[u].end(); ++p)
        if (!vis[p->v] && p->v != pre && sz[p->v] > n) return get(p->v, u, n);
    return u;
}

void dfs(int u, int pre, int d) {
    dis.push_back(d);
    for (Iterator p = g[u].begin(); p != g[u].end(); ++p)
        if (!vis[p->v] && p->v != pre) dfs(p->v, u, d + p->w);
}

void solve(int u) {
    dfsSize(u, 0);
    vis[u = get(u, 0, sz[u] / 2)] = true;
    for (int i = 0; i < (int)pos.size(); i++) l.reset(pos[i]);
    pos.clear();
    l.set(0);
    for (Iterator p = g[u].begin(); p != g[u].end(); ++p) {
        if (!vis[p->v]) {
            dis.clear();
            dfs(p->v, u, p->w);
            std::sort(dis.begin(), dis.end());
            for (List::iterator it = que.begin(); it != que.end();) {
                bool flag = true;
                for (int i = 0; i < (int)dis.size(); i++) {
                    if (it->w - dis[i] < 0) break;
                    if (l.test(it->w - dis[i])) {
                        *it->ans = true;
                        flag = false;
                        it = que.erase(it);
                        break;
                    }
                }
                if (flag) ++it;
            }
            for (int i = 0; i < (int)dis.size() && dis[i] < MAXM; i++) {
                l.set(dis[i]);
                pos.push_back(dis[i]);
            }
        }
    }
    for (Iterator p = g[u].begin(); p != g[u].end(); ++p)
        if (!vis[p->v]) solve(p->v);
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    io >> n >> q;
    for (int i = 1, u, v, w; i < n; i++) {
        io >> u >> v >> w;
        g[u].push_back(Node(v, w));
        g[v].push_back(Node(u, w));
    }
    Query tmp;
    for (int i = 1; i <= q; i++) {
        io >> tmp.w;
        if (tmp.w == 0) {
            ans[i] = true;
            continue;
        }
        tmp.ans = ans + i;
        que.push_back(tmp);
    }
    solve(1);
    for (int i = 1; i <= q; i++) io << (ans[i] ? "Yes\n" : "No\n");
    return 0;
}