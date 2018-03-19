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
 * 「BZOJ 3786」星系探索 19-03-2018
 * Treap
 * @author xehoth
 */
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

struct InputOutputStream {
    enum { SIZE = 1024 * 1024 * 5 };

    char ibuf[SIZE], *s, obuf[SIZE], *oh;

    InputOutputStream() : s(ibuf), oh(obuf) { fread(ibuf, 1, SIZE, stdin); }

    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    inline void read(char &c) {
        while (c = *s++, isspace(c))
            ;
    }

    template <typename T>
    inline void read(T &x) {
        static char c;
        static bool iosig;
        for (c = *s++, iosig = false; !isdigit(c); c = *s++) iosig |= c == '-';
        for (x = 0; isdigit(c); c = *s++) x = x * 10 + (c ^ '0');
        if (iosig) x = -x;
    }

    template <typename T>
    inline InputOutputStream &operator>>(T &x) {
        read(x);
        return *this;
    }

    template <typename T>
    inline InputOutputStream &operator<<(T x) {
        static int buf[21], cnt;
        if (x != 0) {
            if (x < 0) {
                *oh++ = '-';
                x = -x;
            }
            for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
            while (cnt) *oh++ = buf[cnt--];
        } else {
            *oh++ = '0';
        }
        *oh++ = '\n';
    }
} io;

inline unsigned int gen() {
    return rand();
    static unsigned int x = 495;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

const int MAXN = 100000 + 9;

char *cur;

struct Node *null;

struct Node {
    static const int NODE_SIZE;
    Node *fa, *lc, *rc;
    unsigned int rk;
    int size, val, cnt, in;
    long long sum, tag;

    Node(int v) : fa(null), lc(null), rc(null), rk(gen()), size(1) {
        sum = val = v;
    }

    inline void maintain() {
        size = lc->size + rc->size + 1;
        sum = lc->sum + rc->sum + val;
        cnt = lc->cnt + rc->cnt + in;
    }

    inline void cover(long long v) {
        if (this != null) {
            tag += v;
            val = (in > 0 ? val + v : val - v);
            sum += v * cnt;
        }
    }

    inline void pushDown() {
        if (tag) {
            lc->cover(tag);
            rc->cover(tag);
            tag = 0;
        }
    }

    inline void *operator new(size_t) { return cur += NODE_SIZE; }
};

const int Node::NODE_SIZE = sizeof(Node);

Node *merge(Node *u, Node *v) {
    if (u == null) return v;
    if (v == null) return u;
    if (u->rk < v->rk) {
        u->pushDown();
        u->rc = merge(u->rc, v);
        if (u->rc != null) u->rc->fa = u;
        u->maintain();
        return u;
    } else {
        v->pushDown();
        v->lc = merge(u, v->lc);
        if (v->lc != null) v->lc->fa = v;
        v->maintain();
        return v;
    }
}

void split(Node *u, int k, Node *&l, Node *&r) {
    if (u == null) {
        l = r = null;
        return;
    }
    u->fa = null;
    u->pushDown();
    if (k <= u->lc->size) {
        split(u->lc, k, l, r);
        u->lc = r;
        if (u->lc != null) u->lc->fa = u;
        r = u;
    } else {
        split(u->rc, k - u->lc->size - 1, l, r);
        u->rc = l;
        if (u->rc != null) u->rc->fa = u;
        l = u;
    }
    u->maintain();
}

inline int rank(Node *p) {
    int ret = p->lc->size + 1;
    for (; p->fa != null; p = p->fa)
        if (p == p->fa->rc) ret += p->fa->lc->size + 1;
    return ret;
}

char pool[MAXN * 2 * Node::NODE_SIZE];

int n, m, w[MAXN], cnt;

Node *in[MAXN], *out[MAXN], *root, *buc[MAXN * 2];

std::vector<int> g[MAXN];

void dfs(int u, int pre) {
    in[u] = new Node(w[u]);
    in[u]->cnt = in[u]->in = 1;
    buc[cnt++] = in[u];
    for (int i = 0, v; i < (int)g[u].size(); i++)
        if ((v = g[u][i]) != pre) dfs(v, u);
    out[u] = new Node(-w[u]);
    out[u]->cnt = out[u]->in = -1;
    buc[cnt++] = out[u];
}

Node *build(int l, int r) {
    if (l >= r) return null;
    int mid = (l + r) >> 1;
    Node *p = buc[mid];
    p->lc = build(l, mid);
    p->rc = build(mid + 1, r);
    p->maintain();
    if (p->lc != null) p->lc->fa = p;
    if (p->rc != null) p->rc->fa = p;
    return p;
}

int main() {
    cur = pool;
    null = (Node *)pool;
    null->fa = null->lc = null->rc = null;
    null->rk = gen();
    root = null;
    io >> n;
    for (int i = 2, x; i <= n; i++) {
        io >> x;
        g[x].push_back(i);
        g[i].push_back(x);
    }
    for (int i = 1; i <= n; i++) io >> w[i];
    for (int i = 0; i <= n; i++) in[i] = out[i] = null;
    dfs(1, 0);
    root = build(0, cnt);
    int m;
    io >> m;
    char cmd;
    for (int x, y; m--;) {
        io >> cmd;
        switch (cmd) {
            case 'Q': {
                io >> x;
                int k = rank(in[x]);
                Node *l, *r;
                split(root, k, l, r);
                io << l->sum;
                root = merge(l, r);
                break;
            }
            case 'C': {
                io >> x >> y;
                int k1 = rank(in[x]) - 1, k2 = rank(out[x]), k3 = rank(in[y]);
                Node *l1, *l2, *l3, *r1, *r2, *r3;
                if (k3 > k2) {
                    split(root, k3, l3, r3);
                    split(l3, k2, l2, r2);
                    split(l2, k1, l1, r1);
                } else {
                    split(root, k2, l3, r3);
                    split(l3, k1, l2, r2);
                    split(l2, k3, l1, r1);
                }
                root = merge(merge(l1, r2), merge(r1, r3));
                break;
            }
            case 'F': {
                io >> x >> y;
                int k1 = rank(in[x]) - 1, k2 = rank(out[x]);
                Node *l1, *r1, *l2, *r2;
                split(root, k2, l2, r2);
                split(l2, k1, l1, r1);
                r1->cover(y);
                root = merge(merge(l1, r1), r2);
                break;
            }
        }
    }
    return 0;
}