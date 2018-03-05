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
 * 「BZOJ 3073」Journeys 05-03-2018
 * 线段树优化建图
 * @author xehoth
 */
#include <cmath>
#include <cstdio>
#include <cstring>
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

struct Edge {
    int v, w;

    Edge(int v, int w) : v(v), w(w) {}
};

typedef std::vector<Edge>::iterator Iterator;

template <typename T>
inline bool tense(T &x, const T v) {
    return (x > v) ? (x = v, true) : false;
}

const int MAXG = 500000 * 10 + 1;

std::vector<Edge> g[MAXG];
int idx, id[MAXG], dis[MAXG];
bool vis[MAXG];

inline void addEdge(int u, int v, int w) { g[u].push_back(Edge(v, w)); }

char *cur;

struct Node *null;

struct Node {
    static const int NODE_SIZE;
    Node *lc, *rc;
    int id;

    Node() : lc(null), rc(null) {}

    inline void *operator new(size_t) { return cur += NODE_SIZE; }
};

const int Node::NODE_SIZE = sizeof(Node);

const int MAXM = 500000 * 4 * 2;

char pool[MAXM * Node::NODE_SIZE];

void buildL(Node *&p, int l, int r) {
    p = new Node;
    p->id = idx++;
    if (l == r) {
        id[l] = p->id;
        return;
    }
    int mid = (l + r) >> 1;
    buildL(p->lc, l, mid);
    buildL(p->rc, mid + 1, r);
    addEdge(p->lc->id, p->id, 0);
    addEdge(p->rc->id, p->id, 0);
}

void buildR(Node *&p, int l, int r) {
    p = new Node;
    p->id = idx++;
    if (l == r) {
        addEdge(p->id, id[l], 0);
        return;
    }
    int mid = (l + r) >> 1;
    buildR(p->lc, l, mid);
    buildR(p->rc, mid + 1, r);
    addEdge(p->id, p->lc->id, 0);
    addEdge(p->id, p->rc->id, 0);
}

int O;

void insertL(Node *p, int l, int r, int s, int t) {
    if (s <= l && t >= r) {
        addEdge(p->id, O, 1);
        return;
    }
    int mid = (l + r) >> 1;
    if (s <= mid) insertL(p->lc, l, mid, s, t);
    if (t > mid) insertL(p->rc, mid + 1, r, s, t);
}

void insertR(Node *p, int l, int r, int s, int t) {
    if (s <= l && t >= r) {
        addEdge(O, p->id, 0);
        return;
    }
    int mid = (l + r) >> 1;
    if (s <= mid) insertR(p->lc, l, mid, s, t);
    if (t > mid) insertR(p->rc, mid + 1, r, s, t);
}

inline void bellmanFord(int s, int n) {
    static std::queue<int> q;
    memset(dis, 0x3f, sizeof(int) * n);
    dis[s] = 0;
    q.push(s);
    for (int u; !q.empty();) {
        u = q.front();
        q.pop();
        vis[u] = false;
        for (Iterator p = g[u].begin(); p != g[u].end(); ++p) {
            if (tense(dis[p->v], dis[u] + p->w) && !vis[p->v]) {
                q.push(p->v);
                vis[p->v] = true;
            }
        }
    }
}

int main() {
    // freopen("sample/1.in", "r", stdin);
    cur = pool;
    null = (Node *)pool;
    null->lc = null->rc = null;
    int n, m, s;
    io >> n >> m >> s;
    Node *rtL, *rtR;
    buildL(rtL, 1, n);
    buildR(rtR, 1, n);
    for (int i = 0, a, b, c, d; i < m; i++) {
        io >> a >> b >> c >> d;
        O = idx++;
        insertL(rtL, 1, n, a, b);
        insertR(rtR, 1, n, c, d);
        O = idx++;
        insertL(rtL, 1, n, c, d);
        insertR(rtR, 1, n, a, b);
    }
    bellmanFord(id[s], idx);
    for (int i = 1; i <= n; i++) io << dis[id[i]] << '\n';
    return 0;
}