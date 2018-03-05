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
 * 「BZOJ 3784」树上的路径 05-03-2018
 * 可持久化可并堆
 * @author xehoth
 */
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <queue>
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

    inline void print(char c) {
        (oh == obuf + SIZE) && (fwrite(obuf, 1, SIZE, stdout), oh = obuf);
        *oh++ = c;
    }

    template <typename T>
    inline void print(T x) {
        static int buf[21], cnt;
        if (x != 0) {
            (x < 0) && (print('-'), x = -x);
            for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
            while (cnt) print((char)buf[cnt--]);
        } else {
            print('0');
        }
    }

    template <typename T>
    inline InputOutputStream &operator<<(const T &x) {
        print(x);
        return *this;
    }
} io;

const int MAXN = 50000 + 1;

struct Edge {
    int v, w;

    Edge(int v, int w) : v(v), w(w) {}
};

std::vector<Edge> g[MAXN];

typedef std::vector<Edge>::iterator Iterator;

int n, m;

inline int get() {
    static unsigned int x = 495;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x & 1;
}

int dis[MAXN];

char *cur;

struct Node {
    static const int NODE_SIZE;
    Node *lc, *rc;
    int i;
    Node(int i) : lc(), rc(), i(i) {}

    inline int val() const { return dis[i]; }

    inline void *operator new(size_t) { return cur += NODE_SIZE; }
};

const int Node::NODE_SIZE = sizeof(Node);

const int MAXM = MAXN * 20;

char pool[MAXM * Node::NODE_SIZE];

int fa[MAXN];
Node *root[MAXN], *suc[MAXN];

Node *merge(Node *u, Node *v) {
    if (u == NULL) return v;
    if (v == NULL) return u;
    if (u->val() < v->val()) std::swap(u, v);
    Node *p = new Node(*u);
    if (get())
        p->lc = merge(p->lc, v);
    else
        p->rc = merge(p->rc, v);
    return p;
}

void dfs(int u, int pre) {
    root[u] = new Node(u);
    for (Iterator p = g[u].begin(); p != g[u].end(); ++p) {
        if (p->v != pre) {
            dis[p->v] = dis[u] + p->w;
            fa[p->v] = u;
            dfs(p->v, u);
            root[u] = merge(root[u], root[p->v]);
        }
    }
    static int sub[MAXN], cnt;
    static Node *tmp[MAXN];
    cnt = 0;
    for (Iterator p = g[u].begin(); p != g[u].end(); ++p)
        if (p->v != pre) sub[++cnt] = p->v;
    tmp[cnt + 1] = NULL;
    for (int i = cnt; i; i--) tmp[i] = merge(tmp[i + 1], root[sub[i]]);
    for (int i = 1; i <= cnt; i++) suc[sub[i]] = tmp[i + 1];
}

struct Data {
    int type, lca;
    Node *u, *v;
    int val;

    Data() {}

    Data(int lca, Node *u) : type(), lca(lca), u(u), v() {
        val = u->val() - dis[lca];
    }

    Data(int type, int lca, Node *u, Node *v)
        : type(type), lca(lca), u(u), v(v) {
        val = u->val() + v->val() - 2 * dis[lca];
    }

    inline bool operator<(const Data &p) const { return val < p.val; }
};

class PriorityQueue : public std::priority_queue<Data> {
   public:
    inline void reserve(int n) { super::c.reserve(n); }

    inline void push_back(const Data &p) { super::c.push_back(p); }

    inline void init() { std::make_heap(super::c.begin(), super::c.end()); }

   private:
    typedef std::priority_queue<Data> super;
} q;

int main() {
    // freopen("sample/1.in", "r", stdin);
    cur = pool;
    io >> n >> m;
    for (int i = 1, u, v, w; i < n; i++) {
        io >> u >> v >> w;
        g[u].push_back(Edge(v, w));
        g[v].push_back(Edge(u, w));
    }
    dfs(1, 0);
    q.reserve((m + n) * 2);
    for (int i = 1; i <= n; i++) q.push_back(Data(i, root[i]));
    for (int i = 2; i <= n; i++)
        if (suc[i] != NULL) q.push_back(Data(1, fa[i], root[i], suc[i]));
    q.init();
    for (Data t; m--;) {
        t = q.top();
        q.pop();
        io << t.val << '\n';
        switch (t.type) {
            case 0: {
                if (t.u->lc) q.push(Data(t.lca, t.u->lc));
                if (t.u->rc) q.push(Data(t.lca, t.u->rc));
                break;
            }
            case 1: {
                if (t.v->lc) q.push(Data(1, t.lca, t.u, t.v->lc));
                if (t.v->rc) q.push(Data(1, t.lca, t.u, t.v->rc));
                if (t.u->lc) q.push(Data(2, t.lca, t.u->lc, t.v));
                if (t.u->rc) q.push(Data(2, t.lca, t.u->rc, t.v));
                break;
            }
            case 2: {
                if (t.u->lc) q.push(Data(2, t.lca, t.u->lc, t.v));
                if (t.u->rc) q.push(Data(2, t.lca, t.u->rc, t.v));
                break;
            }
        }
    }
    return 0;
}