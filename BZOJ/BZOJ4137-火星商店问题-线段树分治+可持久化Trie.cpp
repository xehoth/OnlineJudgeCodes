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
 * 「BZOJ 4137」火星商店问题 28-02-2018
 * 线段树分治 + 可持久化 Trie
 * @author xehoth
 */
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <vector>

inline void *operator new(size_t size) {
    static const int POOL_SIZE = 1024 * 1024 * 20;
    static char pool[POOL_SIZE], *s = pool;
    char *t = s;
    s += size;
    return t;
}

inline void operator delete(void *) {}

inline void operator delete(void *, size_t) {}

struct InputOutputStream {
    enum { SIZE = 1 << 18 | 1 };

    char ibuf[SIZE], *s, *t, obuf[SIZE], *oh;

    ~InputOutputStream() { fwrite(obuf, 1, oh - obuf, stdout); }

    InputOutputStream() : s(), t(), oh(obuf) {}

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

const int MAXN = 100000 + 9;
const int MAX_LOG = 17;
const int MAXM = MAXN * (MAX_LOG + 1);

struct Node *null;

char *cur;

struct Node {
    static const int NODE_SIZE;
    Node *c[2];
    int cnt;

    inline void *operator new(size_t) { return cur += NODE_SIZE; }

    Node() : cnt() { c[0] = c[1] = null; }
};

const int Node::NODE_SIZE = sizeof(Node);

char pool[MAXM * Node::NODE_SIZE];

void insert(Node *&p, Node *pre, int v, int dep = MAX_LOG - 1) {
    p = new Node(*pre);
    p->cnt++;
    if (dep < 0) return;
    int d = v >> dep & 1;
    insert(p->c[d], pre->c[d], v, dep - 1);
}

int query(Node *l, Node *r, int v, int dep = MAX_LOG - 1) {
    if (dep < 0) return 0;
    int d = v >> dep & 1;
    if (r->c[d ^ 1]->cnt - l->c[d ^ 1]->cnt > 0)
        return (1 << dep) | query(l->c[d ^ 1], r->c[d ^ 1], v, dep - 1);
    return query(l->c[d], r->c[d], v, dep - 1);
}

Node *root[MAXN];

inline int query(int l, int r, int v) {
    return l - 1 > r ? 0 : query(root[l - 1], root[r], v);
}

struct Modify {
    int t, s, v;

    inline bool operator<(const Modify &p) const { return s < p.s; }
} mdf[MAXN];

struct Query {
    int L, R, l, r, x, *ans;
} que[MAXN];

struct SegmentTree {
    std::vector<Modify *> m;
    std::vector<Query *> q;
} d[262145];

void insert(int k, int l, int r, int pos, Modify *m) {
    d[k].m.push_back(m);
    if (l == r) return;
    int mid = (l + r) >> 1;
    pos <= mid ? insert(k << 1, l, mid, pos, m)
               : insert(k << 1 | 1, mid + 1, r, pos, m);
}

void insert(int k, int l, int r, int s, int t, Query *m) {
    if (s <= l && t >= r) {
        d[k].q.push_back(m);
        return;
    }
    int mid = (l + r) >> 1;
    if (s <= mid) insert(k << 1, l, mid, s, t, m);
    if (t > mid) insert(k << 1 | 1, mid + 1, r, s, t, m);
}

inline bool cmpModify(const Modify *a, const Modify *b) { return a->s < b->s; }

inline bool equalModify(const Modify *a, const Modify *b) {
    return a->s == b->s;
}

inline void solveNode(SegmentTree &p) {
    cur = pool;
    root[0] = null;
    int rtn = 0;
    for (int i = 0; i < (int)p.m.size(); i++) {
        rtn++;
        insert(root[rtn], root[rtn - 1], p.m[i]->v);
        while (i + 1 < (int)p.m.size() && p.m[i]->s == p.m[i + 1]->s)
            insert(root[rtn], root[rtn], p.m[++i]->v);
    }
    p.m.erase(std::unique(p.m.begin(), p.m.end(), equalModify), p.m.end());
    Modify tmp;
    for (int i = 0, l, r; i < (int)p.q.size(); i++) {
        tmp.s = p.q[i]->L;
        l = std::lower_bound(p.m.begin(), p.m.end(), &tmp, cmpModify) -
            p.m.begin() + 1;
        tmp.s = p.q[i]->R;
        r = std::upper_bound(p.m.begin(), p.m.end(), &tmp, cmpModify) -
            p.m.begin();
        *p.q[i]->ans = std::max(*p.q[i]->ans, query(l, r, p.q[i]->x));
    }
}

void solve(int k, int l, int r) {
    solveNode(d[k]);
    if (l == r) return;
    int mid = (l + r) >> 1;
    solve(k << 1, l, mid);
    solve(k << 1 | 1, mid + 1, r);
}

int day, n, m, ans[MAXN], mt, qt;

int main() {
    // freopen("sample/1.in", "r", stdin);
    cur = pool;
    null = (Node *)pool;
    null->c[0] = null->c[1] = null;
    null->cnt = 0;
    io >> n >> m;
    for (int i = 0; i <= n; i++) root[i] = null;
    for (int i = 1, x; i <= n; i++) {
        io >> x;
        insert(root[i], root[i - 1], x);
    }
    for (int i = 1, cmd, qd; i <= m; i++) {
        io >> cmd;
        switch (cmd) {
            case 0: {
                mt++;
                mdf[mt].t = ++day;
                io >> mdf[mt].s >> mdf[mt].v;
                break;
            }
            case 1: {
                qt++;
                io >> que[qt].L >> que[qt].R >> que[qt].x >> qd;
                que[qt].l = std::max(day - qd, 0) + 1;
                que[qt].r = day;
                que[qt].ans = ans + qt;
                ans[qt] = query(que[qt].L, que[qt].R, que[qt].x);
                break;
            }
        }
    }
    std::sort(mdf + 1, mdf + mt + 1);
    for (int i = 1; i <= mt; i++) insert(1, 1, day, mdf[i].t, mdf + i);
    for (int i = 1; i <= qt; i++)
        if (que[i].l <= que[i].r)
            insert(1, 1, day, que[i].l, que[i].r, que + i);
    solve(1, 1, day);
    for (int i = 1; i <= qt; i++) io << ans[i] << '\n';
    return 0;
}