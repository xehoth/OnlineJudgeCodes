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
/**
 * 「SDOI2017」树点涂色 09-05-2017
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
        if (x < 0) print('-'), x = -x;
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

inline void flush() {
    fwrite(obuf, 1, oh - obuf, stdout);
}

}

namespace SharedData {

const int MAXN = 100010;

std::vector<int> edge[MAXN];

inline void addEdge(const int u, const int v) {
    edge[u].push_back(v);
    edge[v].push_back(u);
}
}

namespace SegmentTree {

using SharedData::MAXN;

struct Node {
    int max, add;
} d[MAXN << 2];

int M;

inline void build(const int n) {
    for (M = 1; M < n + 2; M <<= 1);
}

inline void cover(int k, int v) {
    d[k].max += v, d[k].add += v;
}

inline void pushDown(int k) {
    if (d[k].add && k < M) {
        (k << 1) ? (cover(k << 1, d[k].add), 0) : 0;
        (k << 1 | 1) ? (cover(k << 1 | 1, d[k].add), 0) : 0;
        d[k].add = 0;
    }
}

inline void update(int k) {
    static int st[25], top;
    for (top = 0; k; k >>= 1) st[++top] = k;
    while (top--) pushDown(st[top]);
}

inline void maintain(int k) {
    d[k].max = std::max(d[k << 1].max, d[k << 1 | 1].max);
}

inline void modify(int s, int t, int v) {
    register int l = 0, r = 0;
    for (s = s + M - 1, t = t + M + 1; s ^ t ^ 1; s >>= 1, t >>= 1) {
        if (~s & 1) {
            l ? 0 : (update(l = s ^ 1), 0);
            cover(s ^ 1, v);
        }
        if (t & 1) {
            r ? 0 : (update(r = t ^ 1), 0);
            cover(t ^ 1, v);
        }
    }
    for (l >>= 1; l; l >>= 1) maintain(l);
    for (r >>= 1; r; r >>= 1) maintain(r);
}

inline int query(int s, int t) {
    register int l = 0, r = 0, ret = 0;
    for (s = s + M - 1, t = t + M + 1; s ^ t ^ 1; s >>= 1, t >>= 1) {
        if (~s & 1) {
            l ? 0 : (update(l = s ^ 1), 0);
            ret = std::max(ret, d[s ^ 1].max);
        }
        if (t & 1) {
            r ? 0 : (update(r = t ^ 1), 0);
            ret = std::max(ret, d[t ^ 1].max);
        }
    }
    return ret;
}

inline int query(int x) {
    return query(x, x); 
}
}

namespace HeavyLightChainDecomposition {

using namespace SharedData;

int dep[MAXN], sz[MAXN], top[MAXN], fa[MAXN], son[MAXN], pos[MAXN];

bool vis[MAXN];

int idx;

inline void dfs1(int u) {
    dep[u] = dep[fa[u]] + 1, sz[u] = 1, vis[u] = true;
    for (register int i = 0, v; i < edge[u].size(); i++) {
        if (!vis[v = edge[u][i]]) {
            fa[v] = u, dfs1(v), sz[u] += sz[v];
            sz[v] > sz[son[u]] ? son[u] = v : 0;
        }
    }
}

inline void dfs2(int u) {
    vis[u] = false, pos[u] = ++idx, top[u] = (u == son[fa[u]] ? top[fa[u]] : u);
    for (register int i = 0; i < edge[u].size(); i++)
        edge[u][i] == son[u] ? (dfs2(edge[u][i]), 0) : 0;
    for (register int i = 0; i < edge[u].size(); i++)
        vis[edge[u][i]] ? (dfs2(edge[u][i]), 0) : 0;
}

inline void cut(int root = 1) {
    dfs1(root), dfs2(root);
}

inline int getLca(int u, int v) {
    while (top[u] != top[v])
        dep[top[u]] < dep[top[v]] ? v = fa[top[v]] : u = fa[top[u]];
    return dep[u] < dep[v] ? u : v;
}
}


namespace LinkCutTree {

using namespace SharedData;

using HeavyLightChainDecomposition::pos;
using HeavyLightChainDecomposition::sz;

struct Node *null;

struct Node {
    Node *c[2], *fa;
    Node *top;
    int id;
    
    Node(int id = 0) : fa(null), top(NULL), id(id) {
        c[0] = c[1] = null;
    }

    inline bool relation() {
        return this == fa->c[1];
    }

    inline void rotate(bool f) {
        Node *o = fa;
        top = o->top;
        (fa = o->fa)->c[o->relation()] = this;
        (o->c[f] = c[!f])->fa = o;
        (c[!f] = o)->fa = this;
    }

    inline void splay() {
        Node *o = fa;
        bool f;
        for (; o != null; o = fa) {
            o->fa == null ? rotate(o->c[1] == this) :
            ((f = o->c[1] == this) == (o->fa->c[1] == o)
            ? (o->rotate(f), rotate(f)) : (rotate(f), rotate(!f)));
        }
    }

    inline Node *get(Node *p) {
        while (p->c[0] != null) p = p->c[0];
        return p;
    }

    inline void set(int u) {
        SegmentTree::modify(pos[u], pos[u] + sz[u] - 1, 1);
    }

    inline void unset(int u) {
        SegmentTree::modify(pos[u], pos[u] + sz[u] - 1, -1);
    }
   
    inline void expose() {
        splay();
        if (c[1] != null) {
            set(get(c[1])->id);
            c[1]->top = this, c[1]->fa = null;
            c[1] = null;
        }
    }

    inline bool splice() {
        splay();
        if (!top) return false;
        unset(get(this)->id);
        top->expose(), top->c[1] = this;
        fa = top, top = NULL;
        return true;
    }

    inline void access() {
        expose();
        while (splice());
    }

} pool[MAXN];

inline void link(int u, int v) {
    (pool + v)->top = pool + u;
}

inline void init(const int n) {
    null = pool, null->fa = null, null->top = NULL;
    for (register int i = 1; i <= n; i++) pool[i] = Node(i);
}

inline void solve() {
    using namespace IO;
    register int n, m;
    read(n), read(m);
    for (register int i = 1, u, v; i < n; i++) read(u), read(v), addEdge(u, v);
    HeavyLightChainDecomposition::cut();
    SegmentTree::build(n);
    init(n);
    for (register int i = 2; i <= n; i++) link(HeavyLightChainDecomposition::fa[i], i);
    for (register int i = 2; i <= n; i++) {

        SegmentTree::modify(pos[i], pos[i] + sz[i] - 1, 1);
    }

    register int cmd, x, y, k;
    while (m--) {
        read(cmd);
        switch (cmd) {
            case 1:
                read(x), (pool + x)->access();
                break;
            case 2:
                read(x), read(y), k = HeavyLightChainDecomposition::getLca(x, y);
                print(SegmentTree::query(pos[x]) + SegmentTree::query(pos[y]) - 
                    2 * SegmentTree::query(pos[k]) + 1), print('\n');
                break;
            case 3:
                read(x);
                print(SegmentTree::query(pos[x], pos[x] + sz[x] - 1) + 1), print('\n');
                break;
        }
    }
}

}

int main() {
    freopen("paint.in", "r", stdin);
    freopen("paint.out", "w", stdout);
    LinkCutTree::solve();
    IO::flush();
    return 0;
}
